#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Actor.h>
#include <FBCore/Scene/Scene.h>
#include <FBCore/Core/FSMManager.h>
#include <FBCore/Interface/Scene/IComponentEvent.h>
#include <FBCore/Interface/Scene/IComponentEventListener.h>
#include <FBCore/Interface/Scene/ISystem.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/ITaskLock.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/LogManager.h>

#include <FBCore/Scene/Components/CollisionBox.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Scene/Components/MeshRenderer.h>
#include <FBCore/Scene/Components/Transform.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Scene/Components/UserComponent.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, SceneManager, CSharedObject<fb::scene::ISceneManager> );
        constexpr auto size = 32768;

        SceneManager::SceneManager()
        {
        }

        SceneManager::~SceneManager()
        {
            unload( nullptr );
        }

        void SceneManager::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto fsmManager = factoryManager->make_ptr<FSMManager>();
                fsmManager->load( nullptr );
                setFsmManager( fsmManager );

                m_actorLoadingStates.resize( size );
                m_fsms.resize( size );
                m_gameFSMs.resize( size );

                m_fsmListeners.resize( size );
                m_gameFsmListeners.resize( size );

                m_stateObjects.resize( size );
                m_stateListeners.resize( size );
                m_scenes.resize( size );

                //m_meshComponents.resize( size );
                //m_meshRenderers.resize( size );
                //m_transforms.resize( size );

                m_actorPreviousFlags.resize( size );
                m_actorCurrentFlags.resize( size );
                m_actorNewFlags.resize( size );

                m_actors.resize( size );

                // auto transformStateData = fb::make_ptr<TransformStateData>();
                // stateManager->setStateDataByType(transformStateData);
                // m_transformStateData = transformStateData;

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneManager::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    destroyActors();

                    m_queueProperties.clear();
                    m_dirtyActors.clear();
                    m_dirtyComponents.clear();
                    m_dirtyComponentTransforms.clear();
                    m_dirtyTransforms.clear();
                    m_dirtyActorTransforms.clear();

                    auto &gc = GarbageCollector::instance();
                    gc.update();

                    if( m_scene )
                    {
                        m_scene->unload( nullptr );
                        m_scene = nullptr;
                    }

                    gc.update();

                    m_components.clear();
                    //m_collisionBoxes.clear();
                    //m_meshComponents.clear();
                    //m_meshRenderers.clear();
                    //m_transforms.clear();

                    m_fsms.clear();
                    m_gameFSMs.clear();
                    m_gameFsmListeners.clear();

                    if( auto fsmManager = getFsmManager() )
                    {
                        fsmManager->unload( nullptr );
                        setFsmManager( nullptr );
                    }

                    gc.update();

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneManager::preUpdate()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( m_scene )
                {
                    m_scene->preUpdate();
                }

                auto sceneTask = getStateTask();
                auto task = Thread::getCurrentTask();
                if( task == sceneTask )
                {
                    if( !m_queueProperties.empty() )
                    {
                        Pair<SmartPtr<ISharedObject>, SmartPtr<Properties>> p;
                        while( m_queueProperties.try_pop( p ) )
                        {
                            try
                            {
                                auto &object = p.first;
                                if( object->isDerived<IComponent>() )
                                {
                                    auto component = fb::static_pointer_cast<IComponent>( object );
                                    component->setProperties( p.second );
                                }
                                else if( object->isDerived<IComponentEventListener>() )
                                {
                                    auto component =
                                        fb::static_pointer_cast<IComponentEventListener>( object );
                                    component->setProperties( p.second );
                                }
                            }
                            catch( std::exception &e )
                            {
                                FB_LOG_EXCEPTION( e );
                            }

                            p.first = nullptr;
                            p.second = nullptr;
                        }
                    }

                    if( !m_dirtyActorTransforms.empty() )
                    {
                        SmartPtr<ITransform> component;
                        while( m_dirtyActorTransforms.try_pop( component ) )
                        {
                            component->updateTransform();
                        }
                    }

                    if( !m_dirtyActors.empty() )
                    {
                        SmartPtr<IActor> actor;
                        while( m_dirtyActors.try_pop( actor ) )
                        {
                            auto handle = actor->getHandle();
                            auto id = handle->getInstanceId();
                            auto flags = getCurrentFlags( id );
                            auto newFlags = getNewFlags( id );
                            setCurrentFlags( id, newFlags );

                            actor->updateDirty( newFlags, flags );
                        }
                    }

                    if( !m_dirtyTransforms.empty() )
                    {
                        Array<SmartPtr<IActor>> actors;
                        actors.reserve( 1024 );

                        SmartPtr<IActor> actor;
                        while( m_dirtyTransforms.try_pop( actor ) )
                        {
                            actors.push_back( actor );
                        }

                        for( auto &actor : actors )
                        {
                            actor->updateTransform();
                        }
                    }

                    if( !m_dirtyComponents.empty() )
                    {
                        // SmartPtr<IComponent> component;
                        // while( m_dirtyComponents.try_pop( component ) )
                        //{
                        //     component->updateDirty();
                        // }
                    }

                    if( !m_dirtyComponentTransforms.empty() )
                    {
                        auto unloadedComponents = Array<SmartPtr<IComponent>>();
                        unloadedComponents.reserve( 1024 );

                        SmartPtr<IComponent> component;
                        while( m_dirtyComponentTransforms.try_pop( component ) )
                        {
                            if( component->isLoaded() )
                            {
                                component->updateTransform();
                            }
                            else
                            {
                                unloadedComponents.push_back( component );
                            }
                        }

                        for( auto &c : unloadedComponents )
                        {
                            m_dirtyComponentTransforms.push( c );
                        }
                    }

                    if( auto fsmManager = getFsmManager() )
                    {
                        fsmManager->update();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        s32 SceneManager::getLoadPriority( SmartPtr<ISharedObject> obj )
        {
            if( obj->isDerived<Mesh>() )
            {
                return 100000;
            }
            if( obj->isDerived<MeshRenderer>() )
            {
                return 95000;
            }

            return 0;
        }

        void SceneManager::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( m_scene )
            {
                m_scene->update();
            }

            auto sceneTask = getStateTask();
            auto task = Thread::getCurrentTask();
            if( task == sceneTask )
            {
                //TruckController::updateComponents();
                //StemComponent::updateComponents();
                //LSystem::updateComponents();
                UserComponent::updateComponents();
                Rigidbody::updateComponents();
                LayoutTransform::updateComponents();
            }
        }

        void SceneManager::postUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( m_scene )
            {
                m_scene->postUpdate();
            }
        }

        void SceneManager::loadScene( const String &filePath )
        {
            if( !StringUtil::isNullOrEmpty( filePath ) )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto taskManager = applicationManager->getTaskManager();

                auto renderLock = taskManager->lockTask( Thread::Task::Render );
                auto physicsLock = taskManager->lockTask( Thread::Task::Physics );

                auto scene = getCurrentScene();
                if( scene )
                {
                    auto ext = Path::getFileExtension( filePath );
                    auto scenePath = filePath;
                    if( StringUtil::isNullOrEmpty( ext ) )
                    {
                        scenePath += ".fbscene";
                    }

                    scene->clear();
                    scene->loadScene( scenePath );

                    if( applicationManager->isEditor() )
                    {
                        if( applicationManager->isPlaying() )
                        {
                            scene->play();
                        }
                        else
                        {
                            scene->edit();
                        }
                    }
                    else
                    {
                        scene->play();
                    }
                }

                if( auto stateObject = applicationManager->getStateObject() )
                {
                    stateObject->triggerEvent( IEvent::Type::Loading, sceneLoadedHash,
                                               Array<Parameter>(), this, this, nullptr );
                }
            }
        }

        SmartPtr<IFSMManager> SceneManager::getFsmManager() const
        {
            return m_fsmManager;
        }

        void SceneManager::setFsmManager( SmartPtr<IFSMManager> fsmManager )
        {
            m_fsmManager = fsmManager;
        }

        SmartPtr<IScene> SceneManager::getCurrentScene() const
        {
            return m_scene;
        }

        void SceneManager::setCurrentScene( SmartPtr<IScene> scene )
        {
            m_scene = scene;
        }

        Array<SmartPtr<IActor>> SceneManager::getActors() const
        {
            Array<SmartPtr<IActor>> actors;
            actors.reserve( m_numActors );

            for( auto actor : m_actors )
            {
                if( actor )
                {
                    actors.push_back( actor );
                }
            }

            return actors;
        }

        SmartPtr<IActor> SceneManager::createActor()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto fsmManager = getFsmManager();
                FB_ASSERT( fsmManager );

                auto actor = factoryManager->make_ptr<Actor>();

                if( auto handle = actor->getHandle() )
                {
                    handle->setInstanceId( m_numActors );

                    handle->setId( m_numActors );

                    auto uuid = StringUtil::getUUID();
                    handle->setUUID( uuid );
                }

                //auto entity = (u32)m_registry->create();
                //actor->setEntity( entity );

                actor->load( nullptr );

                FB_ASSERT( m_actors[m_numActors] == nullptr );
                m_actors[m_numActors] = actor;

                auto fsm = fsmManager->createFSM();
                auto gameFSM = fsmManager->createFSM();

                m_fsms[m_numActors] = fsm;
                m_gameFSMs[m_numActors] = gameFSM;

                auto actorFsmListener = factoryManager->make_ptr<Actor::ActorFsmListener>();
                auto actorGameFsmListener = factoryManager->make_ptr<Actor::ActorGameFsmListener>();

                m_fsmListeners[m_numActors] = actorFsmListener;
                m_gameFsmListeners[m_numActors] = actorGameFsmListener;

                actorFsmListener->setOwner( actor.get() );
                fsm->addListener( actorFsmListener );

                actorGameFsmListener->setOwner( actor.get() );
                gameFSM->addListener( actorGameFsmListener );

                auto flags = IActor::ActorFlagEnabled;
                m_actorCurrentFlags[m_numActors] = flags;

                ++m_numActors;
                return actor;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IActor> SceneManager::createDummyActor()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto fsmManager = getFsmManager();
                FB_ASSERT( fsmManager );

                auto actor = factoryManager->make_ptr<Actor>();

                auto handle = actor->getHandle();
                if( handle )
                {
                    handle->setInstanceId( m_numActors );
                    handle->setName( "Dummy" );
                }

                actor->setFlag( IActor::ActorFlagDummy, true );
                actor->load( nullptr );

                m_actors[m_numActors] = actor;

                auto fsm = fsmManager->createFSM();
                auto gameFSM = fsmManager->createFSM();

                m_fsms[m_numActors] = fsm;
                m_gameFSMs[m_numActors] = gameFSM;

                auto actorFsmListener = factoryManager->make_ptr<Actor::ActorFsmListener>();
                auto actorGameFsmListener = factoryManager->make_ptr<Actor::ActorGameFsmListener>();

                m_fsmListeners[m_numActors] = actorFsmListener;
                m_gameFsmListeners[m_numActors] = actorGameFsmListener;

                actorFsmListener->setOwner( actor.get() );
                fsm->addListener( actorFsmListener );

                actorGameFsmListener->setOwner( actor.get() );
                gameFSM->addListener( actorGameFsmListener );

                auto flags = IActor::ActorFlagEnabled;
                m_actorCurrentFlags[m_numActors] = flags;

                ++m_numActors;
                return actor;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void SceneManager::destroyActor( SmartPtr<IActor> actor )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto fsmManager = getFsmManager();
                FB_ASSERT( fsmManager );

                if( actor )
                {
                    if( auto parent = actor->getParent() )
                    {
                        parent->removeChild( actor );
                    }

                    auto actorId = 0;

                    if( auto handle = actor->getHandle() )
                    {
                        actorId = handle->getInstanceId();
                    }

                    auto fsm = m_fsms[actorId];
                    auto gameFSM = m_gameFSMs[actorId];

                    auto actorFsmListener = m_fsmListeners[actorId];
                    auto actorGameFsmListener = m_gameFsmListeners[actorId];

                    actor->unload( nullptr );

                    if( fsm )
                    {
                        if( actorFsmListener )
                        {
                            fsm->removeListener( actorFsmListener );
                            actorFsmListener = nullptr;
                        }

                        fsmManager->destroyFSM( fsm );
                        fsm = nullptr;
                    }

                    if( gameFSM )
                    {
                        if( actorGameFsmListener )
                        {
                            gameFSM->removeListener( actorGameFsmListener );
                            actorGameFsmListener = nullptr;
                        }

                        fsmManager->destroyFSM( gameFSM );
                        gameFSM = nullptr;
                    }

                    m_fsms[actorId] = nullptr;
                    m_gameFSMs[actorId] = nullptr;

                    m_fsmListeners[actorId] = nullptr;
                    m_gameFsmListeners[actorId] = nullptr;
                    m_actors[actorId] = nullptr;

                    //auto pActor = fb::static_pointer_cast<CActor>( actor );
                    //auto entity = (entt::registry::entity_type)pActor->getEntity();
                    //if( entity != (entt::registry::entity_type)0 )
                    //{
                    //    m_registry->destroy( entity );
                    //}

                    removeDirty( actor );
                    actor = nullptr;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneManager::destroyActors()
        {
            try
            {
                auto actors = getActors();
                for( auto actor : actors )
                {
                    if( actor )
                    {
                        destroyActor( actor );
                    }
                }

                m_actors.clear();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SceneManager::play()
        {
            auto scene = fb::static_pointer_cast<Scene>( getCurrentScene() );
            if( scene )
            {
                scene->play();
            }
        }

        void SceneManager::edit()
        {
            auto scene = fb::static_pointer_cast<Scene>( getCurrentScene() );
            if( scene )
            {
                scene->edit();
            }
        }

        void SceneManager::stop()
        {
            auto scene = fb::static_pointer_cast<Scene>( getCurrentScene() );
            if( scene )
            {
                scene->stop();
            }
        }

        u32 SceneManager::addTransformComponent( SmartPtr<Transform> transformComponent )
        {
            auto &transforms = getTransforms();

            transforms.push_back( transformComponent );
            return static_cast<u32>( transforms.size() - 1 );
        }

        u32 SceneManager::addComponent( SmartPtr<IComponent> component )
        {
            if( auto handle = component->getHandle() )
            {
                auto uuid = StringUtil::getUUID();
                handle->setUUID( uuid );
            }

            auto typeInfo = component->getTypeInfo();
            auto &components = m_components[typeInfo];
            components.push_back( component );
            return static_cast<u32>( components.size() - 1 );
        }

        u32 SceneManager::removeComponent( SmartPtr<IComponent> component )
        {
            auto typeInfo = component->getTypeInfo();
            auto it = m_components.find( typeInfo );
            if( it != m_components.end() )
            {
                auto &components = it->second;
                auto componentIt = std::find( components.begin(), components.end(), component );
                if( componentIt != components.end() )
                {
                    components.erase( componentIt );
                }
            }

            return 0;
        }

        SmartPtr<IActor> SceneManager::getActor( u32 id ) const
        {
            FB_ASSERT( id < m_actors.size() );
            return m_actors[id];
        }

        SmartPtr<IActor> SceneManager::getActorByName( const String &name ) const
        {
            auto actors = getActors();
            for( auto actor : actors )
            {
                if( auto handle = actor->getHandle() )
                {
                    if( handle->getName() == name )
                    {
                        return actor;
                    }
                }
            }

            return nullptr;
        }

        SmartPtr<IActor> SceneManager::getActorByFileId( const String &id ) const
        {
            auto actors = getActors();
            for( auto actor : actors )
            {
                if( auto handle = actor->getHandle() )
                {
                    if( handle->getFileId() == id )
                    {
                        return actor;
                    }
                }
            }

            return nullptr;
        }

        SmartPtr<IComponent> SceneManager::getComponent( u32 id ) const
        {
            //FB_ASSERT( id < m_components.size() );
            //auto pComponent = m_components[id];
            //return SmartPtr<IComponent>( pComponent );

            return nullptr;
        }

        SmartPtr<IFSM> SceneManager::getFSM( u32 id ) const
        {
            FB_ASSERT( id < m_fsms.size() );
            return m_fsms[id];
        }

        SmartPtr<ITransform> SceneManager::createTransformComponent()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto transform = factoryManager->make_object<ITransform>();
            FB_ASSERT( transform );

            transform->load( nullptr );

            auto &transforms = getTransforms();
            transforms.push_back( transform );
            return transform;
        }

        void SceneManager::destroyTransformComponent( SmartPtr<ITransform> transform )
        {
            if( transform )
            {
                transform->unload( nullptr );

                auto &transforms = getTransforms();

                transforms.erase( std::remove( transforms.begin(), transforms.end(), transform ),
                                  transforms.end() );
            }
        }

        SmartPtr<Transform> SceneManager::getTransform( u32 id ) const
        {
            FB_ASSERT( id < m_transforms.size() );
            return m_transforms[id];
        }

        u32 SceneManager::getCurrentFlags( u32 id ) const
        {
            FB_ASSERT( id < m_actorCurrentFlags.size() );
            return m_actorCurrentFlags[id];
        }

        void SceneManager::setCurrentFlags( u32 id, u32 flags )
        {
            FB_ASSERT( id < m_actorCurrentFlags.size() );
            m_actorCurrentFlags[id] = flags;
        }

        u32 SceneManager::getNewFlags( u32 id ) const
        {
            FB_ASSERT( id < m_actorNewFlags.size() );
            return m_actorNewFlags[id];
        }

        void SceneManager::setNewFlags( u32 id, u32 flags )
        {
            FB_ASSERT( id < m_actorNewFlags.size() );
            m_actorNewFlags[id] = flags;
        }

        u32 SceneManager::getPreviousFlags( u32 id ) const
        {
            FB_ASSERT( id < m_actorPreviousFlags.size() );
            return m_actorPreviousFlags[id];
        }

        void SceneManager::setPreviousFlags( u32 id, u32 flags )
        {
            FB_ASSERT( id < m_actorPreviousFlags.size() );
            m_actorPreviousFlags[id] = flags;
        }

        u32 SceneManager::getFlags( u32 id ) const
        {
            FB_ASSERT( id < m_actorCurrentFlags.size() );
            return m_actorCurrentFlags[id];
        }

        void SceneManager::setFlags( u32 id, u32 flags )
        {
            FB_ASSERT( id < m_actorCurrentFlags.size() );
            m_actorCurrentFlags[id] = flags;

            FB_ASSERT( id < m_actorNewFlags.size() );
            m_actorNewFlags[id] = flags;

            FB_ASSERT( id < m_actorPreviousFlags.size() );
            m_actorPreviousFlags[id] = flags;
        }

        void SceneManager::addDirty( SmartPtr<IActor> actor )
        {
            if( isLoaded() )
            {
                m_dirtyActors.push( actor );
            }
        }

        void SceneManager::removeDirty( SmartPtr<IActor> actor )
        {
            if( isLoaded() )
            {
                auto dirtyActors = Array<SmartPtr<IActor>>();
                dirtyActors.reserve( 32 );

                while( !m_dirtyActors.empty() )
                {
                    SmartPtr<IActor> dirtyActor;
                    if( m_dirtyActors.try_pop( dirtyActor ) )
                    {
                        dirtyActors.push_back( dirtyActor );
                    }
                }

                dirtyActors.erase( std::remove( dirtyActors.begin(), dirtyActors.end(), actor ),
                                   dirtyActors.end() );

                for( auto dirtyActor : dirtyActors )
                {
                    m_dirtyActors.push( dirtyActor );
                }
            }
        }

        void SceneManager::addDirtyComponent( SmartPtr<IComponent> component )
        {
            if( isLoaded() )
            {
                m_dirtyComponents.push( component );
            }
        }

        void SceneManager::addDirtyComponentTransform( SmartPtr<IComponent> component )
        {
            if( isLoaded() )
            {
                m_dirtyComponentTransforms.push( component );
            }
        }

        void SceneManager::queueProperties( SmartPtr<ISharedObject> object,
                                            SmartPtr<Properties> properties )
        {
            if( isLoaded() )
            {
                auto p = std::make_pair( object, properties );
                m_queueProperties.push( p );
            }
        }

        void SceneManager::addDirtyTransform( SmartPtr<IActor> actor )
        {
            if( isLoaded() )
            {
                m_dirtyTransforms.push( actor );
            }
        }

        void SceneManager::addDirtyTransform( SmartPtr<ITransform> transform )
        {
            if( isLoaded() )
            {
                m_dirtyActorTransforms.push( transform );
            }
        }

        Thread::Task SceneManager::getStateTask() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto threadPool = applicationManager->getThreadPool() )
            {
                if( threadPool->getNumThreads() > 0 )
                {
                    auto hasTasks = applicationManager->hasTasks();
                    return hasTasks ? Thread::Task::Application : Thread::Task::Primary;
                }
            }

            return Thread::Task::Primary;
        }

        Thread::Task SceneManager::getSceneTask() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( applicationManager->isLoading() )
            {
                return Thread::Task::Primary;
            }

            auto taskManager = applicationManager->getTaskManager();
            if( taskManager )
            {
                auto task = taskManager->getTask( Thread::Task::Application );
                if( task )
                {
                    if( task->isExecuting() )
                    {
                        return Thread::Task::Application;
                    }
                    if( task->isPrimary() )
                    {
                        return Thread::Task::Primary;
                    }
                }
            }

            return applicationManager->hasTasks() ? Thread::Task::Application : Thread::Task::Primary;
        }

        Array<SmartPtr<IComponent>> SceneManager::getComponents() const
        {
            Array<SmartPtr<IComponent>> components;
            for( auto it : m_components )
            {
                auto &c = it.second;
                components.insert( components.begin(), c.begin(), c.end() );
            }

            return components;
        }

        Array<SmartPtr<IComponent>> SceneManager::getComponents( u32 type ) const
        {
            auto it = m_components.find( type );
            if( it != m_components.end() )
            {
                return it->second;
            }

            return Array<SmartPtr<IComponent>>();
        }

        Array<SmartPtr<ITransform>> &SceneManager::getTransforms()
        {
            return m_transforms;
        }

        const Array<SmartPtr<ITransform>> &SceneManager::getTransforms() const
        {
            return m_transforms;
        }

        void SceneManager::setTransforms( const Array<SmartPtr<ITransform>> transforms )
        {
            m_transforms = transforms;
        }

        Array<String> SceneManager::getComponentFactoryIgnoreList() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_componentFactoryIgnoreList;
        }

        Map<String, String> SceneManager::getComponentFactoryMap() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_componentFactoryMap;
        }

        void SceneManager::setComponentFactoryMap( const Map<String, String> &map )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_componentFactoryMap = map;
        }

        String SceneManager::getComponentFactoryType( const String &type ) const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( std::find( m_componentFactoryIgnoreList.begin(), m_componentFactoryIgnoreList.end(),
                           type ) != m_componentFactoryIgnoreList.end() )
            {
                return String( "" );
            }

            auto it = m_componentFactoryMap.find( type );
            if( it != m_componentFactoryMap.end() )
            {
                return it->second;
            }

            return "";
        }

        void SceneManager::setComponentFactoryIgnoreList( const Array<String> &ignoreList )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_componentFactoryIgnoreList = ignoreList;
        }

    }  // namespace scene
}  // end namespace fb
