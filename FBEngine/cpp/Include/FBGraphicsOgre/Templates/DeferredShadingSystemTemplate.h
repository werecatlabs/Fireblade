#ifndef DeferredShadingSystemTemplate_h__
#define DeferredShadingSystemTemplate_h__

//#include <FBCore/Interface/System/IDirector.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{

    class DeferredShadingSystemTemplate : public CSharedObject<ISharedObject>
    {
    public:
        DeferredShadingSystemTemplate();
        ~DeferredShadingSystemTemplate();

        String getGBufferCompositorName() const;
        void setGBufferCompositorName( const String &val );

        String getShowLightingCompositorName() const;
        void setShowLightingCompositorName( const String &val );

        String getShowNormalsCompositorName() const;
        void setShowNormalsCompositorName( const String &val );

        String getShowDepthSpecularCompositorName() const;
        void setShowDepthSpecularCompositorName( const String &val );

        String getShowColourCompositorName() const;
        void setShowColourCompositorName( const String &val );

        String getLightCompositionPassName() const;
        void setLightCompositionPassName( const String &val );

        String getRenderTargetName() const;
        void setRenderTargetName( const String &val );

        bool isDirty() const;
        void setDirty( bool dirty );

    protected:
        String m_sGBufferCompositorName;
        String m_sShowLightingCompositorName;
        String m_sShowNormalsCompositorName;
        String m_sShowDepthSpecularCompositorName;
        String m_sShowColourCompositorName;

        String m_sLightCompositionPassName;

        String m_renderTargetName;
    };

}  // end namespace fb

#endif  // DeferredShadingSystemTemplate_h__
