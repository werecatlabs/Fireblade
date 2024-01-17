#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Scene.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/IPrefabManager.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/ITaskLock.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/LogManager.h>
#include "FBCore/Jobs/CameraManagerReset.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Scene, Resource<IScene> );

    static bool isSceneLoaded = false;  // todo remove to use a fsm

    Scene::Scene() = default;

    Scene::~Scene()
    {
        unload( nullptr );
    }

    auto Scene::getLabel() const -> String
    {
        return m_label;
    }

    void Scene::setLabel( const String &name )
    {
        m_label = name;
    }

    void Scene::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            setupCache();

            FB_ASSERT( isValid() );
            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Scene::loadScene( const String &path )
    {
        try
        {
            setFilePath( path );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto jobQueue = applicationManager->getJobQueue();

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto taskManager = applicationManager->getTaskManager();
            FB_ASSERT( taskManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto renderLock = taskManager->lockTask( Thread::Task::Render );
            auto physicsLock = taskManager->lockTask( Thread::Task::Physics );

            timer->setTimeSinceLevelLoad( 0.0 );

            auto scenePath = StringUtil::cleanupPath( path );
            auto stream = fileSystem->open( scenePath, true, false, false, false, false );
            if( !stream )
            {
                stream = fileSystem->open( scenePath, true, false, false, true, true );
            }

            if( stream )
            {
                auto name = Path::getFileNameWithoutExtension( path );
                setLabel( name );

                auto dataStr = stream->getAsString();
                auto sceneData = fb::make_ptr<Properties>();
                DataUtil::parse( dataStr, sceneData.get() );

                fromData( sceneData );
            }

            auto applicationTask = taskManager->getTask( Thread::Task::Application );
            auto cameraManagerResetJob = fb::make_ptr<CameraManagerReset>();
            applicationTask->addJob( cameraManagerResetJob );

            applicationManager->triggerEvent( IEvent::Type::Scene, IEvent::loadScene, Array<Parameter>(),
                                              this, this, nullptr );

            isSceneLoaded = true;
            setSceneLoadingState( SceneLoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Scene::setupCache()
    {
        auto actorsPtr = getActorsPtr();
        if( !actorsPtr )
        {
            auto pActors = fb::make_shared<Array<SmartPtr<IActor>>>();
            setActorsPtr( pActors );

            for( u32 x = 0; x < static_cast<int>( Thread::UpdateState::Count ); ++x )
            {
                for( u32 y = 0; y < static_cast<int>( Thread::Task::Count ); ++y )
                {
                    auto actors = fb::make_shared<ConcurrentArray<SmartPtr<IActor>>>();
                    setRegisteredObjects( static_cast<Thread::UpdateState>( x ),
                                          static_cast<Thread::Task>( y ), actors );
                }
            }
        }
    }

    auto Scene::getActorsPtr() const -> SharedPtr<Array<SmartPtr<IActor>>>
    {
        return m_actors;
    }

    void Scene::setActorsPtr( SharedPtr<Array<SmartPtr<IActor>>> ptr )
    {
        m_actors = ptr;
    }

    void Scene::saveScene( const String &path )
    {
        // FB_ASSERT( !Path::isPathAbsolute( path ) );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto data = toData();
        FB_ASSERT( data );

        if( data )
        {
            setFilePath( path );

            auto name = Path::getFileNameWithoutExtension( path );
            setLabel( name );

            auto dataStr = DataUtil::toString( data.get(), true );
            FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            auto scenePath = StringUtil::cleanupPath( path );
            FB_ASSERT( !StringUtil::isNullOrEmpty( scenePath ) );

            if( StringUtil::isNullOrEmpty( Path::getFileExtension( scenePath ) ) )
            {
                scenePath += ".fbscene";
            }

            fileSystem->writeAllText( scenePath, dataStr );
        }
    }

    void Scene::saveScene()
    {
        auto filePath = getFilePath();
        FB_ASSERT( StringUtil::isNullOrEmpty( filePath ) == false );

        saveScene( filePath );
    }

    void Scene::reload( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto cachePath = applicationManager->getCachePath();
        auto tempScenePath = cachePath + "/tmp.fbscene";

        FB_ASSERT( !Path::isPathAbsolute( tempScenePath ) );
        saveScene( tempScenePath );
        clear();
        loadScene( tempScenePath );
    }

    void Scene::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            SmartPtr<IActor> actor;
            while( m_playQueue.try_pop( actor ) )
            {
                actor = nullptr;
            }

            while( m_editQueue.try_pop( actor ) )
            {
                actor = nullptr;
            }

            m_playQueue.clear();
            m_editQueue.clear();

            for( u32 x = 0; x < static_cast<int>( Thread::UpdateState::Count ); ++x )
            {
                for( u32 y = 0; y < static_cast<int>( Thread::Task::Count ); ++y )
                {
                    m_updateObjects[x][y] = nullptr;
                }
            }

            clearCache();

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Scene::clearCache()
    {
        m_actors = nullptr;
    }

    void Scene::preUpdate()
    {
        FB_DEBUG_TRACE;
        FB_ASSERT( isValid() );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto sceneManager = applicationManager->getSceneManager();

        auto sceneTask = sceneManager->getStateTask();
        auto task = Thread::getCurrentTask();
        if( task == sceneTask )
        {
            if( !m_editQueue.empty() )
            {
                Array<SmartPtr<IActor>> actors;
                actors.reserve( 32 );

                SmartPtr<IActor> actor;
                while( m_editQueue.try_pop( actor ) )
                {
                    actors.push_back( actor );
                }

                for( auto actor : actors )
                {
                    actor->setState( IActor::State::Edit );
                }
            }

            if( !m_playQueue.empty() )
            {
                Array<SmartPtr<IActor>> actors;
                actors.reserve( 32 );

                SmartPtr<IActor> actor;
                while( m_playQueue.try_pop( actor ) )
                {
                    actors.push_back( actor );
                }

                for( auto actor : actors )
                {
                    actor->setState( IActor::State::Play );
                }
            }

            auto p = getRegisteredObjects( Thread::UpdateState::PreUpdate, task );
            if( p )
            {
                auto &updateObjects = *p;
                for( auto actor : updateObjects )
                {
                    if( actor )
                    {
                        actor->preUpdate();
                    }
                }
            }

            FB_ASSERT( isValid() );
        }
    }

    void Scene::update()
    {
        switch( auto task = Thread::getCurrentTask() )
        {
        case Thread::Task::Application:
        {
            FB_DEBUG_TRACE;
            FB_ASSERT( isValid() );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto p = getRegisteredObjects( Thread::UpdateState::Update, task );
            if( p )
            {
                auto &updateObjects = *p;
                for( auto actor : updateObjects )
                {
                    if( actor )
                    {
                        actor->update();
                    }
                }
            }

            FB_ASSERT( isValid() );
        }
        break;
        default:
        {
        }
        }
    }

    void Scene::postUpdate()
    {
        FB_DEBUG_TRACE;
        FB_ASSERT( isValid() );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto task = Thread::getCurrentTask();
        switch( task )
        {
        case Thread::Task::Application:
        {
            auto p = getRegisteredObjects( Thread::UpdateState::PostUpdate, task );
            if( p )
            {
                auto &updateObjects = *p;
                for( auto actor : updateObjects )
                {
                    if( actor )
                    {
                        actor->postUpdate();
                    }
                }
            }

            FB_ASSERT( isValid() );

            if( isSceneLoaded )
            {
                auto actors = getActors();
                for( auto actor : actors )
                {
                    if( actor )
                    {
                        actor->levelWasLoaded( this );
                    }
                }

                isSceneLoaded = false;
            }
        }
        break;
        default:
        {
        }
        }
    }

    void Scene::addActor( SmartPtr<IActor> actor )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            FB_ASSERT( isValid() );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            if( actor )
            {
                auto handle = actor->getHandle();
                auto id = handle->getId();
                //FB_ASSERT( id != 0 );

                FB_ASSERT( StringUtil::isNullOrEmpty( handle->getUUID() ) == false );

                auto actors = Array<SmartPtr<IActor>>();
                auto pActors = getActorsPtr();
                if( pActors )
                {
                    actors = *pActors;
                }

                actors.push_back( actor );
                actor->setScene( this );

                FB_ASSERT( std::unique( actors.begin(), actors.end() ) == actors.end() );

                auto pNewActors =
                    factoryManager->make_shared<Array<SmartPtr<IActor>>>( actors.begin(), actors.end() );
                setActorsPtr( pNewActors );

                auto actorIndex = handle->getInstanceId();
                FB_ASSERT( actorIndex != std::numeric_limits<hash64>::max() );

                actor->setFlag( IActor::ActorFlagInScene, true );
            }
            else
            {
                FB_LOG_ERROR( "CScene::addActor null" );
            }

            applicationManager->triggerEvent( IEvent::Type::Scene, IEvent::addActor, Array<Parameter>(),
                                              this, actor, nullptr );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Scene::removeActor( SmartPtr<IActor> actor )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            FB_ASSERT( isValid() );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            if( actor )
            {
                auto handle = actor->getHandle();
                auto id = handle->getId();

                auto actors = getActors();
                auto it = std::find( actors.begin(), actors.end(), actor );
                if( it != actors.end() )
                {
                    actors.erase( it );

                    auto pNewActors = factoryManager->make_shared<Array<SmartPtr<IActor>>>();
                    *pNewActors = Array<SmartPtr<IActor>>( actors.begin(), actors.end() );
                    setActorsPtr( pNewActors );
                }

                actor->setFlag( IActor::ActorFlagInScene, false );
            }
            else
            {
                FB_LOG_ERROR( "CScene::removeActor null" );
            }

            applicationManager->triggerEvent( IEvent::Type::Scene, IEvent::removeActor,
                                              Array<Parameter>(), this, actor, nullptr );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Scene::removeAllActors()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if( auto pActors = getActorsPtr() )
        {
            auto &actors = *pActors;
            actors.clear();
        }
    }

    auto Scene::findActorById( int id ) const -> SmartPtr<IActor>
    {
        FB_ASSERT( getLoadingState() == LoadingState::Loaded );

        FB_ASSERT( isValid() );
        FB_ASSERT( id != 0 );

        auto actors = getActors();
        for( auto actor : actors )
        {
            auto handle = actor->getHandle();
            if( handle->getId() == id )
            {
                return actor;
            }
        }

        return nullptr;
    }

    auto Scene::getActors() const -> Array<SmartPtr<IActor>>
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( isLoaded() );

        if( auto p = getActorsPtr() )
        {
            return *p;
        }

        return {};
    }

    void Scene::clear()
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( getLoadingState() == LoadingState::Loaded );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( applicationManager->isPlaying() )
        {
            auto p = getActorsPtr();
            auto actors = *p;

            for( auto actor : actors )
            {
                if( actor )
                {
                    if( actor->getPerpetual() == false )
                    {
                        actor->unload( nullptr );
                        actors.erase( std::remove( actors.begin(), actors.end(), actor ) );
                    }
                }
            }

            p = fb::make_shared<Array<SmartPtr<IActor>>>( actors.begin(), actors.end() );
            setActorsPtr( p );
        }
        else
        {
            auto p = getActorsPtr();
            auto actors = *p;

            for( auto actor : actors )
            {
                if( actor )
                {
                    actor->unload( nullptr );
                }
            }

            p = fb::make_shared<Array<SmartPtr<IActor>>>();
            setActorsPtr( p );
        }
    }

    void Scene::destroyOnLoad()
    {
        FB_ASSERT( isValid() );

        ConcurrentQueue<SmartPtr<IActor>> removeQueue;

        auto actors = getActors();
        for( auto actor : actors )
        {
            if( actor )
            {
                if( actor->getPerpetual() )
                {
                    actor->unload( nullptr );
                    removeQueue.push( actor );
                }
            }
        }

        SmartPtr<IActor> actor;
        while( removeQueue.try_pop( actor ) )
        {
            auto handle = actor->getHandle();
            auto id = handle->getId();
            removeActor( actor );
        }
    }

    void Scene::registerAllUpdates( SmartPtr<IActor> actor )
    {
        try
        {
            FB_ASSERT( isValid() );
            FB_ASSERT( getLoadingState() == LoadingState::Loaded );

            for( u32 x = 0; x < static_cast<u32>( Thread::Task::Count ); ++x )
            {
                for( u32 y = 0; y < static_cast<u32>( Thread::UpdateState::Count ); ++y )
                {
                    registerUpdate( static_cast<Thread::Task>( x ),
                                    static_cast<Thread::UpdateState>( y ), actor );
                }
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Scene::registerUpdates( Thread::Task taskId, SmartPtr<IActor> actor )
    {
        registerUpdate( taskId, Thread::UpdateState::PreUpdate, actor );
        registerUpdate( taskId, Thread::UpdateState::Update, actor );
        registerUpdate( taskId, Thread::UpdateState::PostUpdate, actor );
    }

    void Scene::registerUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                SmartPtr<IActor> object )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( getLoadingState() == LoadingState::Loaded );

        auto p = getRegisteredObjects( updateType, taskId );
        auto &updateObjects = *p;
        auto it = std::find( updateObjects.begin(), updateObjects.end(), object );

        if( it == updateObjects.end() )
        {
            updateObjects.push_back( object );
            sortObjects();
        }
    }

    void Scene::sortObjects()
    {
        FB_ASSERT( isValid() );

        for( u32 x = 0; x < static_cast<u32>( Thread::UpdateState::Count ); ++x )
        {
            for( u32 y = 0; y < static_cast<u32>( Thread::Task::Count ); ++y )
            {
                auto p = getRegisteredObjects( static_cast<Thread::UpdateState>( x ),
                                               static_cast<Thread::Task>( y ) );
                ConcurrentArray<SmartPtr<scene::IActor>> &updateObjects = *p;
                std::sort( updateObjects.begin(), updateObjects.end() );
            }
        }
    }

    void Scene::unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                  SmartPtr<IActor> object )
    {
        FB_ASSERT( isValid() );

        boost::shared_ptr<ConcurrentArray<SmartPtr<IActor>>> p =
            getRegisteredObjects( updateType, taskId );
        if( p )
        {
            ConcurrentArray<SmartPtr<IActor>> &updateObjects = *p;

            Array<SmartPtr<IActor>> objects( updateObjects.begin(), updateObjects.end() );
            auto it = std::find( objects.begin(), objects.end(), object );
            if( it != objects.end() )
            {
                objects.erase( it );

                auto pNew = new ConcurrentArray<SmartPtr<IActor>>();
                boost::shared_ptr<ConcurrentArray<SmartPtr<IActor>>> ptr( pNew );
                setRegisteredObjects( updateType, taskId, ptr );
            }

            sortObjects();
        }
    }

    void Scene::unregisterAll( SmartPtr<IActor> object )
    {
        FB_ASSERT( isValid() );

        for( u32 x = 0; x < static_cast<int>( Thread::UpdateState::Count ); ++x )
        {
            for( u32 y = 0; y < static_cast<int>( Thread::Task::Count ); ++y )
            {
                boost::shared_ptr<ConcurrentArray<SmartPtr<IActor>>> p = getRegisteredObjects(
                    static_cast<Thread::UpdateState>( x ), static_cast<Thread::Task>( y ) );
                if( p )
                {
                    ConcurrentArray<SmartPtr<IActor>> &updateObjects = *p;

                    bool hasElement = false;
                    auto it = updateObjects.begin();
                    for( ; it != updateObjects.end(); ++it )
                    {
                        if( ( *it ) == object )
                        {
                            hasElement = true;
                            break;
                        }
                    }

                    if( hasElement )
                    {
                        ConcurrentArray<SmartPtr<IActor>> newArray;
                        for( const auto &updateObject : updateObjects )
                        {
                            if( object != updateObject )
                            {
                                newArray.push_back( updateObject );
                            }
                        }

                        auto pNew =
                            new ConcurrentArray<SmartPtr<IActor>>( newArray.begin(), newArray.end() );
                        boost::shared_ptr<ConcurrentArray<SmartPtr<IActor>>> ptr( pNew );
                        setRegisteredObjects( static_cast<Thread::UpdateState>( x ),
                                              static_cast<Thread::Task>( y ), ptr );
                        updateObjects = newArray;
                    }
                }
            }
        }

        sortObjects();
    }

    auto Scene::getRegisteredObjects( Thread::UpdateState updateState, Thread::Task task ) const
        -> boost::shared_ptr<ConcurrentArray<SmartPtr<IActor>>>
    {
        FB_ASSERT( static_cast<size_t>( updateState ) < m_updateObjects.size() );
        FB_ASSERT( static_cast<size_t>( task ) <
                   m_updateObjects[static_cast<s32>( updateState )].size() );

        const auto &ptr = m_updateObjects[static_cast<int>( updateState )][static_cast<int>( task )];
        auto objects = ptr;

        FB_ASSERT( objects );

        return objects;
    }

    void Scene::setRegisteredObjects( Thread::UpdateState updateState, Thread::Task task,
                                      boost::shared_ptr<ConcurrentArray<SmartPtr<IActor>>> objects )
    {
        FB_ASSERT( static_cast<s32>( updateState ) < static_cast<s32>( m_updateObjects.size() ) );
        FB_ASSERT( static_cast<s32>( task ) <
                   static_cast<s32>( m_updateObjects[static_cast<s32>( updateState )].size() ) );
        FB_ASSERT( objects );

        auto &ptr = m_updateObjects[static_cast<s32>( updateState )][static_cast<s32>( task )];
        ptr = objects;
    }

    auto Scene::isValid() const -> bool
    {
        auto updateObjects = m_updateObjects;

        bool result = true;
        for( u32 x = 0; x < static_cast<int>( Thread::UpdateState::Count ); ++x )
        {
            for( u32 y = 0; y < static_cast<int>( Thread::Task::Count ); ++y )
            {
                auto &p = updateObjects[x][y];

                auto ptr = p;
                if( ptr == nullptr )
                {
                    result = false;
                }
            }
        }

        return result;
    }

    auto Scene::toData() const -> SmartPtr<ISharedObject>
    {
        auto properties = fb::make_ptr<Properties>();

        auto lightingData = String();
        properties->setProperty( "lighting", lightingData );

        auto actors = getActors();
        for( auto actor : actors )
        {
            if( actor )
            {
                auto pActorData = fb::static_pointer_cast<Properties>( actor->toData() );
                pActorData->setName( "actors" );
                properties->addChild( pActorData );
            }
        }

        return properties;
    }

    void Scene::fromData( SmartPtr<ISharedObject> data )
    {
        auto properties = fb::static_pointer_cast<Properties>( data );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        auto graphicsScene = graphicsSystem->getGraphicsScene();
        FB_ASSERT( graphicsScene );

        //graphicsSceneManager->setAmbientLight( ColourF::White * 0.5f );
        //graphicsSceneManager->setEnableShadows( true );

        setupCache();

        auto actorsData = properties->getChildrenByName( "actors" );
        for( auto actorData : actorsData )
        {
            auto actor = prefabManager->loadActor( actorData, nullptr );
            addActor( actor );
        }
    }

    void Scene::setState( State state )
    {
        m_state = state;

        switch( m_state )
        {
        case State::None:
        {
        }
        break;
        case State::Edit:
        {
            if( auto pActors = getActorsPtr() )
            {
                auto &actors = *pActors;
                for( auto actor : actors )
                {
                    m_editQueue.push( actor );
                }
            }
        }
        break;
        case State::Play:
        {
            if( auto pActors = getActorsPtr() )
            {
                auto &actors = *pActors;
                for( auto actor : actors )
                {
                    m_playQueue.push( actor );
                }
            }
        }
        break;
        case State::Reset:
        {
        }
        break;
        default:
        {
        }
        break;
        }
    }

    auto Scene::getState() const -> IScene::State
    {
        return m_state;
    }

    void Scene::setSceneLoadingState( SceneLoadingState state )
    {
        m_sceneLoadingState = state;
    }

    auto Scene::getSceneLoadingState() const -> IScene::SceneLoadingState
    {
        return m_sceneLoadingState;
    }
}  // namespace fb::scene
