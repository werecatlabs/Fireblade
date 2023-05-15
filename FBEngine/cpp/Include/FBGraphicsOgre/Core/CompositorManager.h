#ifndef _CCompositorManager_H_
#define _CCompositorManager_H_

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Graphics/IViewport.h>

namespace fb
{
    namespace render
    {
        class CompositorManager : public CSharedObject<ISharedObject>
        {
        public:
            CompositorManager();
            ~CompositorManager() override;

            void registerCompositors( SmartPtr<IViewport> viewport );
            void registerCompositor( SmartPtr<IViewport> viewport, const String &compositorName );
            void removeCompositor( SmartPtr<IViewport> viewport, const String &compositorName );

            void setCompositorEnabled( const String &name, SmartPtr<IViewport> viewport, bool enabled );
            bool isCompositorEnabled( const String &name, SmartPtr<IViewport> viewport ) const;

            void setCompositorProperties( const String &name, SmartPtr<IViewport> viewport,
                                          const Properties &properties );
            void getCompositorProperties( const String &name, SmartPtr<IViewport> viewport,
                                          Properties &properties ) const;

            SmartPtr<IGraphicsScene> getSceneManager() const;

            void setSceneManager( SmartPtr<IGraphicsScene> val );

            SmartPtr<IWindow> getWindow() const;

            void setWindow( SmartPtr<IWindow> val );

            SmartPtr<ICamera> getCamera() const;

            void setCamera( SmartPtr<ICamera> val );

            String getWorkspaceName() const;

            void setWorkspaceName( const String &val );

            bool isEnabled() const;

            void setEnabled( bool val );

        protected:
            /** Create the hard coded postfilter effects. */
            void createEffects();

            HDRListener *hdrListener;

            using DOFMap = std::map<Ogre::Viewport *, DepthOfFieldEffect *>;
            DOFMap m_dofEffects;
        };
    }  // end namespace render
}  // end namespace fb

#endif
