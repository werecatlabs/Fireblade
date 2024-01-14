#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/GraphicsSystemState.h>
#include <FBCore/System/ApplicationManager.h>
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

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, GraphicsSystemState, BaseState );

    GraphicsSystemState::GraphicsSystemState() = default;

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

    auto GraphicsSystemState::getRenderApi() const -> IGraphicsSystem::RenderApi
    {
        return m_renderApi;
    }

    void GraphicsSystemState::setRenderApi( IGraphicsSystem::RenderApi api )
    {
        m_renderApi = api;
    }

    auto GraphicsSystemState::getMeshConverter() const -> SmartPtr<IMeshConverter>
    {
        return m_meshConverter;
    }

    void GraphicsSystemState::setMeshConverter( SmartPtr<IMeshConverter> meshConverter )
    {
        m_meshConverter = meshConverter;
    }

    auto GraphicsSystemState::getGraphicsSystem() const -> SmartPtr<IGraphicsSystem>
    {
        auto pGraphicsSystem = m_graphicsSystem.load();
        return pGraphicsSystem.lock();
    }

    void GraphicsSystemState::setGraphicsSystem( SmartPtr<IGraphicsSystem> graphicsSystem )
    {
        m_graphicsSystem = graphicsSystem;
    }

    auto GraphicsSystemState::getDefaultSceneManager() const -> SmartPtr<IGraphicsScene>
    {
        return m_defaultSceneManager;
    }

    void GraphicsSystemState::setDefaultSceneManager( SmartPtr<IGraphicsScene> defaultSceneManager )
    {
        m_defaultSceneManager = defaultSceneManager;
    }

    auto GraphicsSystemState::getDebug() const -> SmartPtr<IDebug>
    {
        return m_debug;
    }

    void GraphicsSystemState::setDebug( SmartPtr<IDebug> debug )
    {
        m_debug = debug;
    }

    auto GraphicsSystemState::getOverlayManager() const -> SmartPtr<IOverlayManager>
    {
        return m_overlayManager;
    }

    void GraphicsSystemState::setOverlayManager( SmartPtr<IOverlayManager> overlayManager )
    {
        m_overlayManager = overlayManager;
    }

    auto GraphicsSystemState::getWindows() const -> Array<SmartPtr<IWindow>>
    {
        return Array<SmartPtr<IWindow>>( m_windows.begin(), m_windows.end() );
    }

    void GraphicsSystemState::setWindows( Array<SmartPtr<IWindow>> windows )
    {
        m_windows = ConcurrentArray<SmartPtr<IWindow>>( windows.begin(), windows.end() );
    }

    auto GraphicsSystemState::createConfiguration() -> SmartPtr<IGraphicsSettings>
    {
        auto graphicsSystem = getGraphicsSystem();
        if( graphicsSystem )
        {
            return graphicsSystem->createConfiguration();
        }

        return nullptr;
    }

    auto GraphicsSystemState::configure( SmartPtr<IGraphicsSettings> config ) -> bool
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

    auto GraphicsSystemState::addSceneManager( const String &type, const String &name )
        -> SmartPtr<IGraphicsScene>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->addGraphicsScene( type, name );
    }

    auto GraphicsSystemState::getSceneManager() const -> SmartPtr<IGraphicsScene>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getGraphicsScene();
    }

    auto GraphicsSystemState::getSceneManager( const String &name ) const -> SmartPtr<IGraphicsScene>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getGraphicsScene( name );
    }

    auto GraphicsSystemState::getSceneManagerById( hash32 id ) const -> SmartPtr<IGraphicsScene>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getGraphicsSceneById( id );
    }

    auto GraphicsSystemState::getResourceGroupManager() const -> SmartPtr<IResourceGroupManager>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getResourceGroupManager();
    }

    auto GraphicsSystemState::getMaterialManager() const -> SmartPtr<IMaterialManager>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getMaterialManager();
    }

    auto GraphicsSystemState::getTextureManager() const -> SmartPtr<ITextureManager>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getTextureManager();
    }

    auto GraphicsSystemState::getInstanceManager() const -> SmartPtr<IInstanceManager>
    {
        return m_instanceManager;
    }

    auto GraphicsSystemState::getSpriteRenderer() const -> SmartPtr<ISpriteRenderer>
    {
        return m_spriteRenderer;
    }

    auto GraphicsSystemState::createRenderWindow( const String &name, u32 width, u32 height,
                                                  bool fullScreen,
                                                  const SmartPtr<Properties> &properties )
        -> SmartPtr<IWindow>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->createRenderWindow( name, width, height, fullScreen, properties );
    }

    auto GraphicsSystemState::getRenderWindow( const String &name /*= StringUtil::EmptyString */ ) const
        -> SmartPtr<IWindow>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getRenderWindow( name );
    }

    auto GraphicsSystemState::getDefaultWindow() const -> SmartPtr<IWindow>
    {
        auto graphicsSystem = getGraphicsSystem();
        return graphicsSystem->getDefaultWindow();
    }

    void GraphicsSystemState::setDefaultWindow( SmartPtr<IWindow> defaultWindow )
    {
        auto graphicsSystem = getGraphicsSystem();
        graphicsSystem->setDefaultWindow( defaultWindow );
    }

    auto GraphicsSystemState::addDeferredShadingSystem( SmartPtr<IViewport> vp )
        -> SmartPtr<IDeferredShadingSystem>
    {
        return nullptr;
    }

    void GraphicsSystemState::removeDeferredShadingSystem( SmartPtr<IViewport> vp )
    {
    }

    auto GraphicsSystemState::getDeferredShadingSystems() const
        -> Array<SmartPtr<IDeferredShadingSystem>>
    {
        return {};
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

    auto GraphicsSystemState::getStateTask() const -> Thread::Task
    {
        auto applicationManager = core::ApplicationManager::instance();
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

    auto GraphicsSystemState::getRenderTask() const -> Thread::Task
    {
        auto applicationManager = core::ApplicationManager::instance();
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

}  // namespace fb::render
