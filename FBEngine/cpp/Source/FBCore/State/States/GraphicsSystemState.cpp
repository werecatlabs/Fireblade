#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/GraphicsSystemState.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Interface/Graphics/IDeferredShadingSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSettings.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IInstanceManager.h>
#include <FBCore/Interface/Graphics/IOverlayManager.h>
#include <FBCore/Interface/Graphics/IMaterialManager.h>
#include <FBCore/Interface/Graphics/IMeshConverter.h>
#include <FBCore/Interface/Resource/IMeshManager.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/ISpriteRenderer.h>
#include <FBCore/Interface/Graphics/ITextureManager.h>
#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Interface/Resource/IResourceGroupManager.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, GraphicsSystemState, BaseState );

        GraphicsSystemState::GraphicsSystemState()
        {
        }

        GraphicsSystemState::~GraphicsSystemState()
        {
            unload( nullptr );
        }

        void GraphicsSystemState::load( SmartPtr<ISharedObject> data )
        {
            auto graphicsSystem = getGraphicsSystem();
            if( graphicsSystem )
            {
                graphicsSystem->load( data );
            }
        }

        void GraphicsSystemState::unload( SmartPtr<ISharedObject> data )
        {
            auto graphicsSystem = getGraphicsSystem();
            if( graphicsSystem )
            {
                graphicsSystem->unload( data );
            }
        }

        IGraphicsSystem::RenderApi GraphicsSystemState::getRenderApi() const
        {
            return m_renderApi;
        }

        void GraphicsSystemState::setRenderApi( IGraphicsSystem::RenderApi api )
        {
            m_renderApi = api;
        }

        SmartPtr<IMeshConverter> GraphicsSystemState::getMeshConverter() const
        {
            return m_meshConverter;
        }

        void GraphicsSystemState::setMeshConverter( SmartPtr<IMeshConverter> meshConverter )
        {
            m_meshConverter = meshConverter;
        }

        SmartPtr<IGraphicsSystem> GraphicsSystemState::getGraphicsSystem() const
        {
            auto pGraphicsSystem = m_graphicsSystem.load();
            return pGraphicsSystem.lock();
        }

        void GraphicsSystemState::setGraphicsSystem( SmartPtr<IGraphicsSystem> graphicsSystem )
        {
            m_graphicsSystem = graphicsSystem;
        }

        SmartPtr<IGraphicsScene> GraphicsSystemState::getDefaultSceneManager() const
        {
            return m_defaultSceneManager;
        }

        void GraphicsSystemState::setDefaultSceneManager( SmartPtr<IGraphicsScene> defaultSceneManager )
        {
            m_defaultSceneManager = defaultSceneManager;
        }

        SmartPtr<IDebug> GraphicsSystemState::getDebug() const
        {
            return m_debug;
        }

        void GraphicsSystemState::setDebug( SmartPtr<IDebug> debug )
        {
            m_debug = debug;
        }

        SmartPtr<IOverlayManager> GraphicsSystemState::getOverlayManager() const
        {
            return m_overlayManager;
        }

        void GraphicsSystemState::setOverlayManager( SmartPtr<IOverlayManager> overlayManager )
        {
            m_overlayManager = overlayManager;
        }

        Array<SmartPtr<IWindow>> GraphicsSystemState::getWindows() const
        {
            return Array<SmartPtr<IWindow>>( m_windows.begin(), m_windows.end() );
        }

        void GraphicsSystemState::setWindows( Array<SmartPtr<IWindow>> windows )
        {
            m_windows = ConcurrentArray<SmartPtr<IWindow>>( windows.begin(), windows.end() );
        }

        SmartPtr<IGraphicsSettings> GraphicsSystemState::createConfiguration()
        {
            auto graphicsSystem = getGraphicsSystem();
            if( graphicsSystem )
            {
                return graphicsSystem->createConfiguration();
            }

            return nullptr;
        }

        bool GraphicsSystemState::configure( SmartPtr<IGraphicsSettings> config )
        {
            auto graphicsSystem = getGraphicsSystem();
            if( graphicsSystem )
            {
                return graphicsSystem->configure( config );
            }

            return false;
        }

        void GraphicsSystemState::messagePump()
        {
            auto graphicsSystem = getGraphicsSystem();
            if( graphicsSystem )
            {
                graphicsSystem->messagePump();
            }
        }

        SmartPtr<IGraphicsScene> GraphicsSystemState::addSceneManager( const String &type,
                                                                      const String &name )
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->addGraphicsScene( type, name );
        }

        SmartPtr<IGraphicsScene> GraphicsSystemState::getSceneManager() const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getGraphicsScene();
        }

        SmartPtr<IGraphicsScene> GraphicsSystemState::getSceneManager( const String &name ) const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getGraphicsScene( name );
        }

        SmartPtr<IGraphicsScene> GraphicsSystemState::getSceneManagerById( hash32 id ) const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getGraphicsSceneById( id );
        }

        SmartPtr<IResourceGroupManager> GraphicsSystemState::getResourceGroupManager() const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getResourceGroupManager();
        }

        SmartPtr<IMaterialManager> GraphicsSystemState::getMaterialManager() const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getMaterialManager();
        }

        SmartPtr<ITextureManager> GraphicsSystemState::getTextureManager() const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getTextureManager();
        }

        SmartPtr<IInstanceManager> GraphicsSystemState::getInstanceManager() const
        {
            return m_instanceManager;
        }

        SmartPtr<ISpriteRenderer> GraphicsSystemState::getSpriteRenderer() const
        {
            return m_spriteRenderer;
        }

        SmartPtr<IWindow> GraphicsSystemState::createRenderWindow(
            const String &name, u32 width, u32 height, bool fullScreen,
            const SmartPtr<Properties> &properties )
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->createRenderWindow( name, width, height, fullScreen, properties );
        }

        SmartPtr<IWindow> GraphicsSystemState::getRenderWindow(
            const String &name /*= StringUtil::EmptyString */ ) const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getRenderWindow( name );
        }

        SmartPtr<IWindow> GraphicsSystemState::getDefaultWindow() const
        {
            auto graphicsSystem = getGraphicsSystem();
            return graphicsSystem->getDefaultWindow();
        }

        void GraphicsSystemState::setDefaultWindow( SmartPtr<IWindow> defaultWindow )
        {
            auto graphicsSystem = getGraphicsSystem();
            graphicsSystem->setDefaultWindow( defaultWindow );
        }

        SmartPtr<IDeferredShadingSystem> GraphicsSystemState::addDeferredShadingSystem(
            SmartPtr<IViewport> vp )
        {
            return nullptr;
        }

        void GraphicsSystemState::removeDeferredShadingSystem( SmartPtr<IViewport> vp )
        {
        }

        Array<SmartPtr<IDeferredShadingSystem>> GraphicsSystemState::getDeferredShadingSystems() const
        {
            return Array<SmartPtr<IDeferredShadingSystem>>();
        }

        void GraphicsSystemState::loadObject( SmartPtr<ISharedObject> graphicsObject,
                                              bool forceQueue /*= false */ )
        {
        }

        void GraphicsSystemState::unloadObject( SmartPtr<ISharedObject> graphicsObject,
                                                bool forceQueue /*= false */ )
        {
        }

        void GraphicsSystemState::setupRenderer( SmartPtr<IGraphicsScene> sceneManager,
                                                 SmartPtr<IWindow> window, SmartPtr<ICamera> camera,
                                                 String workspaceName, bool val )
        {
        }

        Thread::Task GraphicsSystemState::getStateTask() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto threadPool = applicationManager->getThreadPool() )
            {
                //if( threadPool->getNumThreads() > 0 )
                {
                    const auto hasTasks = applicationManager->hasTasks();
                    return hasTasks ? Thread::Task::Render : Thread::Task::Primary;
                }
            }

            return Thread::Task::Primary;
        }

        Thread::Task GraphicsSystemState::getRenderTask() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( const auto taskManager = applicationManager->getTaskManager() )
            {
                if( applicationManager->isLoading() )
                {
                    return Thread::Task::Primary;
                }

                // if (isUpdating())
                //{
                //	return Thread::getCurrentTask();
                // }

                if( const auto threadPool = applicationManager->getThreadPool() )
                {
                    if( threadPool->getNumThreads() > 0 )
                    {
                        if( const auto task = taskManager->getTask( Thread::Task::Render ) )
                        {
                            if( task->isExecuting() )
                            {
                                return Thread::Task::Render;
                            }

                            if( task->isPrimary() )
                            {
                                return Thread::Task::Primary;
                            }

                            return Thread::Task::Primary;
                        }
                    }
                    else
                    {
                        if( const auto task = taskManager->getTask( Thread::Task::Render ) )
                        {
                            if( task->isExecuting() )
                            {
                                return Thread::Task::Render;
                            }
                        }

                        return Thread::Task::Primary;
                    }
                }

                return applicationManager->hasTasks() ? Thread::Task::Render : Thread::Task::Primary;
            }

            return Thread::Task::Primary;
        }

    }  // namespace render
}  // namespace fb
