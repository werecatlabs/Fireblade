#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "FBGraphicsOgre/Templates/DeferredShadingSystemTemplate.h"

namespace fb
{

    DeferredShadingSystemTemplate::DeferredShadingSystemTemplate()
    {
        m_sGBufferCompositorName = "DeferredShading/GBuffer";
        m_sShowLightingCompositorName = "DeferredShading/ShowLit";
        m_sShowNormalsCompositorName = "DeferredShading/ShowNormals";
        m_sShowDepthSpecularCompositorName = "DeferredShading/ShowDepthSpecular";
        m_sShowColourCompositorName = "DeferredShading/ShowColour";
    }

    DeferredShadingSystemTemplate::~DeferredShadingSystemTemplate()
    {
    }

    fb::String DeferredShadingSystemTemplate::getGBufferCompositorName() const
    {
        return m_sGBufferCompositorName;
    }

    void DeferredShadingSystemTemplate::setGBufferCompositorName( const String &val )
    {
        m_sGBufferCompositorName = val;
    }

    fb::String DeferredShadingSystemTemplate::getShowLightingCompositorName() const
    {
        return m_sShowLightingCompositorName;
    }

    void DeferredShadingSystemTemplate::setShowLightingCompositorName( const String &val )
    {
        m_sShowLightingCompositorName = val;
    }

    fb::String DeferredShadingSystemTemplate::getShowNormalsCompositorName() const
    {
        return m_sShowNormalsCompositorName;
    }

    void DeferredShadingSystemTemplate::setShowNormalsCompositorName( const String &val )
    {
        m_sShowNormalsCompositorName = val;
    }

    fb::String DeferredShadingSystemTemplate::getShowDepthSpecularCompositorName() const
    {
        return m_sShowDepthSpecularCompositorName;
    }

    void DeferredShadingSystemTemplate::setShowDepthSpecularCompositorName( const String &val )
    {
        m_sShowDepthSpecularCompositorName = val;
    }

    fb::String DeferredShadingSystemTemplate::getShowColourCompositorName() const
    {
        return m_sShowColourCompositorName;
    }

    void DeferredShadingSystemTemplate::setShowColourCompositorName( const String &val )
    {
        m_sShowColourCompositorName = val;
    }

    fb::String DeferredShadingSystemTemplate::getLightCompositionPassName() const
    {
        return m_sLightCompositionPassName;
    }

    void DeferredShadingSystemTemplate::setLightCompositionPassName( const String &val )
    {
        m_sLightCompositionPassName = val;
    }

    fb::String DeferredShadingSystemTemplate::getRenderTargetName() const
    {
        return m_renderTargetName;
    }

    void DeferredShadingSystemTemplate::setRenderTargetName( const String &val )
    {
        m_renderTargetName = val;
    }

    bool DeferredShadingSystemTemplate::isDirty() const
    {
        return false;
    }

    void DeferredShadingSystemTemplate::setDirty( bool dirty )
    {
    }

}  // namespace fb
