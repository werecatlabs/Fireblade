#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Scene.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/IPrefabManager.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/ITaskLock.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/DebugTrace.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Base/Path.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Scene, CSharedObject<IScene> );

        Scene::Scene()
        {
            constexpr auto size = 32768;  // todo move to scene manager
            m_actorFlags.resize( size );
        }

        Scene::~Scene()
        {
            unload( nullptr );
        }

        String Scene::getName() const
        {
            return m_name;
        }

        void Scene::setName( const String &name )
        {
            m_name = name;
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

                auto applicationManager = core::IApplicationManager::instance();
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

                //auto data = fb::make_ptr<Data<data::fb_scene>>();
                //auto sceneData = data->getDataAsType<data::fb_scene>();

                //auto scenePath = StringUtil::cleanupPath( path );
                //auto stream = fileSystem->open( scenePath, true, false, false, false, false );
                //if( !stream )
                //{
                //    stream = fileSystem->open( scenePath, true, false, false, true, true );
                //}

                //if( stream )
                //{
                //    auto name = Path::getFileNameWithoutExtension( path );
                //    setName( name );

                //    auto dataStr = stream->getAsString();
                //    DataUtil::parse( dataStr, sceneData );

                //    fromData( data );
                //}

                //auto cameraManagerResetJob = fb::make_ptr<CameraManagerReset>();
                //jobQueue->queueJob( cameraManagerResetJob );

                applicationManager->triggerEvent( IEvent::Type::Scene, IEvent::loadScene,
                                                  Array<Parameter>(), this, this, nullptr );
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
                auto pActors = fb::make_shared<Array<SmartPtr<scene::IActor>>>();
                setActorsPtr( pActors );

                for( u32 x = 0; x < static_cast<int>( Thread::UpdateState::Count ); ++x )
                {
                    for( u32 y = 0; y < static_cast<int>( Thread::Task::Count ); ++y )
                    {
                        auto actors = fb::make_shared<ConcurrentArray<SmartPtr<scene::IActor>>>();
                        setRegisteredObjects( static_cast<Thread::UpdateState>( x ),
                                              static_cast<Thread::Task>( y ), actors );
                    }
                }
            }
        }

        SharedPtr<Array<SmartPtr<scene::IActor>>> Scene::getActorsPtr() const
        {
            return m_actors;
        }

        void Scene::setActorsPtr( SharedPtr<Array<SmartPtr<scene::IActor>>> ptr )
        {
            m_actors = ptr;
        }

        void Scene::saveScene( const String &path )
        {
            // FB_ASSERT( !Path::isPathAbsolute( path ) );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            //auto data = toData();
            //FB_ASSERT( data );

            //if( data )
            //{
            //    setName( Path::getFileNameWithoutExtension( path ) );

            //    auto sceneData = data->getDataAsType<data::fb_scene>();
            //    auto dataStr = DataUtil::toString( sceneData, true );
            //    FB_ASSERT( !StringUtil::isNullOrEmpty( dataStr ) );

            //    auto scenePath = StringUtil::cleanupPath( path );
            //    FB_ASSERT( !StringUtil::isNullOrEmpty( scenePath ) );

            //    if( StringUtil::isNullOrEmpty( Path::getFileExtension( scenePath ) ) )
            //    {
            //        scenePath += ".fbscene";
            //    }

            //    fileSystem->writeAllText( scenePath, dataStr );
            //}
        }

        void Scene::saveScene()
        {
            auto filePath = getFilePath();
            FB_ASSERT( StringUtil::isNullOrEmpty( filePath ) == false );

            saveScene( filePath );
        }

        void Scene::reload( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
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

                auto &gc = GarbageCollector::instance();
                gc.update();

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

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto sceneManager = applicationManager->getSceneManager();

            auto sceneTask = sceneManager->getStateTask();
            auto task = Thread::getCurrentTask();
            if( task == sceneTask )
            {
                if( !m_editQueue.empty() )
                {
                    Array<SmartPtr<scene::IActor>> actors;
                    actors.reserve( 32 );

                    SmartPtr<scene::IActor> actor;
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
                    Array<SmartPtr<scene::IActor>> actors;
                    actors.reserve( 32 );

                    SmartPtr<scene::IActor> actor;
                    while( m_playQueue.try_pop( actor ) )
                    {
                        actors.push_back( actor );
                    }

                    for( auto actor : actors )
                    {
                        actor->setState( IActor::State::Play );
                    }
                }

                // auto actorIndex = 0;
                // for (auto& actorFlags : m_actorFlags)
                //{
                //	if ((actorFlags & CActor::ACTOR_FLAG_STARTED) == 0)
                //	{
                //		if (actorIndex < actors.size())
                //		{
                //			auto actor = actors[actorIndex];
                //			actor->start();
                //			actorFlags = actorFlags | CActor::ACTOR_FLAG_STARTED;
                //		}
                //	}

                //	actorIndex++;

                //	if (actorIndex >= actors.size())
                //	{
                //		break;
                //	}
                //}

                auto p = getRegisteredObjects( Thread::UpdateState::PreUpdate, task );
                if( p )
                {
                    auto &updateObjects = *p;
                    for( auto actor : updateObjects )
                    {
                        actor->preUpdate();
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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto p = getRegisteredObjects( Thread::UpdateState::Update, task );
                if( p )
                {
                    auto &updateObjects = *p;
                    for( auto &object : updateObjects )
                    {
                        object->update();
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

            auto applicationManager = core::IApplicationManager::instance();
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
                        actor->postUpdate();
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

        void Scene::addActor( SmartPtr<scene::IActor> actor )
        {
            try
            {
                FB_ASSERT( isValid() );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                if( actor )
                {
                    auto handle = actor->getHandle();
                    auto id = handle->getId();
                    //FB_ASSERT( id != 0 );

                    FB_ASSERT( StringUtil::isNullOrEmpty( handle->getUUID() ) == false );

                    auto actors = Array<SmartPtr<scene::IActor>>();
                    auto pActors = getActorsPtr();
                    if( pActors )
                    {
                        actors = *pActors;
                    }

                    actors.push_back( actor );
                    actor->setScene( this );

                    FB_ASSERT( std::unique( actors.begin(), actors.end() ) == actors.end() );

                    auto pNewActors = factoryManager->make_shared<Array<SmartPtr<scene::IActor>>>(
                        actors.begin(), actors.end() );
                    setActorsPtr( pNewActors );

                    auto actorIndex = handle->getInstanceId();
                    FB_ASSERT( actorIndex != std::numeric_limits<hash64>::max() );

                    m_actorFlags[actorIndex] = IActor::ActorFlagEnabled;

                    actor->setNewFlag( IActor::ActorFlagInScene, true );
                }
                else
                {
                    FB_LOG_ERROR( "CScene::addActor null" );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Scene::removeActor( SmartPtr<scene::IActor> actor )
        {
            try
            {
                FB_ASSERT( isValid() );

                auto applicationManager = core::IApplicationManager::instance();
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

                        auto pNewActors = factoryManager->make_shared<Array<SmartPtr<scene::IActor>>>();
                        *pNewActors = Array<SmartPtr<scene::IActor>>( actors.begin(), actors.end() );
                        setActorsPtr( pNewActors );
                    }

                    actor->setNewFlag( IActor::ActorFlagInScene, false );
                }
                else
                {
                    FB_LOG_ERROR( "CScene::removeActor null" );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Scene::removeAllActors()
        {
        }

        SmartPtr<scene::IActor> Scene::findActorById( int id ) const
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

        Array<SmartPtr<scene::IActor>> Scene::getActors() const
        {
            FB_ASSERT( isValid() );
            FB_ASSERT( isLoaded() );

            if( auto p = getActorsPtr() )
            {
                return *p;
            }

            return Array<SmartPtr<scene::IActor>>();
        }

        void Scene::clear()
        {
            FB_ASSERT( isValid() );
            FB_ASSERT( getLoadingState() == LoadingState::Loaded );

            auto actors = getActors();
            for( auto actor : actors )
            {
                if( actor )
                {
                    actor->unload( nullptr );
                }
            }

            auto pActorsCache = getActorsPtr();
            if( pActorsCache )
            {
                auto &actorsCache = *pActorsCache;
                actorsCache.clear();
            }

            m_actors = nullptr;
        }

        void Scene::destroyOnLoad()
        {
            FB_ASSERT( isValid() );

            ConcurrentQueue<SmartPtr<scene::IActor>> removeQueue;

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

            SmartPtr<scene::IActor> actor;
            while( removeQueue.try_pop( actor ) )
            {
                auto handle = actor->getHandle();
                auto id = handle->getId();
                removeActor( actor );
            }
        }

        void Scene::registerAllUpdates( SmartPtr<scene::IActor> actor )
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

        void Scene::registerUpdates( Thread::Task taskId, SmartPtr<scene::IActor> actor )
        {
            registerUpdate( taskId, Thread::UpdateState::PreUpdate, actor );
            registerUpdate( taskId, Thread::UpdateState::Update, actor );
            registerUpdate( taskId, Thread::UpdateState::PostUpdate, actor );
        }

        void Scene::registerUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                     SmartPtr<scene::IActor> object )
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

            // for (u32 x = 0; x < Tasks::E_UPDATE_STATE_COUNT; ++x)
            //{
            //	for (u32 y = 0; y < Tasks::ET_COUNT; ++y)
            //	{
            //		boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> p =
            // getRegisteredObjects((Thread::EUpdateState)x, (Tasks::TaskIds)y);
            //		ConcurrentArray<SmartPtr<scene::IActor>>& updateObjects = *p;
            //		std::sort(updateObjects.begin(), updateObjects.end());
            //	}
            // }
        }

        void Scene::unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                       SmartPtr<scene::IActor> object )
        {
            FB_ASSERT( isValid() );

            boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> p =
                getRegisteredObjects( updateType, taskId );
            if( p )
            {
                ConcurrentArray<SmartPtr<scene::IActor>> &updateObjects = *p;

                Array<SmartPtr<scene::IActor>> objects( updateObjects.begin(), updateObjects.end() );
                auto it = std::find( objects.begin(), objects.end(), object );
                if( it != objects.end() )
                {
                    objects.erase( it );

                    auto pNew = new ConcurrentArray<SmartPtr<scene::IActor>>();
                    boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> ptr( pNew );
                    setRegisteredObjects( updateType, taskId, ptr );
                }

                sortObjects();
            }
        }

        void Scene::unregisterAll( SmartPtr<scene::IActor> object )
        {
            FB_ASSERT( isValid() );

            for( u32 x = 0; x < static_cast<int>( Thread::UpdateState::Count ); ++x )
            {
                for( u32 y = 0; y < static_cast<int>( Thread::Task::Count ); ++y )
                {
                    boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> p = getRegisteredObjects(
                        static_cast<Thread::UpdateState>( x ), static_cast<Thread::Task>( y ) );
                    if( p )
                    {
                        ConcurrentArray<SmartPtr<scene::IActor>> &updateObjects = *p;

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
                            ConcurrentArray<SmartPtr<scene::IActor>> newArray;
                            for( unsigned int i = 0; i < updateObjects.size(); ++i )
                            {
                                if( object != updateObjects[i] )
                                {
                                    newArray.push_back( updateObjects[i] );
                                }
                            }

                            auto pNew = new ConcurrentArray<SmartPtr<scene::IActor>>( newArray.begin(),
                                                                                      newArray.end() );
                            boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> ptr( pNew );
                            setRegisteredObjects( static_cast<Thread::UpdateState>( x ),
                                                  static_cast<Thread::Task>( y ), ptr );
                            updateObjects = newArray;
                        }
                    }
                }
            }

            sortObjects();
        }

        void Scene::refreshRegistration( SmartPtr<scene::IActor> object )
        {
            // FB_ASSERT(isValid());

            // if (object)
            //{
            //	unregisterAll(object);

            //	for (u32 x = 0; x < int(Thread::EUpdateState::E_UPDATE_STATE_COUNT); ++x)
            //	{
            //		for (u32 y = 0; y < int(Thread::Task::Count); ++y)
            //		{
            //			bool bRegister = object->isRegistered((Thread::Task)y, (Thread::EUpdateState)x);
            //			if (bRegister)
            //			{
            //				registerUpdate((Thread::Task)y, (Thread::EUpdateState)x, object);
            //			}
            //		}
            //	}

            //	sortObjects();
            //}
        }

        boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> Scene::getRegisteredObjects(
            Thread::UpdateState updateState, Thread::Task task ) const
        {
            FB_ASSERT( static_cast<size_t>( updateState ) < m_updateObjects.size() );
            FB_ASSERT( static_cast<size_t>( task ) <
                       m_updateObjects[static_cast<s32>( updateState )].size() );

            const auto &ptr = m_updateObjects[static_cast<int>( updateState )][static_cast<int>( task )];
            auto objects = ptr;

            FB_ASSERT( objects );

            return objects;
        }

        void Scene::setRegisteredObjects(
            Thread::UpdateState updateState, Thread::Task task,
            boost::shared_ptr<ConcurrentArray<SmartPtr<scene::IActor>>> objects )
        {
            FB_ASSERT( static_cast<s32>( updateState ) < static_cast<s32>( m_updateObjects.size() ) );
            FB_ASSERT( static_cast<s32>( task ) <
                       static_cast<s32>( m_updateObjects[static_cast<s32>( updateState )].size() ) );
            FB_ASSERT( objects );

            auto &ptr = m_updateObjects[static_cast<s32>( updateState )][static_cast<s32>( task )];
            ptr = objects;
        }

        bool Scene::isValid() const
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

        String Scene::getFilePath() const
        {
            return m_filePath;
        }

        void Scene::setFilePath( const String &val )
        {
            m_filePath = val;
        }

        void Scene::play()
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

        void Scene::edit()
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

        void Scene::stop()
        {
        }

        void Scene::setState( State state )
        {
        }

        IScene::State Scene::getState() const
        {
            return m_state;
        }

        void Scene::setSceneLoadingState( SceneLoadingState state )
        {
            m_sceneLoadingState = state;
        }

        IScene::SceneLoadingState Scene::getSceneLoadingState() const
        {
            return m_sceneLoadingState;
        }

    }  // end namespace scene
}  // end namespace fb
