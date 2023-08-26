#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
#include <FBCore/Scene/Components/SubComponent.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Interface/Scene/IComponentEvent.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/System/IState.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, Component, Resource<IComponent> );
        u32 Component::m_idExt = 0;

        Component::Component()
        {
        }

        Component::~Component()
        {
            unload( nullptr );
        }

        void Component::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto fsmManager = sceneManager->getComponentFsmManager( getTypeInfo() );
                FB_ASSERT( fsmManager );

                m_componentFSM = fsmManager->createFSM();

                auto componentFsmListener = factoryManager->make_ptr<BaseComponentFSMListener>();
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
                    auto applicationManager = core::IApplicationManager::instance();
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

                    if( m_stateObject )
                    {
                        m_stateObject->unload( data );
                        m_stateObject = nullptr;
                    }

                    if( m_stateListener )
                    {
                        m_stateListener->unload( data );
                        m_stateListener = nullptr;
                    }

                    m_actor = nullptr;
                    m_owner = nullptr;

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

        void Component::preUpdateDirty()
        {
        }

        void Component::updateDirty( u32 flags, u32 oldFlags )
        {
        }

        void Component::postUpdateDirty()
        {
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

        bool Component::getComponentFlag( u32 flag )
        {
            return ( m_componentFlags & flag ) != 0;
        }

        void Component::setEnabled( bool enabled )
        {
            m_enabled = enabled;
        }

        bool Component::isEnabled() const
        {
            return m_enabled;
        }

        IFSM::ReturnType Component::handleComponentEvent( u32 state, IFSM::Event eventType )
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

        void Component::_setComponentId( hash32 val )
        {
            // m_componentId = val;
            //  m_handle->setHash(val);
        }

        hash32 Component::getComponentTypeId() const
        {
            return 0;
        }

        const String &Component::getComponentType() const
        {
            static String type = "BaseComponent";
            return type;
        }

        hash32 Component::getComponentId() const
        {
            return 0;
        }

        void Component::setDirty( bool dirty )
        {
            m_isDirty = dirty;
        }

        void Component::setDirty( Thread::Task taskId, Thread::UpdateState updateType, bool dirty )
        {
        }

        bool Component::isDirty( Thread::Task taskId, Thread::UpdateState updateType ) const
        {
            return m_isDirty;
        }

        bool Component::isDirty() const
        {
            return m_isDirty;
        }

        void Component::makeDirty()
        {
        }

        void Component::makeDirty( Thread::Task task )
        {
        }

        SmartPtr<IActor> Component::getActor() const
        {
            return m_actor.lock();
        }

        void Component::setActor( SmartPtr<IActor> actor )
        {
            m_actor = actor;
        }

        SmartPtr<IStateContext> Component::getStateObject() const
        {
            return m_stateObject;
        }

        void Component::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        void Component::levelWasLoaded( s32 level )
        {
        }

        void Component::parentChanged( SmartPtr<IActor> newParent, SmartPtr<IActor> oldParent )
        {
        }

        void Component::hierarchyChanged()
        {
        }

        void Component::childAdded( SmartPtr<IActor> child )
        {
        }

        void Component::childRemoved( SmartPtr<IActor> child )
        {
        }

        void Component::childAddedInHierarchy( SmartPtr<IActor> child )
        {
        }

        void Component::childRemovedInHierarchy( SmartPtr<IActor> child )
        {
        }

        void Component::visibilityChanged()
        {
        }

        void Component::enable()
        {
        }

        void Component::disable()
        {
        }

        void Component::triggerEnter( SmartPtr<IComponent> collision )
        {
        }

        void Component::triggerLeave( SmartPtr<IComponent> collision )
        {
        }

        void Component::componentLoaded( SmartPtr<IComponent> component )
        {
        }

        void Component::actorReset()
        {
        }

        void Component::actorUnload()
        {
            setActor( nullptr );
        }

        SmartPtr<ISharedObject> Component::toData() const
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
                if( auto subComponentData =
                        fb::static_pointer_cast<Properties>( subComponent->toData() ) )
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
            auto applicationManager = core::IApplicationManager::instance();
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
                    pComponent =
                        factoryManager->createObjectFromType<ISubComponent>( componentTypeClean );
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

        Array<SmartPtr<ISharedObject>> Component::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> childObjects;
            childObjects.reserve( 6 );

            childObjects.push_back( m_componentFSM );
            childObjects.push_back( m_componentFsmListener );
            childObjects.push_back( m_stateObject );
            childObjects.push_back( m_stateListener );
            childObjects.push_back( m_director );

            if( auto actor = getActor() )
            {
                childObjects.push_back( actor );
            }

            if( auto owner = m_owner.lock() )
            {
                childObjects.push_back( owner );
            }

            return childObjects;
        }

        SmartPtr<Properties> Component::getProperties() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto properties = Resource<IComponent>::getProperties();

            const auto enabled = isEnabled();
            properties->setProperty( "Enabled", enabled );

            const auto dirty = isDirty();
            properties->setProperty( "Dirty", dirty );

            const u32 componentFlags = m_componentFlags;
            properties->setProperty( "ComponentFlags", componentFlags );

            properties->setProperty( "state", static_cast<s32>( getState() ) );

            auto director = fb::dynamic_pointer_cast<ISharedObject>( getDirector() );
            properties->setProperty( "director", director );

            return properties;
        }

        void Component::setProperties( SmartPtr<Properties> properties )
        {
            auto enabled = isEnabled();
            properties->getPropertyValue( "Enabled", enabled );

            auto dirty = true;
            properties->getPropertyValue( "Dirty", dirty );

            u32 componentFlags = 0;
            properties->getPropertyValue( "ComponentFlags", componentFlags );
            m_componentFlags = 0;

            SmartPtr<ISharedObject> director;
            properties->getPropertyValue( "director", director );
            m_director = fb::dynamic_pointer_cast<IDirector>( director );

            setEnabled( enabled );
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

        void Component::setState( State state )
        {
            if( m_componentFSM )
            {
                m_componentFSM->setState( state );
            }
        }

        IComponent::State Component::getState() const
        {
            if( m_componentFSM )
            {
                return m_componentFSM->getState<State>();
            }

            return State::Count;
        }

        void *Component::getDataPtr() const
        {
            return m_dataPtr;
        }

        void Component::setDataPtr( void *data )
        {
            m_dataPtr = data;
        }

        Array<SmartPtr<IComponentEvent>> Component::getEvents() const
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

        u32 Component::getNumSubComponents() const
        {
            if( auto p = getSubComponentsPtr() )
            {
                return (u32)p->size();
            }

            return 0;
        }

        Array<SmartPtr<ISubComponent>> Component::getSubComponents() const
        {
            if( auto p = getSubComponentsPtr() )
            {
                return Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
            }

            return Array<SmartPtr<ISubComponent>>();
        }

        void Component::setSubComponents( Array<SmartPtr<ISubComponent>> children )
        {
            auto p = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>( children.begin(),
                                                                                children.end() );
            setSubComponentsPtr( p );
        }

        SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> Component::getSubComponentsPtr() const
        {
            return m_children;
        }

        void Component::setSubComponentsPtr(
            SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children )
        {
            m_children = children;
        }

        SmartPtr<IDirector> Component::getDirector() const
        {
            return m_director;
        }

        void Component::setDirector( SmartPtr<IDirector> director )
        {
            m_director = director;
        }

        Component::BaseComponentStateListener::BaseComponentStateListener()
        {
        }

        Component::BaseComponentStateListener::~BaseComponentStateListener()
        {
        }

        void Component::BaseComponentStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void Component::BaseComponentStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void Component::BaseComponentStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        Component *Component::BaseComponentStateListener::getComponent() const
        {
            return m_component;
        }

        void Component::BaseComponentStateListener::setComponent( Component *component )
        {
            m_component = component;
        }

        Component::BaseComponentFSMListener::BaseComponentFSMListener()
        {
        }

        Component::BaseComponentFSMListener::~BaseComponentFSMListener()
        {
        }

        IFSM::ReturnType Component::BaseComponentFSMListener::handleEvent( u32 state,
                                                                           IFSM::Event eventType )
        {
            FB_ASSERT( m_owner );
            return m_owner->handleComponentEvent( state, eventType );
        }

        Component *Component::BaseComponentFSMListener::getOwner() const
        {
            return m_owner;
        }

        void Component::BaseComponentFSMListener::setOwner( Component *owner )
        {
            m_owner = owner;
        }

        SmartPtr<IState> &Component::getComponentState()
        {
            return m_componentState;
        }

        const SmartPtr<IState> &Component::getComponentState() const
        {
            return m_componentState;
        }

        void Component::setComponentState( SmartPtr<IState> state )
        {
            m_componentState = state;
        }

    }  // namespace scene
}  // end namespace fb
