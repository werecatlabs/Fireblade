#ifndef IDeferredShadingSystem_h__
#define IDeferredShadingSystem_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        //------------------------------------------------
        class IDeferredShadingSystem : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IDeferredShadingSystem() override = default;

            /** Get rendering mode (one of DSMode). */
            virtual u32 getMode() const = 0;

            /** Set rendering mode (one of DSMode). */
            virtual void setMode( u32 mode ) = 0;

            /** Get screen space ambient occlusion mode. */
            virtual bool getSSAO() const = 0;

            /** Set screen space ambient occlusion mode. */
            virtual void setSSAO( bool ssao ) = 0;

            /** Gets whether system is activated or deactivated. */
            virtual bool getActive() const = 0;

            /** Activate or deactivate system. */
            virtual void setActive( bool active ) = 0;

            /** Get shadows enabled. */
            virtual bool getShadowsEnabled() const = 0;

            /** Set shadows enabled. */
            virtual void setShadowsEnabled( bool enabled ) = 0;

            virtual String getGBufferCompositorName() const = 0;
            virtual void setGBufferCompositorName( const String &val ) = 0;

            virtual String getShowLightingCompositorName() const = 0;
            virtual void setShowLightingCompositorName( const String &val ) = 0;

            virtual String getShowNormalsCompositorName() const = 0;
            virtual void setShowNormalsCompositorName( const String &val ) = 0;

            virtual String getShowDepthSpecularCompositorName() const = 0;
            virtual void setShowDepthSpecularCompositorName( const String &val ) = 0;

            virtual String getShowColourCompositorName() const = 0;
            virtual void setShowColourCompositorName( const String &val ) = 0;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IDeferredShadingSystem_h__
