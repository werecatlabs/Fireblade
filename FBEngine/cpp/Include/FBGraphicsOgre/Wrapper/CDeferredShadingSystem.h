#ifndef CDeferredShadingSystem_h__
#define CDeferredShadingSystem_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IDeferredShadingSystem.h>


namespace fb
{
    namespace render
    {

        class CDeferredShadingSystem : public IDeferredShadingSystem
        {
        public:
            CDeferredShadingSystem( SmartPtr<IViewport> viewport );
            ~CDeferredShadingSystem();

            void load( SmartPtr<ISharedObject> data );
            void unload( SmartPtr<ISharedObject> data );

            u32 getMode() const;
            void setMode( u32 mode );

            bool getSSAO() const;
            void setSSAO( bool ssao );

            bool getActive() const;
            void setActive( bool active );

            bool getShadowsEnabled() const;
            void setShadowsEnabled( bool enabled );

            DeferredShadingSystem *getDeferredShading() const;
            void setDeferredShading( DeferredShadingSystem *val );

            void update();

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

            virtual void initialise( SmartPtr<scene::IDirector> objectTemplate );

            virtual void initialise( SmartPtr<scene::IDirector> objectTemplate,
                                     SmartPtr<Properties> instanceProperties );

            FB_CLASS_REGISTER_DECL;

        protected:
            DeferredShadingSystem *mSystem;
            time_interval m_nextShadowUpdate;
        };

    }  // namespace render
}  // end namespace fb

#endif  // CDeferredShadingSystem_h__
