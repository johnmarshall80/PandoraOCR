/**
 *  @file   TestPandora/src/PandoraOCR.cc
 *
 *  @brief  Implementation for the pandora ocr application
 *
 *  $Log: $
 */

#include "Api/PandoraApi.h"

#ifdef MONITORING
#include "TApplication.h"
#endif

#include "OCRPseudoLayerPlugin.h"
#include "VisualMonitoringAlgorithm.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/time.h>

extern "C"
{
    #include "ihead.h"
    #include "mis.h"

    MIS *readmisfile(char *file);
    void bits2bytes(u_char *p, u_char *q, u_int pixels);
    void freemis(MIS *mis);
}

using namespace pandora;

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Parameters class
 */
class Parameters
{
public:
    /**
     *  @brief Default constructor
     */
    Parameters();

    std::string     m_characterFile;                ///< The path to the input character .mis file (mandatory parameter)
    std::string     m_pandoraSettingsFile;          ///< The path to the pandora settings file (mandatory parameter)
    int             m_nEventsToProcess;             ///< The number of events to process (default all events in file)
    bool            m_shouldDisplayEventTime;       ///< Whether event times should be calculated and displayed (default false)
    bool            m_shouldDisplayEventNumber;     ///< Whether event numbers should be displayed (default false)
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Parse the command line arguments, setting the application parameters
 *
 *  @param  argc argument count
 *  @param  argv argument vector
 *  @param  parameters to receive the application parameters
 *
 *  @return success
 */
bool ParseCommandLine(int argc, char *argv[], Parameters &parameters);

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Create a new hit in a pandora instance
 *
 *  @param  pPandora address of the pandora instance
 *  @param  hitX the hit x coordinate
 *  @param  hitZ the hit z coordinate
 *  @param  hitCounter the hit counter
 *
 *  @return status code
 */
StatusCode CreateHit(const Pandora *const pPandora, const float hitX, const float hitZ, unsigned int &hitCounter);

//------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    try
    {
        // Parse command line parameters
        Parameters parameters;

        if (!ParseCommandLine(argc, argv, parameters))
            return 1;
#ifdef MONITORING
        TApplication *const pTApplication = new TApplication("MyTest", &argc, argv);
        pTApplication->SetReturnFromRun(kTRUE);
#endif
        // Construct pandora instance
        const Pandora *const pPandora = new Pandora();
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerPlugin(*pPandora, new ocr_content::OCRPseudoLayerPlugin));
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*pPandora, "OCRVisualMonitoring", new ocr_content::VisualMonitoringAlgorithm::Factory));

        // Read in pandora settings from config file       
        PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*pPandora, parameters.m_pandoraSettingsFile));

        // Read in character file
        MIS *pMis = readmisfile(const_cast<char*>(parameters.m_characterFile.c_str()));

        if (pMis->misd != 1)
        {
            std::cout << "incorrect entry size or depth" << std::endl;
            throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);
        }

        u_char *const pBytesData = new u_char[pMis->misw * pMis->mish];
        bits2bytes(pMis->data, pBytesData, pMis->misw * pMis->mish);

        unsigned int hitCounter(0);
        const u_char *pBytesDataAccess(pBytesData);

        // Process the events (characters)
        for (int iCharacter = 0 ; iCharacter < pMis->ent_num ; ++iCharacter)
        {
            if ((iCharacter >= parameters.m_nEventsToProcess) && (0 < parameters.m_nEventsToProcess))
                break;

            struct timeval startTime, endTime;

            if (parameters.m_shouldDisplayEventNumber)
                std::cout << std::endl << "   PROCESSING EVENT: " << iCharacter << std::endl << std::endl;

            if (parameters.m_shouldDisplayEventTime)
                (void) gettimeofday(&startTime, NULL);

            for (int iPixelZ = 0 ; iPixelZ < pMis->enth ; ++iPixelZ)
            {
                for (int iPixelX = 0 ; iPixelX < pMis->entw ; ++iPixelX)
                {
                    const bool isFilledPixel(*pBytesDataAccess++);
                    //std::cout << (isFilledPixel ? "#" : ".") << (((l + 1) % pMis->misw) ? " " : "\n");

                    if (!isFilledPixel)
                        continue;

                    // ATTN Mirror z to support default event display view
                    const float hitX(static_cast<float>(iPixelX + 0.5f));
                    const float hitZ(static_cast<float>(pMis->enth - iPixelZ - 0.5f));
                    PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, CreateHit(pPandora, hitX, hitZ, hitCounter));
                }
            }

            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*pPandora));

            if (parameters.m_shouldDisplayEventTime)
            {
                (void) gettimeofday(&endTime, NULL);
                std::cout << "Event time " << (endTime.tv_sec + (endTime.tv_usec / 1.e6) - startTime.tv_sec - (startTime.tv_usec / 1.e6)) << std::endl;
            }

            PANDORA_THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*pPandora));
        }   

        freemis(pMis);
        delete [] pBytesData;
        delete pPandora;
    }
    catch (StatusCodeException &statusCodeException)
    {
        std::cerr << "Pandora Exception caught: " << statusCodeException.ToString() << std::endl;
        return 1;
    }

    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ParseCommandLine(int argc, char *argv[], Parameters &parameters)
{
    int c(0);

    while ((c = getopt(argc, argv, "c:i:n:t::N::h")) != -1)
    {
        switch (c)
        {
        case 'c':
            parameters.m_characterFile = optarg;
            break;
        case 'i':
            parameters.m_pandoraSettingsFile = optarg;
            break;
        case 'n':
            parameters.m_nEventsToProcess = atoi(optarg);
            break;
        case 't':
            parameters.m_shouldDisplayEventTime = true;
            break;
        case 'N':
            parameters.m_shouldDisplayEventNumber = true;
            break;
        case 'h':
        default:
            std::cout << std::endl << "./bin/PandoraOCR " << std::endl
                      << "    -c CharacterFile.mis    (mandatory)" << std::endl
                      << "    -i PandoraSettings.xml  (mandatory)" << std::endl
                      << "    -n NEventsToProcess     (optional)" << std::endl
                      << "    -N                      (optional, display event numbers)" << std::endl
                      << "    -t                      (optional, display event times)" << std::endl << std::endl;
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateHit(const Pandora *const pPandora, const float hitX, const float hitZ, unsigned int &hitCounter)
{
    PandoraApi::CaloHit::Parameters caloHitParameters;
    caloHitParameters.m_positionVector = CartesianVector(hitX, 0.f, hitZ);
    caloHitParameters.m_expectedDirection = CartesianVector(0.f, 0.f, 1.f);
    caloHitParameters.m_cellNormalVector = CartesianVector(0.f, 0.f, 1.f);
    caloHitParameters.m_cellGeometry = RECTANGULAR;
    caloHitParameters.m_cellSize0 = 0.9f;
    caloHitParameters.m_cellSize1 = 0.9f;
    caloHitParameters.m_cellThickness = 0.9f;
    caloHitParameters.m_nCellRadiationLengths = 0.1f;
    caloHitParameters.m_nCellInteractionLengths = 0.1f;
    caloHitParameters.m_time = 0.f;
    caloHitParameters.m_inputEnergy = 1.f;
    caloHitParameters.m_mipEquivalentEnergy = 1.f;
    caloHitParameters.m_electromagneticEnergy = 1.f;
    caloHitParameters.m_hadronicEnergy = 1.f;
    caloHitParameters.m_isDigital = false;
    caloHitParameters.m_hitType = HIT_CUSTOM;
    caloHitParameters.m_hitRegion = SINGLE_REGION;
    caloHitParameters.m_layer = 0;
    caloHitParameters.m_isInOuterSamplingLayer = false;
    caloHitParameters.m_pParentAddress = reinterpret_cast<void *>(hitCounter++);

    return PandoraApi::CaloHit::Create(*pPandora, caloHitParameters);
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters() :
    m_nEventsToProcess(-1),
    m_shouldDisplayEventTime(false),
    m_shouldDisplayEventNumber(false)
{
}
