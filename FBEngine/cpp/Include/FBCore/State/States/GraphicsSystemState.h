#ifndef GraphicsSystemState_h__
#define GraphicsSystemState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Atomics/Atomics.h>
#include "FBCore/Core/ConcurrentQueue.h"
#include <FBCore/Math/AABB3.h>

namespace fb
{
    namespace render
    {
        class GraphicsSystemState : public BaseState
        {
        public:
            GraphicsSystemState();
            ~GraphicsSystemState() override;

            /** @copydoc IGraphicsSystem::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            IGraphicsSystem::RenderApi getRenderApi() const;
            void setRenderApi( IGraphicsSystem::RenderApi api );

            SmartPtr<IGraphicsScene> getDefaultSceneManager() const;
            void setDefaultSceneManager( SmartPtr<IGraphicsScene> defaultSceneManager );

            SmartPtr<IDebug> getDebug() const;
            void setDebug( SmartPtr<IDebug> debug );

            SmartPtr<IOverlayManager> getOverlayManager() const;
            void setOverlayManager( SmartPtr<IOverlayManager> overlayManager );

            Array<SmartPtr<IWindow>> getWindows() const;
            void setWindows( Array<SmartPtr<IWindow>> windows );

            SmartPtr<IGraphicsSettings> createConfiguration();

            bool configure( SmartPtr<IGraphicsSettings> config );

            void messagePump();

            SmartPtr<IGraphicsScene> addSceneManager( const String &type, const String &name );

            SmartPtr<IGraphicsScene> getSceneManager() const;

            SmartPtr<IGraphicsScene> getSceneManager( const String &name ) const;

            SmartPtr<IGraphicsScene> getSceneManagerById( hash32 id ) const;

            SmartPtr<IResourceGroupManager> getResourceGroupManager() const;

            SmartPtr<IMaterialManager> getMaterialManager() const;

            SmartPtr<ITextureManager> getTextureManager() const;

            SmartPtr<IInstanceManager> getInstanceManager() const;

            SmartPtr<ISpriteRenderer> getSpriteRenderer() const;

            SmartPtr<IWindow> createRenderWindow( const String &name, u32 width, u32 height,
                                                  bool fullScreen,
                                                  const SmartPtr<Properties> &properties );

            SmartPtr<IWindow> getRenderWindow( const String &name = StringUtil::EmptyString ) const;

            SmartPtr<IWindow> getDefaultWindow() const;

            void setDefaultWindow( SmartPtr<IWindow> defaultWindow );

            SmartPtr<IDeferredShadingSystem> addDeferredShadingSystem( SmartPtr<IViewport> vp );

            void removeDeferredShadingSystem( SmartPtr<IViewport> vp );

            Array<SmartPtr<IDeferredShadingSystem>> getDeferredShadingSystems() const;

            void loadObject( SmartPtr<ISharedObject> graphicsObject, bool forceQueue = false );

            void unloadObject( SmartPtr<ISharedObject> graphicsObject, bool forceQueue = false );

            void setupRenderer( SmartPtr<IGraphicsScene> sceneManager, SmartPtr<IWindow> window,
                                SmartPtr<ICamera> camera, String workspaceName, bool val );

            Thread::Task getStateTask() const;

            Thread::Task getRenderTask() const;

            SmartPtr<IMeshConverter> getMeshConverter() const;

            void setMeshConverter( SmartPtr<IMeshConverter> meshConverter );

            SmartPtr<IGraphicsSystem> getGraphicsSystem() const;

            void setGraphicsSystem( SmartPtr<IGraphicsSystem> graphicsSystem );

            FB_CLASS_REGISTER_DECL;

        protected:
            ///
            AtomicWeakPtr<IGraphicsSystem> m_graphicsSystem;

            ///
            Atomic<IGraphicsSystem::RenderApi> m_renderApi = IGraphicsSystem::RenderApi::None;

            ///
            AtomicSmartPtr<IGraphicsScene> m_defaultSceneManager;

            ///
            AtomicSmartPtr<IDebug> m_debug;

            SmartPtr<ISpriteRenderer> m_spriteRenderer;

            ///
            AtomicSmartPtr<IOverlayManager> m_overlayManager;

            ///
            AtomicSmartPtr<IResourceGroupManager> m_resourceGroupManager;

            ///
            AtomicSmartPtr<IMaterialManager> m_materialManager;

            ///
            AtomicSmartPtr<ITextureManager> m_textureManager;

            ///
            AtomicSmartPtr<IInstanceManager> m_instanceManager;

            ///
            AtomicSmartPtr<IMeshConverter> m_meshConverter;

            ///
            AtomicSmartPtr<IMeshManager> m_meshManager;

            ///
            AtomicSmartPtr<IWindow> m_defaultWindow;

            ///
            ConcurrentArray<SmartPtr<IWindow>> m_windows;

            ///
            ConcurrentArray<SmartPtr<IDeferredShadingSystem>> m_deferredShadingSystems;

            ///
            ConcurrentArray<SmartPtr<IGraphicsScene>> m_sceneManagers;

            ///
            ConcurrentArray<SmartPtr<ISharedObject>> m_graphicsObjects;

            ///
            ConcurrentQueue<SmartPtr<ISharedObject>> m_loadQueue;

            ///
            ConcurrentQueue<SmartPtr<ISharedObject>> m_unloadQueue;

            ///
            atomic_bool m_useShaderGenerator = false;

            ///
            atomic_bool m_isUpdating = false;
        };
    }  // namespace render
}  // namespace fb

#endif  // GraphicsSystemState_h__
