#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
#include <FBCore/Scene/Components/SubComponent.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Interface/Scene/IComponentEvent.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/System/IState.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Component, Resource<IComponent> );
    u32 Component::m_idExt = 0;
    const String Component::enabledStr = "Enabled";
    const String Component::dirtyStr = "Dirty";

    Component::Component()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "Component" );
        }
    }

    Component::~Component()
    {
        unload( nullptr );
    }

    void Component::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto fsmManager = sceneManager->getComponentFsmManager( getTypeInfo() );
            FB_ASSERT( fsmManager );

            m_componentFSM = fsmManager->createFSM();

            auto componentFsmListener = factoryManager->make_ptr<ComponentFSMListener>();
            componentFsmListener->setOwner( this );
            m_componentFsmListener = componentFsmListener;

            m_componentFSM->addListener( m_componentFsmListener );

            setResourceType( ResourceType::Component );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Component::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( auto sceneManager = applicationManager->getSceneManager() )
                {
                    if( auto fsmManager = sceneManager->getComponentFsmManager( getTypeInfo() ) )
                    {
                        if( m_componentFSM )
                        {
                            if( m_componentFsmListener )
                            {
                                m_componentFSM->removeListener( m_componentFsmListener );
                                m_componentFsmListener = nullptr;
                            }

                            fsmManager->destroyFSM( m_componentFSM );
                            m_componentFSM = nullptr;
                        }
                    }
                }

                if( auto p = getSubComponentsPtr() )
                {
                    for( auto &subComponent : *p )
                    {
                        subComponent->unload( nullptr );
                    }
                }

                if( m_stateContext )
                {
                    m_stateContext->unload( data );
                    m_stateContext = nullptr;
                }

                if( m_stateListener )
                {
                    m_stateListener->unload( data );
                    m_stateListener = nullptr;
                }

                m_actor = nullptr;

                Resource<IComponent>::unload( nullptr );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Component::reload( SmartPtr<ISharedObject> data )
    {
        unload( nullptr );
        load( nullptr );
    }

    void Component::updateFlags( u32 flags, u32 oldFlags )
    {
        updateTransform();
        updateVisibility();
    }

    void Component::setComponentFlag( u32 flag, bool value )
    {
        u32 flags = m_componentFlags;
        if( value )
        {
            flags |= flag;
        }
        else
        {
            flags &= ~flag;
        }

        m_componentFlags = flags;
    }

    auto Component::getComponentFlag( u32 flag ) const -> bool
    {
        return ( m_componentFlags & flag ) != 0;
    }

    void Component::setEnabled( bool enabled )
    {
        m_enabled = enabled;
    }

    auto Component::isEnabled() const -> bool
    {
        return m_enabled;
    }

    auto Component::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
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
            case State::Play:
            {
            }
            break;
            default:
            {
            }
            }
        }
        break;
        case IFSM::Event::Leave:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Play:
            {
            }
            break;
            default:
            {
            }
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
        default:
        {
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    void Component::setDirty( bool dirty )
    {
        m_isDirty = dirty;
    }

    void Component::setDirty( Thread::Task taskId, Thread::UpdateState updateType, bool dirty )
    {
    }

    auto Component::isDirty( Thread::Task taskId, Thread::UpdateState updateType ) const -> bool
    {
        return m_isDirty;
    }

    auto Component::isDirty() const -> bool
    {
        return m_isDirty;
    }

    void Component::makeDirty()
    {
    }

    void Component::makeDirty( Thread::Task task )
    {
    }

    auto Component::getActor() -> SmartPtr<IActor> &
    {
        return m_actor;
    }

    auto Component::getActor() const -> const SmartPtr<IActor> &
    {
        return m_actor;
    }

    void Component::setActor( SmartPtr<IActor> actor )
    {
        m_actor = actor;
    }

    auto Component::toData() const -> SmartPtr<ISharedObject>
    {
        auto componentData = getProperties();

        if( auto handle = getHandle() )
        {
            auto uuid = handle->getUUID();
            if( StringUtil::isNullOrEmpty( uuid ) )
            {
                uuid = StringUtil::getUUID();
            }

            componentData->setProperty( "uuid", uuid );
        }

        auto subComponents = getSubComponents();
        for( auto &subComponent : subComponents )
        {
            if( auto subComponentData = fb::static_pointer_cast<Properties>( subComponent->toData() ) )
            {
                subComponentData->setName( "subComponent" );
                componentData->addChild( subComponentData );
            }
        }

        auto events = getEvents();
        for( auto &event : events )
        {
            auto eventData = fb::make_ptr<Properties>();

            auto listeners = event->getListeners();
            for( auto &listener : listeners )
            {
                if( auto properties = listener->getProperties() )
                {
                    properties->setName( "listener" );
                    eventData->addChild( properties );
                }
            }

            eventData->setName( "event" );
            componentData->addChild( eventData );
        }

        return componentData;
    }

    void Component::fromData( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto componentData = fb::static_pointer_cast<Properties>( data );

        auto name = String();
        componentData->getPropertyValue( "name", name );

        if( auto handle = getHandle() )
        {
            auto uuid = String();
            componentData->getPropertyValue( "uuid", uuid );

            if( StringUtil::isNullOrEmpty( uuid ) )
            {
                uuid = StringUtil::getUUID();
            }

            handle->setUUID( uuid );
        }

        //auto properties = componentData->getChild( "properties" );
        //if( !properties )
        //{
        //    properties = factoryManager->make_ptr<Properties>();

        //    if( auto oldStyleProperties = componentData->getChild( "properties_" ) )
        //    {
        //        auto childProperties = oldStyleProperties->getChildrenByName( "properties_" );
        //        for( auto child : childProperties )
        //        {
        //            Property property;
        //            property.setName( child->getProperty( "name" ) );
        //            property.setValue( child->getProperty( "value" ) );
        //            properties->addProperty( property );
        //        }
        //    }
        //}

        sceneManager->queueProperties( this, data );
        //setProperties( pProperties );

        auto subComponentData = componentData->getChildrenByName( "subComponent" );

        auto components = Array<SmartPtr<ISubComponent>>();
        components.reserve( subComponentData.size() );

        for( auto &component : subComponentData )
        {
            auto componentType = String();
            component->getPropertyValue( "componentType", componentType );

            auto pComponent = factoryManager->createObjectFromType<ISubComponent>( componentType );
            if( !pComponent )
            {
                auto componentTypeClean = StringUtil::replaceAll( componentType, "fb::", "" );
                pComponent = factoryManager->createObjectFromType<ISubComponent>( componentTypeClean );
            }

            if( pComponent )
            {
                components.push_back( pComponent );
            }
        }

        for( auto &pComponent : components )
        {
            try
            {
                addSubComponent( pComponent );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        auto count = 0;
        for( auto pComponent : components )
        {
            try
            {
                if( pComponent )
                {
                    pComponent->setParent( this );

                    if( count < subComponentData.size() )
                    {
                        auto &pComponentData = subComponentData[count];
                        if( pComponentData )
                        {
                            pComponent->fromData( componentData );
                        }
                    }

                    count++;

                    pComponent->load( nullptr );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        auto eventsData = componentData->getChildrenByName( "event" );
        auto events = getEvents();

        count = 0;
        for( auto &rEvent : eventsData )
        {
            auto event = events[count];
            event->removeListeners();

            auto listenerData = rEvent->getChildrenByName( "listener" );

            for( auto &rListener : listenerData )
            {
                auto eventListener = factoryManager->make_ptr<ComponentEventListener>();
                eventListener->setEvent( event );

                sceneManager->queueProperties( eventListener, rListener );
                //eventListener->setProperties( pProperties );

                event->addListener( eventListener );
            }

            count++;
        }
    }

    auto Component::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> childObjects;
        childObjects.reserve( 6 );

        childObjects.emplace_back( m_componentFSM );
        childObjects.emplace_back( m_componentFsmListener );

        if( auto actor = getActor() )
        {
            childObjects.emplace_back( actor );
        }

        return childObjects;
    }

    auto Component::getProperties() const -> SmartPtr<Properties>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto properties = Resource<IComponent>::getProperties();

        const auto enabled = isEnabled();
        properties->setProperty( enabledStr, enabled );

        const auto dirty = isDirty();
        properties->setProperty( dirtyStr, dirty );

        const u32 componentFlags = m_componentFlags;
        properties->setProperty( "ComponentFlags", componentFlags );

        properties->setProperty( "state", static_cast<s32>( getState() ) );

        return properties;
    }

    void Component::setProperties( SmartPtr<Properties> properties )
    {
        auto enabled = isEnabled();
        properties->getPropertyValue( enabledStr, enabled );

        auto dirty = true;
        properties->getPropertyValue( dirtyStr, dirty );

        u32 componentFlags = 0;
        properties->getPropertyValue( "ComponentFlags", componentFlags );
        m_componentFlags = 0;

        if( isEnabled() != enabled )
        {
            setEnabled( enabled );
            updateVisibility();
        }
    }

    void Component::updateTransform()
    {
    }

    void Component::updateTransform( const Transform3<real_Num> &transform )
    {
    }

    void Component::updateVisibility()
    {
    }

    void Component::updateMaterials()
    {
    }

    void Component::setState( State state )
    {
        if( m_componentFSM )
        {
            m_componentFSM->setState( state );
        }
    }

    auto Component::getState() const -> IComponent::State
    {
        if( m_componentFSM )
        {
            return m_componentFSM->getState<State>();
        }

        return State::Count;
    }

    auto Component::getEvents() const -> Array<SmartPtr<IComponentEvent>>
    {
        return m_events;
    }

    void Component::setEvents( Array<SmartPtr<IComponentEvent>> events )
    {
        m_events = events;
    }

    void Component::addEvent( SmartPtr<IComponentEvent> event )
    {
        m_events.push_back( event );
    }

    void Component::removeEvent( SmartPtr<IComponentEvent> event )
    {
        auto it = std::find( m_events.begin(), m_events.end(), event );
        if( it != m_events.end() )
        {
            m_events.erase( it );
        }
    }

    void Component::addSubComponent( SmartPtr<ISubComponent> child )
    {
        auto p = getSubComponentsPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>();
            setSubComponentsPtr( p );
        }

        if( p )
        {
            child->setParent( this );

            auto &children = *p;
            children.push_back( child );
        }
    }

    void Component::removeSubComponent( SmartPtr<ISubComponent> child )
    {
        child->setParent( nullptr );

        if( auto p = getSubComponentsPtr() )
        {
            auto children = Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
            auto it = std::find( children.begin(), children.end(), child );
            if( it != children.end() )
            {
                children.erase( it );

                auto newPtr = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>(
                    children.begin(), children.end() );
                setSubComponentsPtr( newPtr );
            }
        }
    }

    void Component::removeSubComponentByIndex( u32 index )
    {
        if( auto p = getSubComponentsPtr() )
        {
            auto children = Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
            auto it = children.begin();
            std::advance( it, index );

            if( it != children.end() )
            {
                children.erase( it );

                auto newPtr = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>(
                    children.begin(), children.end() );
                setSubComponentsPtr( newPtr );
            }
        }
    }

    auto Component::getNumSubComponents() const -> u32
    {
        if( auto p = getSubComponentsPtr() )
        {
            return static_cast<u32>( p->size() );
        }

        return 0;
    }

    auto Component::getSubComponentByIndex( u32 index ) const -> SmartPtr<ISubComponent>
    {
        return m_children->at( index );
    }

    auto Component::getSubComponents() const -> Array<SmartPtr<ISubComponent>>
    {
        if( auto p = getSubComponentsPtr() )
        {
            return Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
        }

        return {};
    }

    void Component::setSubComponents( Array<SmartPtr<ISubComponent>> children )
    {
        auto p = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>( children.begin(),
                                                                            children.end() );
        setSubComponentsPtr( p );
    }

    auto Component::getSubComponentsPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>>
    {
        return m_children;
    }

    void Component::setSubComponentsPtr( SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children )
    {
        m_children = children;
    }

    Component::ComponentStateListener::ComponentStateListener() = default;

    Component::ComponentStateListener::~ComponentStateListener() = default;

    void Component::ComponentStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    void Component::ComponentStateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
    }

    void Component::ComponentStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto Component::ComponentStateListener::getComponent() const -> Component *
    {
        return m_component;
    }

    void Component::ComponentStateListener::setComponent( Component *component )
    {
        m_component = component;
    }

    Component::ComponentFSMListener::ComponentFSMListener() = default;

    Component::ComponentFSMListener::~ComponentFSMListener() = default;

    void Component::ComponentFSMListener::unload( SmartPtr<ISharedObject> data )
    {
        setOwner( nullptr );
    }

    auto Component::ComponentFSMListener::handleEvent( u32 state, IFSM::Event eventType )
        -> IFSM::ReturnType
    {
        if( auto owner = getOwner() )
        {
            return owner->handleComponentEvent( state, eventType );
        }

        return IFSM::ReturnType::NotHandled;
    }

    auto Component::ComponentFSMListener::getOwner() const -> SmartPtr<Component>
    {
        return m_owner;
    }

    void Component::ComponentFSMListener::setOwner( SmartPtr<Component> owner )
    {
        m_owner = owner;
    }

    auto Component::getComponentState() -> SmartPtr<IState> &
    {
        return m_componentState;
    }

    auto Component::getComponentState() const -> const SmartPtr<IState> &
    {
        return m_componentState;
    }

    void Component::setComponentState( SmartPtr<IState> state )
    {
        m_componentState = state;
    }

    auto Component::compareTag( const String &tag ) const -> bool
    {
        if( auto actor = getActor() )
        {
            return actor->compareTag( tag );
        }

        return false;
    }

    auto Component::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                 const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                 SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IComponent::actorUnload )
        {
            setActor( nullptr );
        }
        else if( eventValue == IComponent::hierarchyChanged )
        {
            updateTransform();
        }
        else if( eventValue == IComponent::actorFlagsChanged )
        {
            auto oldFlags = arguments[0].getU32();
            auto newFlags = arguments[1].getU32();

            updateFlags( newFlags, oldFlags );
        }

        return {};
    }
}  // namespace fb::scene
