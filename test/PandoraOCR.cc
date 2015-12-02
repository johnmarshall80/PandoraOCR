/**
 *  @file   TestPandora/src/PandoraInterface.cc
 *
 *  @brief  Implementation for the pandora interface binary
 *
 *  $Log: $
 */

// External Pandora includes
#include "Api/PandoraApi.h"

extern "C"
{
#include <ihead.h>
#include <mis.h>
}

#include "OCRPseudoLayerPlugin.h"
#include "VisualMonitoringAlgorithm.h"

#ifdef MONITORING
#include "TApplication.h"
#endif

#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/time.h>

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

extern "C"
{
    void bits2bytes(u_char *p, u_char *q, u_int pixels);
    void freemis(MIS *mis);
    MIS *readmisfile(char *file);
}

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
        const pandora::Pandora *const pPandora = new pandora::Pandora();
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerPlugin(*pPandora, new ocr_content::OCRPseudoLayerPlugin));
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::RegisterAlgorithmFactory(*pPandora, "OCRVisualMonitoring", new ocr_content::VisualMonitoringAlgorithm::Factory));

        // Read in pandora settings from config file       
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ReadSettings(*pPandora, parameters.m_pandoraSettingsFile));

        // Process the events
        int nEvents(0);

        while ((nEvents++ < parameters.m_nEventsToProcess) || (0 > parameters.m_nEventsToProcess))
        {
            struct timeval startTime, endTime;

            if (parameters.m_shouldDisplayEventNumber)
                std::cout << std::endl << "   PROCESSING EVENT: " << (nEvents - 1) << std::endl << std::endl;

            if (parameters.m_shouldDisplayEventTime)
                (void) gettimeofday(&startTime, NULL);

            char *data8 = nullptr;
            char *misfile = const_cast<char*>(parameters.m_characterFile.c_str());

            MIS *mis = readmisfile(misfile);

            if (mis->misd != 1)
            {
                std::cout << "incorrect entry size or depth" << std::endl;
                throw pandora::StatusCodeException(pandora::STATUS_CODE_INVALID_PARAMETER);
            }

            if ((data8 = (char *)malloc(mis->misw * mis->mish * sizeof(char))) == NULL)
            {
                std::cout << "unable to allocate 8 bit space" << std::endl;
                throw pandora::StatusCodeException(pandora::STATUS_CODE_FAILURE);
            }

            bits2bytes(reinterpret_cast<u_char*>(mis->data), reinterpret_cast<u_char*>(data8), static_cast<u_int>(mis->misw * mis->mish));

            char *dptr = data8;
            for (int j = 0 ; j < mis->ent_num ; j++ )
            {   
                for (int k = 0 ; k < mis->enth ; k++ )
                    for (int l = 0 ; l < mis->entw ; l++ )
                        fprintf(stdout, "%c%c", ( *dptr++ ) ? '#' : '.', ((l+1) % mis->misw) ? ' ' : '\n');
                fprintf(stdout, "\n");
            }   

            free(data8);
            freemis(mis);

            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*pPandora));

            if (parameters.m_shouldDisplayEventTime)
            {
                (void) gettimeofday(&endTime, NULL);
                std::cout << "Event time " << (endTime.tv_sec + (endTime.tv_usec / 1.e6) - startTime.tv_sec - (startTime.tv_usec / 1.e6)) << std::endl;
            }

            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*pPandora));
        }

        delete pPandora;
    }
    catch (pandora::StatusCodeException &statusCodeException)
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
//------------------------------------------------------------------------------------------------------------------------------------------

Parameters::Parameters() :
    m_nEventsToProcess(-1),
    m_shouldDisplayEventTime(false),
    m_shouldDisplayEventNumber(false)
{
}
