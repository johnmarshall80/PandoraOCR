/**
 *  @file   PandoraOCR/include/OCRPseudoLayerPlugin.h
 *
 *  @brief  Header file for the ocr pseudo layer plugin class.
 *
 *  $Log: $
 */
#ifndef OCR_PSEUDO_LAYER_PLUGIN_H
#define OCR_PSEUDO_LAYER_PLUGIN_H 1

#include "Plugins/PseudoLayerPlugin.h"

namespace ocr_content
{

/**
 *  @brief  OCRPseudoLayerPlugin class
 */
class OCRPseudoLayerPlugin : public pandora::PseudoLayerPlugin
{
public:
    /**
     *  @brief  Default constructor
     */
    OCRPseudoLayerPlugin();    

    /**
     *  @brief  Get pseudolayer corresponding to a specified z coordinate
     *
     *  @param  zCoordinate the z coordinate
     *
     *  @return the pseudolayer
     */
    unsigned int GetPseudoLayer(const float zCoordinate) const;

    /**
     *  @brief  Get the wire pitch
     */
    float GetZPitch() const;    

private:
    unsigned int GetPseudoLayer(const pandora::CartesianVector &positionVector) const;
    unsigned int GetPseudoLayerAtIp() const;
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    const float     m_zPitch;       ///< The z pitch
    float           m_zOffset;      ///< The z offset
    unsigned int    m_zerothLayer;  ///< The zeroth layer
};

} // namespace ocr_content

#endif // #ifndef OCR_PSEUDO_LAYER_PLUGIN_H
