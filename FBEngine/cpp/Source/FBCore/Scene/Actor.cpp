#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Actor.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Scene/Components/Transform.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/Util.h>
#include <FBCore/Core/Properties.h>
#include <rttr/registration>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, Actor, CResource<IActor> );
        u32 Actor::m_idExt = 0;

        Actor::Actor()
        {
            setResourceType( ResourceType::Actor );

            m_children = fb::make_shared<ConcurrentArray<SmartPtr<IActor>>>();
            m_components = fb::make_shared<ConcurrentArray<SmartPtr<IComponent>>>();
        }

        Actor::~Actor()
        {
            unload( nullptr );
        }

        String Actor::getName() const
        {
            auto handle = getHandle();
            FB_ASSERT( handle );

            return handle->getName();
        }

        void Actor::setName( const String &name )
        {
            auto handle = getHandle();
            FB_ASSERT( handle );

            handle->setName( name );
        }

        Transform3<real_Num> Actor::getWorldTransform( time_interval t ) const
        {
            if( auto transform = getTransform() )
            {
                return transform->getWorldTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> Actor::getWorldTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getWorldTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> Actor::getLocalTransform( time_interval t ) const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> Actor::getLocalTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalTransform();
            }

            return Transform3<real_Num>();
        }

        Vector3<real_Num> Actor::getLocalPosition() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void Actor::setLocalPosition( const Vector3<real_Num> &localPosition )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalPosition( localPosition );
            }
        }

        Vector3<real_Num> Actor::getLocalScale() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getLocalScale();
            }

            return Vector3<real_Num>::zero();
        }

        void Actor::setLocalScale( const Vector3<real_Num> &localScale )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setLocalScale( localScale );
            }
        }

        Quaternion<real_Num> Actor::getLocalOrientation() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getLocalOrientation();
            }

            return Quaternion<real_Num>::identity();
        }

        void Actor::setLocalOrientation( const Quaternion<real_Num> &localOrientation )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setLocalOrientation( localOrientation );
            }
        }

        Vector3<real_Num> Actor::getPosition() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void Actor::lookAt( const Vector3<real_Num> &position )
        {
            auto vec = position - getPosition();
            auto rot = MathUtil<real_Num>::getRotationTo( -Vector3<real_Num>::unitZ(), vec );
            setOrientation( rot );
        }

        void Actor::lookAt( const Vector3<real_Num> &position, const Vector3<real_Num> &yawAxis )
        {
            auto vec = position - getPosition();
            auto rot = MathUtil<real_Num>::getOrientationFromDirection( vec, -Vector3<real_Num>::unitZ(),
                                                                        true, yawAxis );
            setOrientation( rot );
        }

        void Actor::setPosition( const Vector3<real_Num> &position )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setPosition( position );
            }
        }

        Vector3<real_Num> Actor::getScale() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void Actor::setScale( const Vector3<real_Num> &scale )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setPosition( scale );
            }
        }

        Quaternion<real_Num> Actor::getOrientation() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getOrientation();
            }

            return Quaternion<real_Num>::identity();
        }

        void Actor::setOrientation( const Quaternion<real_Num> &orientation )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setOrientation( orientation );
            }
        }

        void Actor::awake()
        {
            try
            {
                auto components = getComponents();
                for( auto component : components )
                {
                    if( component )
                    {
                        auto c = fb::static_pointer_cast<Component>( component );
                        c->awake();
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        child->awake();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::play()
        {
            try
            {
                auto components = getComponents();

                //std::sort(
                //    components.begin(), components.end(),
                //    []( const SmartPtr<ISharedObject> &a, const SmartPtr<ISharedObject> &b ) -> bool {
                //        auto aPriority = ApplicationUtil::getEventPriority( a );
                //        auto bPriority = ApplicationUtil::getEventPriority( b );

                //        return aPriority > bPriority;
                //    } );

                for( auto component : components )
                {
                    if( component )
                    {
                        auto c = fb::static_pointer_cast<Component>( component );
                        c->play();
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        child->play();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::edit()
        {
            try
            {
                if( auto p = getComponentsPtr() )
                {
                    auto &components = *p;
                    for( auto component : components )
                    {
                        if( component )
                        {
                            component->edit();
                        }
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        if( child )
                        {
                            child->edit();
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::levelWasLoaded( SmartPtr<IScene> scene )
        {
            try
            {
                auto components = getComponents();
                for( auto component : components )
                {
                    if( component )
                    {
                        auto c = fb::static_pointer_cast<Component>( component );
                        c->levelWasLoaded( 0 );
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        child->levelWasLoaded( scene );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::reset()
        {
            try
            {
                auto components = getComponents();
                for( auto component : components )
                {
                    if( component )
                    {
                        auto c = fb::static_pointer_cast<Component>( component );
                        c->reset();
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        child->reset();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::hierarchyChanged()
        {
            try
            {
                auto components = getComponents();
                for( auto component : components )
                {
                    if( component )
                    {
                        auto c = fb::static_pointer_cast<Component>( component );
                        c->hierarchyChanged();
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto child : children )
                    {
                        child->hierarchyChanged();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::childAdded( SmartPtr<IActor> child )
        {
            try
            {
                auto components = getComponents();
                for( auto component : components )
                {
                    if( component )
                    {
                        auto c = fb::static_pointer_cast<Component>( component );
                        c->childAdded( child );
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto child : children )
                    {
                        child->childAdded( child );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::childRemoved( SmartPtr<IActor> child )
        {
            try
            {
                auto components = getComponents();
                for( auto component : components )
                {
                    if( component )
                    {
                        auto c = fb::static_pointer_cast<Component>( component );
                        c->childRemoved( child );
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto child : children )
                    {
                        child->childRemoved( child );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        SmartPtr<T> Actor::removeComponents()
        {
            // int iClassType = Object::getClassType<scalar_num>();
            // std::unordered_map<int, Array<SmartPtr<IComponent>>>::iterator it =
            // m_componentsByType.find(iClassType); if (it != m_componentsByType.end())
            //{
            //	Array<SmartPtr<IComponent>>& components = it->second;
            //	if (!components.empty())
            //	{
            //		Array<SmartPtr<IComponent>>::iterator componentIt = components.begin();
            //		SmartPtr<scalar_num> component = (*componentIt);
            //		components.erase(componentIt);
            //		rebuildComponentCache();
            //		return component;
            //	}
            // }

            return nullptr;
        }

        void Actor::preUpdate()
        {
            preUpdateDirtyComponents();

            auto components = getComponents();
            for( auto &c : components )
            {
                if( c )
                {
                    c->preUpdate();
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        child->preUpdate();
                    }
                }
            }
        }

        SharedPtr<ConcurrentArray<SmartPtr<IComponent>>> Actor::getComponentsPtr() const
        {
            return m_components;
        }

        void Actor::updateDirty( u32 flags, u32 oldFlags )
        {
            auto components = getComponents();
            for( auto &component : components )
            {
                if( component )
                {
                    component->updateDirty( flags, oldFlags );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    if( child )
                    {
                        child->updateDirty( flags, oldFlags );
                    }
                }
            }
        }

        void Actor::update()
        {
            updateDirtyComponents();

            auto components = getComponents();
            for( auto &c : components )
            {
                if( c )
                {
                    c->update();
                }
            }
        }

        void Actor::postUpdate()
        {
            postUpdateDirtyComponents();

            auto components = getComponents();
            for( auto &c : components )
            {
                if( c )
                {
                    c->postUpdate();
                }
            }
        }

        void Actor::addComponentInstance( SmartPtr<IComponent> component )
        {
            try
            {
                FB_ASSERT( component );

                component->setActor( this );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto id = sceneManager->addComponent( component );

                FB_ASSERT( id != std::numeric_limits<u32>::max() );

                auto handle = component->getHandle();
                if( handle )
                {
                    handle->setInstanceId( id );
                }

                if( auto p = getComponentsPtr() )
                {
                    auto &components = *p;
                    components.push_back( component );
                }

                component->awake();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::removeComponentInstance( SmartPtr<IComponent> component )
        {
            FB_ASSERT( component );

            auto components = getComponents();
            auto it = std::find( components.begin(), components.end(), component );
            if( it != components.end() )
            {
                components.erase( it );
            }

            *m_components =
                ConcurrentArray<SmartPtr<IComponent>>( components.begin(), components.end() );

            component->unload( nullptr );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            sceneManager->removeComponent( component );
        }

        bool Actor::hasComponent( int handle )
        {
            auto components = getComponents();
            for( auto component : components )
            {
                auto pHandle = component->getHandle();
                auto componentId = pHandle->getId();

                if( componentId == handle )
                {
                    return true;
                }
            }

            return false;
        }

        SmartPtr<IComponent> Actor::getComponent( int handle ) const
        {
            // for (auto component : m_components)
            //{
            //	auto pHandle = component->getHandle();
            //	auto componentId = pHandle->getId();

            //	if (componentId == handle)
            //	{
            //		return component;
            //	}
            //}

            return nullptr;
        }

        bool Actor::isMine() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagMine );
        }

        void Actor::setMine( bool val )
        {
            // m_isMine = val;
        }

        bool Actor::isStatic() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagStatic );
        }

        void Actor::setStatic( bool isstatic )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );
            FB_ASSERT( sceneManager->isLoaded() );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getNewFlags( id );

            if( BitUtil::getFlagValue( flags, ActorFlagStatic ) != isstatic )
            {
                auto newFlags = BitUtil::setFlagValue( flags, ActorFlagStatic, isstatic );
                sceneManager->setNewFlags( id, newFlags );
                sceneManager->addDirty( this );
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    child->setStatic( isstatic );
                }
            }
        }

        bool Actor::isEnabledInScene() const
        {
            auto enabledInScene = isEnabled();
            if( enabledInScene )
            {
                auto parent = getParent();
                while( parent )
                {
                    if( !parent->isEnabled() )
                    {
                        return false;
                    }

                    parent = parent->getParent();
                }
            }

            return enabledInScene;
        }

        bool Actor::isEnabled() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagEnabled );
        }

        void Actor::setEnabled( bool enabled )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );
            FB_ASSERT( sceneManager->isLoaded() );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getNewFlags( id );

            if( BitUtil::getFlagValue( flags, ActorFlagEnabled ) != enabled )
            {
                auto newFlags = BitUtil::setFlagValue( flags, ActorFlagEnabled, enabled );
                sceneManager->setNewFlags( id, newFlags );

                sceneManager->addDirty( this );

                auto children = getAllChildren();
                for( auto &child : children )
                {
                    sceneManager->addDirty( child );
                }
            }

            updateVisibility();
        }

        bool Actor::isVisible() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagVisible );
        }

        void Actor::setVisible( bool visible, bool cacade )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );
            FB_ASSERT( sceneManager->isLoaded() );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getNewFlags( id );
            if( BitUtil::getFlagValue( flags, ActorFlagVisible ) != visible )
            {
                auto newFlags = BitUtil::setFlagValue( flags, ActorFlagVisible, visible );
                sceneManager->setNewFlags( id, newFlags );

                if( cacade )
                {
                    if( auto p = getChildrenPtr() )
                    {
                        auto &children = *p;
                        for( auto child : children )
                        {
                            child->setVisible( visible );
                        }
                    }
                }
            }
        }

        bool Actor::isDirty() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagDirty );
        }

        void Actor::setDirty( bool dirty )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );
            FB_ASSERT( sceneManager->isLoaded() );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            auto newFlags = BitUtil::setFlagValue( flags, ActorFlagDirty, dirty );
            sceneManager->setNewFlags( id, newFlags );
        }

        bool Actor::isValid() const
        {
            return true;
        }

        void Actor::updateTransform()
        {
            if( auto transform = getTransform() )
            {
                transform->updateTransform();
            }

            auto components = getComponents();
            for( auto component : components )
            {
                component->updateTransform();
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    child->updateTransform();
                }
            }
        }

        Array<SmartPtr<IComponent>> Actor::getComponents() const
        {
            if( auto p = getComponentsPtr() )
            {
                auto &components = *p;
                return Array<SmartPtr<IComponent>>( components.begin(), components.end() );
            }

            return Array<SmartPtr<IComponent>>();
        }

        void Actor::updateComponents()
        {
            // for (auto component : m_components)
            //{
            //	component->update();
            // }
        }

        void Actor::rebuildComponentCache()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // auto factoryManager = applicationManager->getFactoryManager();

            // Array<SmartPtr<IComponent>> componentCache;
            // componentCache.reserve(m_components.size());

            // for (auto component : m_components)
            //{
            //	if (component)
            //	{
            //		componentCache.push_back(component);
            //	}
            // }

            // auto p = boost::make_shared<ConcurrentArray<SmartPtr<IComponent>>>();
            //*p = ConcurrentArray<SmartPtr<IComponent>>(componentCache.begin(),
            // componentCache.end()); m_componentCache = p;
        }

        void Actor::preUpdateDirtyComponent( SmartPtr<IComponent> &component )
        {
            if( component )
            {
                auto task = Thread::getCurrentTask();

                // component->preUpdateDirty();

                // auto& dirtySibling =
                // component->getDirtySibling(Thread::UpdateState::PreUpdate, task);
                // preUpdateDirtyComponent(dirtySibling);

                // SmartPtr<IComponent> nullComponent = nullptr;
                // component->setDirtySibling(Thread::UpdateState::PreUpdate, task,
                // nullComponent);
            }
        }

        void Actor::updateDirtyComponent( SmartPtr<IComponent> &component )
        {
            if( component )
            {
                auto task = Thread::getCurrentTask();

                // component->updateDirty();

                // auto& dirtySibling =
                // component->getDirtySibling(Thread::UpdateState::Update, task);
                // updateDirtyComponent(dirtySibling);

                // SmartPtr<IComponent> nullComponent = nullptr;
                // component->setDirtySibling(Thread::UpdateState::Update, task,
                // nullComponent);
            }
        }

        void Actor::postUpdateDirtyComponent( SmartPtr<IComponent> &component )
        {
            if( component )
            {
                auto task = Thread::getCurrentTask();

                // component->postUpdateDirty();

                // auto& dirtySibling =
                // component->getDirtySibling(Thread::UpdateState::PostUpdate, task);
                // postUpdateDirtyComponent(dirtySibling);

                // SmartPtr<IComponent> nullComponent = nullptr;
                // component->setDirtySibling(Thread::UpdateState::PostUpdate, task,
                // nullComponent);
            }
        }

        IFSM::ReturnType Actor::handleActorEvent( u32 state, IFSM::Event eventType )
        {
            switch( eventType )
            {
            case IFSM::Event::Change:
                break;
            case IFSM::Event::Enter:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Create:
                case State::Destroyed:
                case State::Edit:
                {
                    bool cascade = getParent() == nullptr;
                    auto transform = getTransform();
                    if( transform )
                    {
                        transform->setDirty( true, cascade );
                    }

                    auto components = getComponents();
                    for( auto component : components )
                    {
                        component->setState( IComponent::State::Edit );
                    }
                }
                break;
                case State::Play:
                {
                    bool cascade = getParent() == nullptr;
                    auto transform = getTransform();
                    if( transform )
                    {
                        transform->setDirty( true, cascade );
                    }

                    auto components = getComponents();
                    for( auto component : components )
                    {
                        component->setState( IComponent::State::Play );
                    }
                }
                break;
                default:
                {
                }
                break;
                }
            }
            break;
            case IFSM::Event::Leave:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Create:
                case State::Destroyed:
                case State::Edit:
                case State::Play:
                {
                }
                break;
                default:
                {
                }
                break;
                }
            }
            break;
            case IFSM::Event::Pending:
                break;
            case IFSM::Event::Complete:
                break;
            case IFSM::Event::NewState:
                break;
            case IFSM::Event::WaitForChange:
                break;
            case IFSM::Event::Count:
                break;
            default:
                break;
            }

            return IFSM::ReturnType::Ok;
        }

        IFSM::ReturnType Actor::handleActorGameEvent( u32 state, IFSM::Event eventType )
        {
            switch( eventType )
            {
            case IFSM::Event::Change:
                break;
            case IFSM::Event::Enter:
                break;
            case IFSM::Event::Leave:
                break;
            case IFSM::Event::Pending:
                break;
            case IFSM::Event::Complete:
                break;
            case IFSM::Event::NewState:
                break;
            case IFSM::Event::WaitForChange:
                break;
            default:
                break;
            }

            return IFSM::ReturnType::Ok;
        }

        void Actor::preUpdateDirtyComponents()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // FB_ASSERT(applicationManager);

            // auto task = Thread::getCurrentTask();

            // auto& component = m_dirtyComponent[(int)Thread::UpdateState::PreUpdate][(int)task];
            // if (component)
            //{
            //	preUpdateDirtyComponent(component);
            //	component = nullptr;
            // }

            // auto& lastDirtyComponent =
            // m_lastDirtyComponent[(int)Thread::UpdateState::PreUpdate][(int)task]; lastDirtyComponent =
            // nullptr;
        }

        void Actor::updateDirtyComponents()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // FB_ASSERT(applicationManager);

            // auto task = Thread::getCurrentTask();

            // auto& component = m_dirtyComponent[(int)Thread::UpdateState::Update][(int)task];
            // if (component)
            //{
            //	updateDirtyComponent(component);
            //	component = nullptr;
            // }

            // auto& lastDirtyComponent = m_lastDirtyComponent[(int)Thread::UpdateState::Update][(int)task];
            // lastDirtyComponent = nullptr;
        }

        void Actor::postUpdateDirtyComponents()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // FB_ASSERT(applicationManager);

            // auto task = Thread::getCurrentTask();

            // auto& component = m_dirtyComponent[(int)Thread::UpdateState::PostUpdate][(int)task];
            // if (component)
            //{
            //	postUpdateDirtyComponent(component);
            //	component = nullptr;
            // }

            // auto& lastDirtyComponent =
            // m_lastDirtyComponent[(int)Thread::UpdateState::PostUpdate][(int)task]; lastDirtyComponent =
            // nullptr;
        }

        bool Actor::compareTag( const String &tag ) const
        {
            // auto it = std::find(m_tags.begin(), m_tags.end(), tag);
            // if (it != m_tags.end())
            //{
            //	return true;
            // }

            return false;
        }

        SmartPtr<IActor> Actor::getSceneRoot() const
        {
            auto parent = getParent();
            while( parent )
            {
                auto root = parent->getSceneRoot();
                if( root == nullptr )
                {
                    return parent;
                }

                parent = root;
            }

            return nullptr;
        }

        u32 Actor::getSceneLevel() const
        {
            auto sceneLevel = 0;
            auto parent = getParent();
            while( parent )
            {
                auto root = parent->getSceneRoot();
                if( root == nullptr )
                {
                    return sceneLevel;
                }

                ++sceneLevel;
                parent = root;
            }

            return sceneLevel;
        }

        SmartPtr<ITransform> Actor::getTransform() const
        {
            auto p = m_transform.load();
            return p.lock();
        }

        void Actor::setTransform( SmartPtr<ITransform> transform )
        {
            m_transform = transform;
        }

        void Actor::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto isDummy = getFlag( ActorFlagDummy );
                if( !isDummy )
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto pSceneManager = applicationManager->getSceneManager();
                    auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
                    if( sceneManager )
                    {
                        FB_ASSERT( sceneManager->isLoaded() );

                        auto transform = sceneManager->createTransformComponent();
                        setTransform( transform );

                        if( transform )
                        {
                            transform->setActor( this );
                        }
                    }

                    setEnabled( true );
                    setVisible( true );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::reload( SmartPtr<ISharedObject> data )
        {
        }

        void Actor::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto isDummy = getFlag( ActorFlagDummy );
                    if( !isDummy )
                    {
                        auto applicationManager = core::IApplicationManager::instance();
                        FB_ASSERT( applicationManager );

                        auto pSceneManager = applicationManager->getSceneManager();
                        auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

                        if( auto transform = getTransform() )
                        {
                            transform->unload( nullptr );
                        }

                        if( auto p = getChildrenPtr() )
                        {
                            auto &children = *p;
                            for( auto child : children )
                            {
                                if( child )
                                {
                                    child->unload( data );
                                }
                            }
                        }

                        auto components = getComponents();
                        for( auto component : components )
                        {
                            try
                            {
                                if( component )
                                {
                                    component->unload( nullptr );
                                }
                            }
                            catch( std::exception &e )
                            {
                                FB_LOG_EXCEPTION( e );
                            }
                        }

                        m_scene = nullptr;
                        m_parent = nullptr;
                        m_transform = nullptr;

                        if( auto p = getChildrenPtr() )
                        {
                            auto &children = *p;
                            children.clear();
                        }

                        m_components->clear();

                        // for (u32 x = 0; x < int(Thread::UpdateState::Count); ++x)
                        //{
                        //	for (u32 y = 0; y < int(Thread::Task::Count); ++y)
                        //	{
                        //		m_updateComponents[x][y] = nullptr;
                        //	}
                        // }

                        if( auto scene = getScene() )
                        {
                            auto pThis = getSharedFromThis<Actor>();
                            scene->unregisterAll( pThis );
                        }

                        setScene( nullptr );

                        if( sceneManager )
                        {
                            auto transform = getTransform();
                            sceneManager->destroyTransformComponent( transform );
                            setTransform( nullptr );
                        }

                        // m_componentCache = nullptr;
                        // m_parent = nullptr;

                        for( auto component : components )
                        {
                            if( component )
                            {
                                auto c = fb::static_pointer_cast<Component>( component );
                                if( c )
                                {
                                    c->actorUnload();
                                }
                            }
                        }
                    }

                    setStateObject( nullptr );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        //
        // Vector3<real_Num> CActor::getPosition() const
        //{
        //	return Vector3<real_Num>::zero();
        // }

        //
        // void CActor::setPosition(const Vector3<real_Num>& val)
        //{

        //}

        //
        // Quaternion<real_Num> CActor::getOrientation() const
        //{
        //	return Quaternion<real_Num>::identity();
        // }

        //
        // void CActor::setOrientation(const Quaternion<real_Num>& val)
        //{

        //}

        //
        // SaracenComponentData CActor::getComponentDataAsFromSceneNode(const SmartPtr<IStateNode>&
        // sceneNode)
        //{
        //	SaracenComponentData componentData;

        //	if (sceneNode)
        //	{
        //		componentData.hashName = sceneNode->getHashName();

        //		Vector3<real_Num> position = sceneNode->_getDerivedPosition();
        //		Quaternion<real_Num> orientation = sceneNode->_getDerivedOrientation();

        //		componentData.worldTransform.position.x = position.X();
        //		componentData.worldTransform.position.y = position.Y();
        //		componentData.worldTransform.position.z = position.Z();
        //		componentData.worldTransform.position.w = 0.0f;

        //		componentData.worldTransform.orientation.x = orientation.X();
        //		componentData.worldTransform.orientation.y = orientation.Y();
        //		componentData.worldTransform.orientation.z = orientation.Z();
        //		componentData.worldTransform.orientation.w = orientation.W();

        //		Vector3<real_Num> localPosition = sceneNode->getPosition();
        //		Quaternion<real_Num> localOrientation = sceneNode->getOrientation();

        //		componentData.localTransform.position.x = localPosition.X();
        //		componentData.localTransform.position.y = localPosition.Y();
        //		componentData.localTransform.position.z = localPosition.Z();
        //		componentData.localTransform.position.w = 0.0f;

        //		componentData.localTransform.orientation.x = localOrientation.X();
        //		componentData.localTransform.orientation.y = localOrientation.Y();
        //		componentData.localTransform.orientation.z = localOrientation.Z();
        //		componentData.localTransform.orientation.w = localOrientation.W();
        //	}

        //	return componentData;
        //}

        bool Actor::getPerpetual() const
        {
            return m_destroyOnLoad;
        }

        void Actor::setPerpetual( bool destroyOnLoad )
        {
            m_destroyOnLoad = destroyOnLoad;
        }

        bool Actor::getAutoUpdateComponents() const
        {
            return m_autoUpdateComponents;
        }

        void Actor::setAutoUpdateComponents( bool autoUpdateComponents )
        {
            m_autoUpdateComponents = autoUpdateComponents;
        }

        SmartPtr<IScene> Actor::getScene() const
        {
            auto p = m_scene.load();
            return p.lock();
        }

        void Actor::setScene( SmartPtr<IScene> scene )
        {
            m_scene = scene;
        }

        bool Actor::isRegistered( Thread::Task taskId, Thread::UpdateState updateType ) const
        {
            // return m_updateObjects[(int)updateType][(int)taskId] ? true : false;
            return false;
        }

        void Actor::registerUpdate( Thread::Task taskId, Thread::UpdateState updateType )
        {
            // m_updateObjects[(int)updateType][(int)taskId] = true;

            // if (m_scene)
            //{
            //	auto pThis = getSharedFromThis<CActor>();
            //	m_scene->refreshRegistration(pThis);
            // }
        }

        void Actor::unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType )
        {
            // m_updateObjects[(int)updateType][(int)taskId] = false;

            // if (m_scene)
            //{
            //	auto pThis = getSharedFromThis<CActor>();
            //	m_scene->refreshRegistration(pThis);
            // }
        }

        void Actor::unregisterAll()
        {
            // for (u32 x = 0; x < int(Thread::UpdateState::Count); ++x)
            //{
            //	for (u32 y = 0; y < int(Thread::Task::Count); ++y)
            //	{
            //		m_updateObjects[x][y] = false;
            //	}
            // }

            // if (m_scene)
            //{
            //	auto pThis = getSharedFromThis<CActor>();
            //	m_scene->refreshRegistration(pThis);
            // }
        }

        void Actor::registerComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                             SmartPtr<IComponent> object )
        {
            auto p = getRegisteredComponents( updateType, taskId );
            if( p )
            {
                auto updateObjects = *p;
                auto it = std::find( updateObjects.begin(), updateObjects.end(), object );
                if( it == updateObjects.end() )
                {
                    updateObjects.push_back( object );

                    auto data = boost::make_shared<ConcurrentArray<SmartPtr<IComponent>>>();
                    std::sort( updateObjects.begin(), updateObjects.end() );
                    *data = updateObjects;
                    setRegisteredComponents( updateType, taskId, data );
                }

                FB_ASSERT( std::unique( updateObjects.begin(), updateObjects.end() ) ==
                           updateObjects.end() );
            }

            // if (object)
            //{
            //	object->OnRegisterUpdate(taskId, updateType);
            // }
        }

        void Actor::unregisterComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
                                               SmartPtr<IComponent> object )
        {
            auto p = getRegisteredComponents( updateType, taskId );
            if( p )
            {
                ConcurrentArray<SmartPtr<IComponent>> &updateObjects = *p;

                Array<SmartPtr<IComponent>> newObjects( updateObjects.begin(), updateObjects.end() );
                auto it = std::find( newObjects.begin(), newObjects.end(), object );
                if( it != newObjects.end() )
                {
                    newObjects.erase( it );

                    std::sort( newObjects.begin(), newObjects.end() );
                    boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> newArray(
                        new ConcurrentArray<SmartPtr<IComponent>>( newObjects.begin(),
                                                                   newObjects.end() ) );
                    setRegisteredComponents( updateType, taskId, newArray );
                }
            }
        }

        void Actor::unregisterAllComponent( SmartPtr<IComponent> object )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();

                for( u32 x = 0; x < static_cast<int>( Thread::UpdateState::Count ); ++x )
                {
                    for( u32 y = 0; y < static_cast<int>( Thread::Task::Count ); ++y )
                    {
                        ConcurrentArray<SmartPtr<IComponent>> updateObjects;

                        if( x < static_cast<int>( Thread::UpdateState::Count ) &&
                            y < static_cast<int>( Thread::Task::Count ) )
                        {
                            auto p = getRegisteredComponents( static_cast<Thread::UpdateState>( x ),
                                                              static_cast<Thread::Task>( y ) );
                            if( p )
                            {
                                updateObjects = *p;
                            }
                        }

                        auto hasElement = false;
                        auto it = updateObjects.begin();
                        for( ; it != updateObjects.end(); ++it )
                        {
                            if( ( *it ).get() == object.get() )
                            {
                                hasElement = true;
                                break;
                            }
                        }

                        if( hasElement )
                        {
                            ConcurrentArray<SmartPtr<IComponent>> newArray;
                            for( size_t i = 0; i < updateObjects.size(); ++i )
                            {
                                if( object.get() != updateObjects[i].get() )
                                {
                                    newArray.push_back( updateObjects[i] );
                                }
                            }

                            if( x < static_cast<int>( Thread::UpdateState::Count ) &&
                                y < static_cast<int>( Thread::Task::Count ) )
                            {
                                auto p = boost::make_shared<ConcurrentArray<SmartPtr<IComponent>>>();
                                *p = newArray;

                                setRegisteredComponents( static_cast<Thread::UpdateState>( x ),
                                                         static_cast<Thread::Task>( y ), p );
                            }
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Actor::handleEvent( const String &data )
        {
        }

        String Actor::getGraphicsDataAsJSON() const
        {
            return "";
        }

        void Actor::triggerEnter( SmartPtr<IComponent> collision )
        {
            // boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> p =
            // m_componentCache; ConcurrentArray<SmartPtr<IComponent>>& componentCache =
            // *p; ConcurrentArray<SmartPtr<IComponent>>::iterator it =
            // componentCache.begin(); for (; it != componentCache.end(); ++it)
            //{
            //	auto component = fb::static_pointer_cast<BaseComponent>(*it);
            //	component->triggerEnter(collision);
            // }
        }

        void Actor::triggerLeave( SmartPtr<IComponent> collision )
        {
            // boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> p =
            // m_componentCache; ConcurrentArray<SmartPtr<IComponent>>& componentCache =
            // *p; ConcurrentArray<SmartPtr<IComponent>>::iterator it =
            // componentCache.begin(); for (; it != componentCache.end(); ++it)
            //{
            //	auto component = fb::static_pointer_cast<BaseComponent>(*it);
            //	component->triggerLeave(collision);
            // }
        }

        void Actor::componentLoaded( SmartPtr<IComponent> component )
        {
            // boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> p =
            // m_componentCache; ConcurrentArray<SmartPtr<IComponent>>& componentCache =
            // *p; ConcurrentArray<SmartPtr<IComponent>>::iterator it =
            // componentCache.begin(); for (; it != componentCache.end(); ++it)
            //{
            //	auto component = fb::static_pointer_cast<BaseComponent>(*it);
            //	component->componentLoaded(component);
            // }
        }

        SmartPtr<IActor> Actor::getChildByIndex( u32 index ) const
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                return children[index];
            }

            return nullptr;
        }

        u32 Actor::getNumChildren() const
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                return static_cast<u32>( children.size() );
            }

            return 0;
        }

        s32 Actor::getSiblingIndex() const
        {
            auto parentTransform = getParent();
            if( parentTransform == nullptr )
            {
                return 0;
            }

            for( u32 i = 0; i < parentTransform->getNumChildren(); i++ )
            {
                const auto child = parentTransform->getChildByIndex( i );
                if( child == getSharedFromThis<IActor>() )
                {
                    return i;
                }
            }

            return -1;
        }

        void Actor::addDirtyComponent( Thread::Task taskId, Thread::UpdateState updateType,
                                       SmartPtr<IComponent> component )
        {
            // auto dirtyComponent = m_lastDirtyComponent[(int)updateType][(int)taskId];
            // if (!dirtyComponent)
            //{
            //	m_dirtyComponent[(int)updateType][(int)taskId] = component;
            //	m_lastDirtyComponent[(int)updateType][(int)taskId] = component;
            // }
            // else
            //{
            //	if (!isComponentDirty(taskId, updateType, component))
            //	{
            //		//dirtyComponent->setDirtySibling(updateType, taskId, component);
            //		m_lastDirtyComponent[(int)updateType][(int)taskId] = component;
            //	}
            // }
        }

        bool Actor::isComponentSiblingDirty( SmartPtr<IComponent> sibling, Thread::Task taskId,
                                             Thread::UpdateState updateType,
                                             SmartPtr<IComponent> component )
        {
            // if (sibling == component)
            //{
            //	return true;
            // }

            // if (sibling)
            //{
            //	//auto dirtySibling = sibling->getDirtySibling(updateType, taskId);
            //	//if (dirtySibling)
            //	//{
            //	//	return isComponentSiblingDirty(dirtySibling, taskId, updateType, component);
            //	//}
            // }

            return false;
        }

        bool Actor::isComponentDirty( Thread::Task taskId, Thread::UpdateState updateType,
                                      SmartPtr<IComponent> component )
        {
            // auto dirtyComponent = m_dirtyComponent[(int)updateType][(int)taskId];
            // if (dirtyComponent == component)
            //{
            //	return true;
            // }

            // if (dirtyComponent)
            //{
            //	//auto dirtySibling = dirtyComponent->getDirtySibling(updateType, taskId);
            //	//if (dirtySibling)
            //	//{
            //	//	return isComponentSiblingDirty(dirtySibling, taskId, updateType, component);
            //	//}
            // }

            return false;
        }

        boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> Actor::getRegisteredComponents(
            Thread::UpdateState updateState, Thread::Task task ) const
        {
            // return m_updateComponents[(int)updateState][(int)task];
            return nullptr;
        }

        void Actor::setRegisteredComponents( Thread::UpdateState updateState, Thread::Task task,
                                             boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> p )
        {
            // FB_ASSERT((size_t)updateState < m_updateComponents.size());
            // FB_ASSERT((size_t)task < m_updateComponents[(int)updateState].size());
            // FB_ASSERT(p);

            // auto& components = m_updateComponents[(int)updateState][(int)task];
            // components = p;
        }

        void Actor::addChild( SmartPtr<IActor> child )
        {
            FB_ASSERT( child );

            if( auto parent = child->getParent() )
            {
                parent->removeChild( child );
            }

            auto handle = child->getHandle();
            auto id = handle->getInstanceId();

            FB_ASSERT( id != std::numeric_limits<u32>::max() );
            FB_ASSERT( id != getHandle()->getInstanceId() );  // child id matches parent

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                children.push_back( child );

                //FB_ASSERT( CoreUtil::hasDuplicates( children ) == false );
            }

            auto pThis = getSharedFromThis<IActor>();
            child->setParent( pThis );

            if( auto p = getComponentsPtr() )
            {
                auto &components = *p;
                for( auto &component : components )
                {
                    component->childAdded( child );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &c : children )
                {
                    c->childAddedInHierarchy( child );
                }
            }
        }

        void Actor::removeChild( SmartPtr<IActor> child )
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;

                auto childrenArray = Array<SmartPtr<IActor>>( children.begin(), children.end() );
                const auto it = std::find( childrenArray.begin(), childrenArray.end(), child );
                if( it != childrenArray.end() )
                {
                    child->setParent( nullptr );
                    childrenArray.erase( it );
                }

                *m_children =
                    ConcurrentArray<SmartPtr<IActor>>( childrenArray.begin(), childrenArray.end() );
            }

            if( auto p = getComponentsPtr() )
            {
                auto &components = *p;
                for( auto &component : components )
                {
                    component->childRemoved( child );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &c : children )
                {
                    c->childRemovedInHierarchy( child );
                }
            }
        }

        void Actor::childAddedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto p = getComponentsPtr() )
            {
                auto &components = *p;
                for( auto &component : components )
                {
                    component->childAddedInHierarchy( child );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &c : children )
                {
                    c->childAddedInHierarchy( child );
                }
            }
        }

        void Actor::childRemovedInHierarchy( SmartPtr<IActor> child )
        {
            if( auto p = getComponentsPtr() )
            {
                auto &components = *p;
                for( auto &component : components )
                {
                    component->childRemovedInHierarchy( child );
                }
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &c : children )
                {
                    c->childRemovedInHierarchy( child );
                }
            }
        }

        void Actor::removeChildren()
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    auto pChild = fb::static_pointer_cast<Actor>( child );
                    pChild->setParent( nullptr );
                }
            }
        }

        SmartPtr<IActor> Actor::findChild( const String &name )
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    if( child->getName() == name )
                    {
                        return child;
                    }
                }
            }

            return nullptr;
        }

        SharedPtr<ConcurrentArray<SmartPtr<IActor>>> Actor::getChildrenPtr() const
        {
            return m_children;
        }

        Array<SmartPtr<IActor>> Actor::getAllChildren( SmartPtr<IActor> parent ) const
        {
            Array<SmartPtr<IActor>> children;
            children.reserve( 128 );

            auto parentTransform = parent;
            auto childCount = parentTransform->getNumChildren();

            for( u32 i = 0; i < childCount; i++ )
            {
                auto childTransform = parentTransform->getChildByIndex( i );
                children.push_back( childTransform );

                auto childChildren = getAllChildren( childTransform );

                children.insert( children.end(), childChildren.begin(), childChildren.end() );
            }

            return children;
        }

        Array<SmartPtr<IActor>> Actor::getAllChildren() const
        {
            auto thisActor = getSharedFromThis<IActor>();
            return getAllChildren( thisActor );
        }

        SmartPtr<IActor> Actor::getParent() const
        {
            auto p = m_parent.load();
            return p.lock();
        }

        void Actor::setParent( SmartPtr<IActor> parent )
        {
            auto pParent = fb::static_pointer_cast<Actor>( parent );

            auto p = getParent();
            if( p != parent )
            {
                auto oldParent = p;

                if( pParent )
                {
                    m_parent = pParent;
                }

                auto transform = getTransform();
                if( transform )
                {
                    transform->parentChanged( parent, oldParent );
                }

                auto components = getComponents();
                for( auto component : components )
                {
                    auto c = fb::static_pointer_cast<Component>( component );
                    c->parentChanged( parent, oldParent );
                }
            }
        }

        SmartPtr<ISharedObject> Actor::toData() const
        {
            auto typeManager = TypeManager::instance();

            //FB_ASSERT( getLoadingState() == LoadingState::Unloaded );
            //FB_ASSERT( getHandle()->getInstanceId() != std::numeric_limits<u32>::max() );

            auto actorData = fb::make_ptr<Properties>();

            auto label = getName();
            auto bStatic = isStatic();
            auto visible = isVisible();
            auto enabled = isEnabled();

            actorData->setProperty( "label", label );
            actorData->setProperty( "static", bStatic );
            actorData->setProperty( "visible", visible );
            actorData->setProperty( "enabled", enabled );

            if( auto handle = getHandle() )
            {
                auto uuid = handle->getUUID();
                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                actorData->setProperty( "uuid", uuid );
            }

            if( auto transform = getTransform() )
            {
                auto localTransform = transform->getLocalTransform();
                auto worldTransform = transform->getWorldTransform();

                actorData->setProperty( "localTransform", localTransform );
                actorData->setProperty( "worldTransform", worldTransform );
            }

            if( auto p = this->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        FB_ASSERT( child->getHandle()->getInstanceId() !=
                                   std::numeric_limits<u32>::max() );

                        auto pChildActorData = fb::static_pointer_cast<Properties>( child->toData() );
                        pChildActorData->setName( "child" );
                        actorData->addChild( pChildActorData );
                    }
                }
            }

            auto components = getComponents();
            for( auto component : components )
            {
                auto componentData = fb::static_pointer_cast<Properties>( component->toData() );
                if( componentData )
                {
                    componentData->setName( "component" );

                    auto typeinfo = component->getTypeInfo();
                    auto className = typeManager->getName( typeinfo );

                    auto componentType = StringUtil::replaceAll( className, "class ", "" );
                    componentData->setProperty( "componentType", componentType );

                    actorData->addChild( componentData );
                }
            }

            return actorData;
        }

        void Actor::fromData( SmartPtr<ISharedObject> data )
        {
            try
            {
                FB_ASSERT( data );

                if( !data )
                {
                    FB_LOG_ERROR( "CActor::fromData data null." );
                    return;
                }

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto actorData = fb::static_pointer_cast<Properties>( data );

                auto label = String();
                auto enabled = true;
                auto visible = true;
                auto bStatic = false;

                actorData->getPropertyValue( "label", label );
                actorData->getPropertyValue( "isEnabled", enabled );
                actorData->getPropertyValue( "isVisible", visible );
                actorData->getPropertyValue( "static", bStatic );

                setName( label );
                setEnabled( enabled );
                setVisible( visible );
                setStatic( bStatic );

                if( auto handle = getHandle() )
                {
                    auto uuid = String();
                    actorData->getPropertyValue( "uuid", uuid );

                    if( StringUtil::isNullOrEmpty( uuid ) )
                    {
                        uuid = StringUtil::getUUID();
                    }

                    handle->setUUID( uuid );
                }

                if( auto transform = getTransform() )
                {
                    auto localTransform = transform->getLocalTransform();
                    auto worldTransform = transform->getWorldTransform();

                    actorData->getPropertyValue( "localTransform", localTransform );
                    actorData->getPropertyValue( "worldTransform", worldTransform );

                    transform->setLocalTransform( localTransform );
                    transform->setWorldTransform( worldTransform );
                }

                auto componentsData = actorData->getChildrenByName( "component" );

                auto components = Array<SmartPtr<IComponent>>();
                components.reserve( componentsData.size() );

                for( auto &component : componentsData )
                {
                    auto componentType = String();
                    component->getPropertyValue( "componentType", componentType );

                    auto pComponent = factoryManager->createObjectFromType<IComponent>( componentType );
                    if( !pComponent )
                    {
                        auto componentTypeClean = StringUtil::replaceAll( componentType, "fb::", "" );
                        pComponent =
                            factoryManager->createObjectFromType<IComponent>( componentTypeClean );
                    }

                    if( !pComponent )
                    {
                        auto componentTypeClean = sceneManager->getComponentFactoryType( componentType );
                        pComponent =
                            factoryManager->createObjectFromType<IComponent>( componentTypeClean );
                    }

                    if( pComponent )
                    {
                        components.push_back( pComponent );
                    }
                }

                for( size_t i = 0; i < components.size(); ++i )
                {
                    try
                    {
                        auto &pComponent = components[i];
                        addComponentInstance( pComponent );
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }

                for( size_t i = 0; i < components.size(); ++i )
                {
                    try
                    {
                        auto &pComponent = components[i];
                        auto &pComponentData = componentsData[i];
                        // FB_ASSERT(pComponent);

                        if( pComponent )
                        {
                            pComponent->setActor( this );

                            pComponent->fromData( pComponentData );

                            pComponent->load( nullptr );
                            componentLoaded( pComponent );
                        }
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }

                auto childrenData = actorData->getChildrenByName( "child" );
                for( auto &child : childrenData )
                {
                    auto childActor = sceneManager->createActor();
                    FB_ASSERT( childActor );

                    childActor->fromData( child );
                    addChild( childActor );

                    //FB_ASSERT( getChildrenPtr()->size() == childrenData.size() );
                    // FB_ASSERT(childActor->getComponents<IComponent>().size() ==
                    // child.components.size());
                }

                updateTransform();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<Properties> Actor::getProperties() const
        {
            auto properties = CResource<IActor>::getProperties();

            properties->setProperty( "name", getName() );

            bool bIsStatic = isStatic();
            properties->setProperty( "isStatic", bIsStatic );

            auto enabled = isEnabled();
            properties->setProperty( "isEnabled", enabled );

            return properties;
        }

        void Actor::setProperties( SmartPtr<Properties> properties )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            auto sceneManager = applicationManager->getSceneManager();

            auto name = String();
            properties->getPropertyValue( "label", name );
            setName( name );

            bool bIsStatic = false;
            properties->getPropertyValue( "isStatic", bIsStatic );
            setStatic( bIsStatic );

            auto enabled = isEnabled();
            properties->getPropertyValue( "isEnabled", enabled );
            setEnabled( enabled );

            if( auto handle = getHandle() )
            {
                auto uuid = String();
                if( properties->getPropertyValue( "uuid", uuid ) )
                {
                    if( StringUtil::isNullOrEmpty( uuid ) )
                    {
                        uuid = StringUtil::getUUID();
                    }

                    handle->setUUID( uuid );
                }
            }

            if( auto transform = getTransform() )
            {
                auto localTransformChild = properties->getChild( "localTransform" );
                auto localTransform = transform->getLocalTransform();
                localTransform.setProperties( localTransformChild );

                auto worldTransformChild = properties->getChild( "worldTransform" );
                auto worldTransform = transform->getWorldTransform();
                worldTransform.setProperties( properties );

                transform->setLocalTransform( localTransform );
                //transform->setWorldTransform( worldTransform );
            }

            auto componentsData = properties->getChildrenByName( "components" );

            auto components = Array<SmartPtr<IComponent>>();
            components.reserve( componentsData.size() );

            for( auto componentData : componentsData )
            {
                auto componentType = String();
                componentData->getPropertyValue( "componentType", componentType );

                auto pComponent = factoryManager->createObjectFromType<Component>( componentType );
                if( !pComponent )
                {
                    componentType = StringUtil::replaceAll( componentType, "fb::", "" );
                    pComponent = factoryManager->createObjectFromType<IComponent>( componentType );
                }

                if( !pComponent )
                {
                    componentType = sceneManager->getComponentFactoryType( componentType );
                    pComponent = factoryManager->createObjectFromType<IComponent>( componentType );
                }

                if( pComponent )
                {
                    components.push_back( pComponent );
                }
            }

            for( size_t i = 0; i < components.size(); ++i )
            {
                try
                {
                    auto &pComponent = components[i];
                    addComponentInstance( pComponent );
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            //FB_ASSERT( components.size() == componentsData.size() );

            for( size_t i = 0; i < components.size(); ++i )
            {
                try
                {
                    auto &pComponent = components[i];
                    auto &componentData = componentsData[i];

                    if( pComponent )
                    {
                        pComponent->setActor( this );

                        pComponent->fromData( componentData );

                        pComponent->load( nullptr );
                        componentLoaded( pComponent );
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            auto childrenData = properties->getChildrenByName( "children" );
            for( auto &childData : childrenData )
            {
                auto childActor = sceneManager->createActor();
                FB_ASSERT( childActor );

                childActor->setProperties( childData );
                addChild( childActor );

                //FB_ASSERT( childActor->getChildren()->size() == childrenData.size() );
                // FB_ASSERT(childActor->getComponents<IComponent>().size() ==
                // child.components.size());
            }

            updateTransform();
        }

        void Actor::setState( State state )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            auto fsm = sceneManager->getFSM( id );
            if( fsm )
            {
                fsm->setState<State>( state );
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->setState( state );
                }
            }
        }

        IActor::State Actor::getState() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            auto fsm = sceneManager->getFSM( id );
            if( fsm )
            {
                return fsm->getState<State>();
            }

            return static_cast<State>( 0 );
        }

        void Actor::setGameState( GameState state )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            auto fsm = sceneManager->getFSM( id );
            if( fsm )
            {
                fsm->setState<GameState>( state );
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->setGameState( state );
                }
            }
        }

        IActor::GameState Actor::getGameState() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            auto fsm = sceneManager->getFSM( id );
            if( fsm )
            {
                return fsm->getState<GameState>();
            }

            return static_cast<GameState>( 0 );
        }

        u32 Actor::getFlags() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            if( id != std::numeric_limits<u32>::max() )
            {
                if( sceneManager )
                {
                    return sceneManager->getCurrentFlags( id );
                }
            }

            return 0;
        }

        void Actor::setFlags( u32 flags )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            sceneManager->setFlags( id, flags );
        }

        bool Actor::getFlag( u32 flag ) const
        {
            return ( getFlags() & flag ) != 0;
        }

        void Actor::setFlag( u32 flag, bool value )
        {
            auto flags = getFlags();
            if( value )
            {
                flags |= flag;
            }
            else
            {
                flags &= ~flag;
            }

            setFlags( flags );
        }

        u32 Actor::getNewFlags() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            if( id != std::numeric_limits<u32>::max() )
            {
                return sceneManager->getNewFlags( id );
            }

            return 0;
        }

        void Actor::setNewFlags( u32 flags )
        {
            if( getNewFlags() != flags )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto pSceneManager = applicationManager->getSceneManager();
                auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
                FB_ASSERT( sceneManager );

                auto handle = getHandle();
                auto id = handle->getInstanceId();
                sceneManager->setNewFlags( id, flags );

                sceneManager->addDirty( this );
            }
        }

        bool Actor::getNewFlag( u32 flag ) const
        {
            return ( getNewFlags() & flag ) != 0;
        }

        void Actor::setNewFlag( u32 flag, bool value )
        {
            auto flags = getNewFlags();
            if( value )
            {
                flags |= flag;
            }
            else
            {
                flags &= ~flag;
            }

            setNewFlags( flags );
        }

        void Actor::updateVisibility()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<SceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );
            FB_ASSERT( sceneManager->isLoaded() );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getNewFlags( id );

            auto enabledInScene = isEnabledInScene();
            if( BitUtil::getFlagValue( flags, ActorFlagEnabledInScene ) != enabledInScene )
            {
                auto newFlags = BitUtil::setFlagValue( flags, ActorFlagEnabledInScene, enabledInScene );
                sceneManager->setNewFlags( id, newFlags );
                sceneManager->addDirty( this );
            }

            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->updateVisibility();
                }
            }
        }

        IFSM::ReturnType Actor::ActorFsmListener::handleEvent( u32 state, IFSM::Event eventType )
        {
            FB_ASSERT( m_owner );
            return m_owner->handleActorEvent( state, eventType );
        }

        Actor *Actor::ActorFsmListener::getOwner() const
        {
            return m_owner;
        }

        void Actor::ActorFsmListener::setOwner( Actor *owner )
        {
            m_owner = owner;
        }

        IFSM::ReturnType Actor::ActorGameFsmListener::handleEvent( u32 state, IFSM::Event eventType )
        {
            FB_ASSERT( m_owner );
            return m_owner->handleActorGameEvent( state, eventType );
        }

        Actor *Actor::ActorGameFsmListener::getOwner() const
        {
            return m_owner;
        }

        void Actor::ActorGameFsmListener::setOwner( Actor *owner )
        {
            m_owner = owner;
        }

        void Actor::registerClass()
        {
            using namespace fb;
            using namespace rttr;

            registration::class_<Actor>( "Actor" ).property( "transform", &Actor::m_transform );
        }

        template <class T>
        SmartPtr<T> Actor::removeComponent()
        {
            // auto typeinfo = T::typeInfo();
            // FB_ASSERT(typeinfo);

            // auto iClassType = typeinfo->getHash();
            // auto it = m_componentsByType.find(iClassType);
            // if (it != m_componentsByType.end())
            //{
            //	auto& components = it->second;
            //	if (!components.empty())
            //	{
            //		auto componentIt = components.begin();
            //		auto component = (*componentIt);
            //		components.erase(componentIt);
            //		rebuildComponentCache();
            //		return component;
            //	}
            // }

            return nullptr;
        }
    }  // namespace scene
}  // end namespace fb
