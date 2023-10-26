#ifndef _CCompositorManager_H_
#define _CCompositorManager_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        class CompositorManager : public ISharedObject
        {
        public:
            CompositorManager();
            CompositorManager( const CompositorManager &other ) = delete;
            ~CompositorManager() override;

            void setupRenderer( SmartPtr<IGraphicsScene> pISceneManager,
                                SmartPtr<IWindow> pIWindow, SmartPtr<ICamera> pICamera,
                                String workspaceName, bool val );
            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

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
            void setSceneManager( SmartPtr<IGraphicsScene> sceneManager );

            SmartPtr<IWindow> getWindow() const;
            void setWindow( SmartPtr<IWindow> window );

            SmartPtr<ICamera> getCamera() const;
            void setCamera( SmartPtr<ICamera> camera );

            FB_CLASS_REGISTER_DECL;

        protected:
            Array<SmartPtr<Compositor>> m_compositors;
            SmartPtr<IGraphicsScene> m_sceneManager;
            SmartPtr<IWindow> m_window;
            SmartPtr<ICamera> m_camera;
        };
    }  // end namespace render
}  // end namespace fb

#endif
