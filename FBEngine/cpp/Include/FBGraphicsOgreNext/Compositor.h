#ifndef _CCompositor_H_
#define _CCompositor_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace render
    {

        /** @class Compositor
         *  @brief A class that manages the composition of graphical elements in a scene.
         *  @inherits CSharedGraphicsObject<ISharedObject>
         */
        class Compositor : public SharedGraphicsObject<ISharedObject>
        {
        public:
            Compositor();
            ~Compositor() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @brief Stops the Compositor. */
            void stopCompositor();

            /** @brief Sets up the enabled state of the Compositor.
             *  @param enabled Whether the Compositor should be enabled or not.
             */
            bool setupEnabled( bool enabled );

            /** @brief Checks if the Compositor is enabled.
             *  @return bool True if the Compositor is enabled, false otherwise.
             */
            bool isEnabled() const;

            /** @brief Sets the enabled state of the Compositor.
             *  @param enabled Whether the Compositor should be enabled or not.
             */
            void setEnabled( bool enabled );

            /** @brief Gets the CompositorWorkspace.
             *  @return Ogre::CompositorWorkspace* The CompositorWorkspace.
             */
            Ogre::CompositorWorkspace *getCompositorWorkspace() const;

            /** @brief Sets the CompositorWorkspace.
             *  @param compositorWorkspace The new CompositorWorkspace.
             */
            void setCompositorWorkspace( Ogre::CompositorWorkspace *compositorWorkspace );

            /** @brief Gets the SceneManager.
             *  @return SmartPtr<IGraphicsSceneManager> The SceneManager.
             */
            SmartPtr<IGraphicsScene> getSceneManager() const;

            /** @brief Sets the SceneManager.
             *  @param sceneManager The new SceneManager.
             */
            void setSceneManager( SmartPtr<IGraphicsScene> sceneManager );

            /** @brief Gets the Window.
             *  @return SmartPtr<IWindow> The Window.
             */
            SmartPtr<IWindow> getWindow() const;

            /** @brief Sets the Window.
             *  @param window The new Window.
             */
            void setWindow( SmartPtr<IWindow> window );

            /** @brief Gets the Camera.
             *  @return SmartPtr<ICamera> The Camera.
             */
            SmartPtr<ICamera> getCamera() const;

            /** @brief Sets the Camera.
             *  @param camera The new Camera.
             */
            void setCamera( SmartPtr<ICamera> camera );

            /** @brief Gets the WorkspaceName.
             *  @return String The WorkspaceName.
             */
            String getWorkspaceName() const;

            /** @brief Sets the WorkspaceName.
             *  @param workspaceName The new WorkspaceName.
             */
            void setWorkspaceName( const String &workspaceName );

            /** @brief Sets up a test CompositorWorkspace.
             *  @return Ogre::CompositorWorkspace* The test CompositorWorkspace.
             */
            Ogre::CompositorWorkspace *setupTestCompositor();

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::getProperties */
            SmartPtr<Properties> getProperties() const;

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::setProperties */
            void setProperties( SmartPtr<Properties> properties );

            /** @copydoc CGraphicsObjectOgreNext<ICamera>::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            /** @class StateListener
             *  @brief A class that listens to state changes in the Compositor.
             *  @inherits IStateListener
             */
            class StateListener : public IStateListener
            {
            public:
                StateListener();
                ~StateListener() override;

                void unload( SmartPtr<ISharedObject> data ) override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<Compositor> getOwner() const;
                void setOwner( SmartPtr<Compositor> owner );

            protected:
                AtomicWeakPtr<Compositor> m_owner;
            };

            Ogre::CompositorWorkspace *setupBasicCompositor();
            Ogre::CompositorWorkspace *setupTerrainCompositor();

            /** @var Ogre::CompositorWorkspace *m_compositorWorkspace
             *  @brief A pointer to the CompositorWorkspace managed by this Compositor.
             */
            Ogre::CompositorWorkspace *m_compositorWorkspace = nullptr;

            /** @var Ogre::TerraWorkspaceListener *mTerraWorkspaceListener
             *  @brief A pointer to the TerraWorkspaceListener associated with this Compositor.
             */
            Ogre::TerraWorkspaceListener *mTerraWorkspaceListener = nullptr;

            Ogre::CompositorPassSceneDef *m_scenePassDef = nullptr;

            /** @var SmartPtr<IGraphicsSceneManager> m_sceneManager
             *  @brief A smart pointer to the IGraphicsSceneManager managed by this Compositor.
             */
            AtomicWeakPtr<IGraphicsScene> m_sceneManager;

            /** @var SmartPtr<IWindow> m_window
             *  @brief A smart pointer to the IWindow managed by this Compositor.
             */
            AtomicWeakPtr<IWindow> m_window;

            /** @var SmartPtr<ICamera> m_camera
             *  @brief A smart pointer to the ICamera managed by this Compositor.
             */
            AtomicWeakPtr<ICamera> m_camera;

            /** @var String m_workspaceName
             *  @brief The name of the CompositorWorkspace managed by this Compositor.
             */
            String m_workspaceName;

            static u32 m_nameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
