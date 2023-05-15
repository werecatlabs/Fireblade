#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/CActor.h>
#include <FBCore/Scene/CSceneManager.h>
#include <FBCore/Scene/Components/Transform.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Math/Core/MathUtil.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Base/BitUtil.h>
#include <FBCore/Base/CoreUtil.h>
#include <rttr/registration>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, CActor, CResource<IActor> );
        u32 CActor::m_idExt = 0;

        CActor::CActor()
        {
            setResourceType( ResourceType::Actor );

            m_children = fb::make_shared<ConcurrentArray<SmartPtr<IActor>>>();
            m_components = fb::make_shared<ConcurrentArray<SmartPtr<IComponent>>>();
        }

        CActor::~CActor()
        {
            unload( nullptr );
        }

        String CActor::getName() const
        {
            auto handle = getHandle();
            FB_ASSERT( handle );

            return handle->getName();
        }

        void CActor::setName( const String &name )
        {
            auto handle = getHandle();
            FB_ASSERT( handle );

            handle->setName( name );
        }

        Transform3<real_Num> CActor::getWorldTransform( time_interval t ) const
        {
            if( auto transform = getTransform() )
            {
                return transform->getWorldTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> CActor::getWorldTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getWorldTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> CActor::getLocalTransform( time_interval t ) const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalTransform();
            }

            return Transform3<real_Num>();
        }

        Transform3<real_Num> CActor::getLocalTransform() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalTransform();
            }

            return Transform3<real_Num>();
        }

        Vector3<real_Num> CActor::getLocalPosition() const
        {
            if( auto transform = getTransform() )
            {
                return transform->getLocalPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void CActor::setLocalPosition( const Vector3<real_Num> &localPosition )
        {
            if( auto transform = getTransform() )
            {
                transform->setLocalPosition( localPosition );
            }
        }

        Vector3<real_Num> CActor::getLocalScale() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getLocalScale();
            }

            return Vector3<real_Num>::zero();
        }

        void CActor::setLocalScale( const Vector3<real_Num> &localScale )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setLocalScale( localScale );
            }
        }

        Quaternion<real_Num> CActor::getLocalOrientation() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getLocalOrientation();
            }

            return Quaternion<real_Num>::identity();
        }

        void CActor::setLocalOrientation( const Quaternion<real_Num> &localOrientation )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setLocalOrientation( localOrientation );
            }
        }

        Vector3<real_Num> CActor::getPosition() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void CActor::lookAt( const Vector3<real_Num> &position )
        {
            auto vec = position - getPosition();
            auto rot = MathUtil<real_Num>::getRotationTo( -Vector3<real_Num>::unitZ(), vec );
            setOrientation( rot );
        }

        void CActor::lookAt( const Vector3<real_Num> &position, const Vector3<real_Num> &yawAxis )
        {
            auto vec = position - getPosition();
            auto rot = MathUtil<real_Num>::getOrientationFromDirection( vec, -Vector3<real_Num>::unitZ(),
                                                                        true, yawAxis );
            setOrientation( rot );
        }

        void CActor::setPosition( const Vector3<real_Num> &position )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setPosition( position );
            }
        }

        Vector3<real_Num> CActor::getScale() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getPosition();
            }

            return Vector3<real_Num>::zero();
        }

        void CActor::setScale( const Vector3<real_Num> &scale )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setPosition( scale );
            }
        }

        Quaternion<real_Num> CActor::getOrientation() const
        {
            auto transform = getTransform();
            if( transform )
            {
                return transform->getOrientation();
            }

            return Quaternion<real_Num>::identity();
        }

        void CActor::setOrientation( const Quaternion<real_Num> &orientation )
        {
            auto transform = getTransform();
            if( transform )
            {
                transform->setOrientation( orientation );
            }
        }

        void CActor::awake()
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

        void CActor::play()
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

        void CActor::edit()
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
                        c->edit();
                    }
                }

                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        child->edit();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CActor::levelWasLoaded( SmartPtr<IScene> scene )
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

        void CActor::reset()
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

        void CActor::hierarchyChanged()
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

        void CActor::childAdded( SmartPtr<IActor> child )
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

        void CActor::childRemoved( SmartPtr<IActor> child )
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

        void CActor::preUpdate()
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

        void CActor::updateDirty( u32 flags, u32 oldFlags )
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

        void CActor::update()
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

        void CActor::postUpdate()
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

        void CActor::addComponentInstance( SmartPtr<IComponent> component )
        {
            try
            {
                FB_ASSERT( component );

                auto entity = getEntity();
                component->setEntity( (u32)entity );

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

        void CActor::removeComponentInstance( SmartPtr<IComponent> component )
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

        bool CActor::hasComponent( int handle )
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

        SmartPtr<IComponent> CActor::getComponent( int handle ) const
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

        bool CActor::isMine() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagMine );
        }

        void CActor::setMine( bool val )
        {
            // m_isMine = val;
        }

        bool CActor::isStatic() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagStatic );
        }

        void CActor::setStatic( bool isstatic )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        bool CActor::isEnabledInScene() const
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

        bool CActor::isEnabled() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagEnabled );
        }

        void CActor::setEnabled( bool enabled )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        bool CActor::isVisible() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagVisible );
        }

        void CActor::setVisible( bool visible, bool cacade )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        bool CActor::isDirty() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto id = handle->getInstanceId();
            FB_ASSERT( id != std::numeric_limits<u32>::max() );

            auto flags = sceneManager->getCurrentFlags( id );
            return BitUtil::getFlagValue( flags, ActorFlagDirty );
        }

        void CActor::setDirty( bool dirty )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        bool CActor::isValid() const
        {
            return true;
        }

        void CActor::updateTransform()
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

        Array<SmartPtr<IComponent>> CActor::getComponents() const
        {
            if( auto p = getComponentsPtr() )
            {
                auto &components = *p;
                return Array<SmartPtr<IComponent>>( components.begin(), components.end() );
            }

            return Array<SmartPtr<IComponent>>();
        }

        void CActor::updateComponents()
        {
            // for (auto component : m_components)
            //{
            //	component->update();
            // }
        }

        void CActor::rebuildComponentCache()
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

        void CActor::preUpdateDirtyComponent( SmartPtr<IComponent> &component )
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

        void CActor::updateDirtyComponent( SmartPtr<IComponent> &component )
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

        void CActor::postUpdateDirtyComponent( SmartPtr<IComponent> &component )
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

        IFSM::ReturnType CActor::handleActorEvent( u32 state, IFSM::Event eventType )
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

        IFSM::ReturnType CActor::handleActorGameEvent( u32 state, IFSM::Event eventType )
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

        void CActor::preUpdateDirtyComponents()
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

        void CActor::updateDirtyComponents()
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

        void CActor::postUpdateDirtyComponents()
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

        bool CActor::compareTag( const String &tag ) const
        {
            // auto it = std::find(m_tags.begin(), m_tags.end(), tag);
            // if (it != m_tags.end())
            //{
            //	return true;
            // }

            return false;
        }

        SmartPtr<IActor> CActor::getSceneRoot() const
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

        u32 CActor::getSceneLevel() const
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

        SmartPtr<ITransform> CActor::getTransform() const
        {
            auto p = m_transform.load();
            return p.lock();
        }

        void CActor::setTransform( SmartPtr<ITransform> transform )
        {
            m_transform = transform;
        }

        void CActor::load( SmartPtr<ISharedObject> data )
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
                    auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        void CActor::reload( SmartPtr<ISharedObject> data )
        {
        }

        void CActor::unload( SmartPtr<ISharedObject> data )
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
                        auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );

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
                            auto pThis = getSharedFromThis<CActor>();
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

        bool CActor::getPerpetual() const
        {
            return m_destroyOnLoad;
        }

        void CActor::setPerpetual( bool destroyOnLoad )
        {
            m_destroyOnLoad = destroyOnLoad;
        }

        bool CActor::getAutoUpdateComponents() const
        {
            return m_autoUpdateComponents;
        }

        void CActor::setAutoUpdateComponents( bool autoUpdateComponents )
        {
            m_autoUpdateComponents = autoUpdateComponents;
        }

        SmartPtr<IScene> CActor::getScene() const
        {
            auto p = m_scene.load();
            return p.lock();
        }

        void CActor::setScene( SmartPtr<IScene> scene )
        {
            m_scene = scene;
        }

        bool CActor::isRegistered( Thread::Task taskId, Thread::UpdateState updateType ) const
        {
            // return m_updateObjects[(int)updateType][(int)taskId] ? true : false;
            return false;
        }

        void CActor::registerUpdate( Thread::Task taskId, Thread::UpdateState updateType )
        {
            // m_updateObjects[(int)updateType][(int)taskId] = true;

            // if (m_scene)
            //{
            //	auto pThis = getSharedFromThis<CActor>();
            //	m_scene->refreshRegistration(pThis);
            // }
        }

        void CActor::unregisterUpdate( Thread::Task taskId, Thread::UpdateState updateType )
        {
            // m_updateObjects[(int)updateType][(int)taskId] = false;

            // if (m_scene)
            //{
            //	auto pThis = getSharedFromThis<CActor>();
            //	m_scene->refreshRegistration(pThis);
            // }
        }

        void CActor::unregisterAll()
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

        void CActor::registerComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
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

        void CActor::unregisterComponentUpdate( Thread::Task taskId, Thread::UpdateState updateType,
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

        void CActor::unregisterAllComponent( SmartPtr<IComponent> object )
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

        void CActor::handleEvent( const String &data )
        {
        }

        String CActor::getGraphicsDataAsJSON() const
        {
            return "";
        }

        void CActor::triggerEnter( SmartPtr<IComponent> collision )
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

        void CActor::triggerLeave( SmartPtr<IComponent> collision )
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

        void CActor::componentLoaded( SmartPtr<IComponent> component )
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

        SmartPtr<IActor> CActor::getChildByIndex( u32 index ) const
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                return children[index];
            }

            return nullptr;
        }

        u32 CActor::getNumChildren() const
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                return (u32)children.size();
            }

            return 0;
        }

        s32 CActor::getSiblingIndex() const
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

        void CActor::addDirtyComponent( Thread::Task taskId, Thread::UpdateState updateType,
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

        bool CActor::isComponentSiblingDirty( SmartPtr<IComponent> sibling, Thread::Task taskId,
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

        bool CActor::isComponentDirty( Thread::Task taskId, Thread::UpdateState updateType,
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

        boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> CActor::getRegisteredComponents(
            Thread::UpdateState updateState, Thread::Task task ) const
        {
            // return m_updateComponents[(int)updateState][(int)task];
            return nullptr;
        }

        void CActor::setRegisteredComponents(
            Thread::UpdateState updateState, Thread::Task task,
            boost::shared_ptr<ConcurrentArray<SmartPtr<IComponent>>> p )
        {
            // FB_ASSERT((size_t)updateState < m_updateComponents.size());
            // FB_ASSERT((size_t)task < m_updateComponents[(int)updateState].size());
            // FB_ASSERT(p);

            // auto& components = m_updateComponents[(int)updateState][(int)task];
            // components = p;
        }

        void CActor::addChild( SmartPtr<IActor> child )
        {
            FB_ASSERT( child );

            if( auto parent = getParent() )
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

                FB_ASSERT( CoreUtil::hasDuplicates( children ) == false );
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

        void CActor::removeChild( SmartPtr<IActor> child )
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

        void CActor::childAddedInHierarchy( SmartPtr<IActor> child )
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

        void CActor::childRemovedInHierarchy( SmartPtr<IActor> child )
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

        void CActor::removeChildren()
        {
            if( auto p = getChildrenPtr() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    auto pChild = fb::static_pointer_cast<CActor>( child );
                    pChild->setParent( nullptr );
                }
            }
        }

        SmartPtr<IActor> CActor::findChild( const String &name )
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

        SharedPtr<ConcurrentArray<SmartPtr<IActor>>> CActor::getChildrenPtr() const
        {
            return m_children;
        }

        Array<SmartPtr<IActor>> CActor::getAllChildren( SmartPtr<IActor> parent ) const
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

        Array<SmartPtr<IActor>> CActor::getAllChildren() const
        {
            auto thisActor = getSharedFromThis<IActor>();
            return getAllChildren( thisActor );
        }

        SmartPtr<IActor> CActor::getParent() const
        {
            auto p = m_parent.load();
            return p.lock();
        }

        void CActor::setParent( SmartPtr<IActor> parent )
        {
            auto pParent = fb::static_pointer_cast<CActor>( parent );

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

        SmartPtr<Properties> CActor::getProperties() const
        {
            auto properties = CResource<IActor>::getProperties();

            properties->setProperty( "name", getName() );

            bool bIsStatic = isStatic();
            properties->setProperty( "isStatic", bIsStatic );

            auto enabled = isEnabled();
            properties->setProperty( "isEnabled", enabled );

            return properties;
        }

        void CActor::setProperties( SmartPtr<Properties> properties )
        {
            auto name = String( "" );
            properties->getPropertyValue( "name", name );
            setName( name );

            bool bIsStatic = false;
            properties->getPropertyValue( "isStatic", bIsStatic );
            setStatic( bIsStatic );

            auto enabled = isEnabled();
            properties->getPropertyValue( "isEnabled", enabled );
            setEnabled( enabled );
        }

        SmartPtr<IStateContext> CActor::getStateObject() const
        {
            return m_stateObject;
        }

        void CActor::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        void CActor::setState( State state )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        IActor::State CActor::getState() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        void CActor::setGameState( GameState state )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        IActor::GameState CActor::getGameState() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        u32 CActor::getFlags() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );

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

        void CActor::setFlags( u32 flags )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            sceneManager->setFlags( id, flags );
        }

        bool CActor::getFlag( u32 flag ) const
        {
            return ( getFlags() & flag ) != 0;
        }

        void CActor::setFlag( u32 flag, bool value )
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

        u32 CActor::getNewFlags() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
            FB_ASSERT( sceneManager );

            auto handle = getHandle();
            auto id = handle->getInstanceId();
            if( id != std::numeric_limits<u32>::max() )
            {
                return sceneManager->getNewFlags( id );
            }

            return 0;
        }

        void CActor::setNewFlags( u32 flags )
        {
            if( getNewFlags() != flags )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto pSceneManager = applicationManager->getSceneManager();
                auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
                FB_ASSERT( sceneManager );

                auto handle = getHandle();
                auto id = handle->getInstanceId();
                sceneManager->setNewFlags( id, flags );

                sceneManager->addDirty( this );
            }
        }

        bool CActor::getNewFlag( u32 flag ) const
        {
            return ( getNewFlags() & flag ) != 0;
        }

        void CActor::setNewFlag( u32 flag, bool value )
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

        void CActor::updateVisibility()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto pSceneManager = applicationManager->getSceneManager();
            auto sceneManager = fb::static_pointer_cast<CSceneManager>( pSceneManager );
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

        u32 CActor::getEntity() const
        {
            return m_entity;
        }

        void CActor::setEntity( u32 entity )
        {
            m_entity = entity;
        }

        IFSM::ReturnType CActor::ActorFsmListener::handleEvent( u32 state, IFSM::Event eventType )
        {
            FB_ASSERT( m_owner );
            return m_owner->handleActorEvent( state, eventType );
        }

        CActor *CActor::ActorFsmListener::getOwner() const
        {
            return m_owner;
        }

        void CActor::ActorFsmListener::setOwner( CActor *owner )
        {
            m_owner = owner;
        }

        IFSM::ReturnType CActor::ActorGameFsmListener::handleEvent( u32 state, IFSM::Event eventType )
        {
            FB_ASSERT( m_owner );
            return m_owner->handleActorGameEvent( state, eventType );
        }

        CActor *CActor::ActorGameFsmListener::getOwner() const
        {
            return m_owner;
        }

        void CActor::ActorGameFsmListener::setOwner( CActor *owner )
        {
            m_owner = owner;
        }

        void CActor::registerClass()
        {
            using namespace fb;
            using namespace rttr;

            registration::class_<CActor>( "Actor" )
                .property( "transform", &CActor::m_transform );
        }

    }  // namespace scene
}  // end namespace fb
