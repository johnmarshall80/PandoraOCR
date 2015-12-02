/**
 *  @file   PandoraOCR/src/OCRPseudoLayerPlugin.cxx
 *
 *  @brief  Implementation of the ocr pseudo layer Plugin class.
 *
 *  $Log: $
 */

#include "Helpers/XmlHelper.h"

#include "Pandora/PandoraInputTypes.h"

#include "OCRPseudoLayerPlugin.h"

#include <limits>

using namespace pandora;

OCRPseudoLayerPlugin::OCRPseudoLayerPlugin() :
    m_zPitch(1.f),
    m_zOffset(0.01f),
    m_zerothLayer(5000)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------

unsigned int OCRPseudoLayerPlugin::GetPseudoLayer(const float zCoordinate) const
{
    const float zLayer((zCoordinate + m_zOffset) / m_zPitch + static_cast<float>(m_zerothLayer));

    if (zLayer < std::numeric_limits<float>::epsilon())
        throw StatusCodeException(STATUS_CODE_FAILURE);

    return static_cast<unsigned int>(zLayer);
}

//------------------------------------------------------------------------------------------------------------------------------------------

unsigned int OCRPseudoLayerPlugin::GetPseudoLayer(const pandora::CartesianVector &positionVector) const
{
    return this->GetPseudoLayer(positionVector.GetZ());
}

//------------------------------------------------------------------------------------------------------------------------------------------

unsigned int OCRPseudoLayerPlugin::GetPseudoLayerAtIp() const
{
    return this->GetPseudoLayer(0.f);
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode OCRPseudoLayerPlugin::ReadSettings(const pandora::TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(pandora::STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "ZOffset", m_zOffset));

    PANDORA_RETURN_RESULT_IF_AND_IF(pandora::STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "ZerothLayer", m_zerothLayer));

    return STATUS_CODE_SUCCESS;
}
