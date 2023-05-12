#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/BaseComponent.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
#include <FBApplication/ApplicationUtil.h>
#include <FBCore/Scene/Components/BaseSubComponent.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, BaseComponent, CResource<IComponent> );
        u32 BaseComponent::m_idExt = 0;

        BaseComponent::BaseComponent()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto sceneManager = applicationManager->getSceneManager();
                FB_ASSERT( sceneManager );

                auto fsmManager = sceneManager->getFsmManager();
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

        BaseComponent::~BaseComponent()
        {
            unload( nullptr );
        }

        void BaseComponent::load( SmartPtr<ISharedObject> data )
        {
        }

        void BaseComponent::unload( SmartPtr<ISharedObject> data )
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
                        if( auto fsmManager = sceneManager->getFsmManager() )
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

                    CResource<IComponent>::unload( nullptr );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void BaseComponent::reload( SmartPtr<ISharedObject> data )
        {
            unload( nullptr );
            load( nullptr );
        }

        void BaseComponent::preUpdateDirty()
        {
        }

        void BaseComponent::updateDirty( u32 flags, u32 oldFlags )
        {
        }

        void BaseComponent::postUpdateDirty()
        {
        }

        void BaseComponent::setComponentFlag( u32 flag, bool value )
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

        bool BaseComponent::getComponentFlag( u32 flag )
        {
            return ( m_componentFlags & flag ) != 0;
        }

        void BaseComponent::setEnabled( bool enabled )
        {
            m_enabled = enabled;
        }

        bool BaseComponent::isEnabled() const
        {
            return m_enabled;
        }

        void BaseComponent::destroy()
        {
        }

        IFSM::ReturnType BaseComponent::handleComponentEvent( u32 state, IFSM::Event eventType )
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

        void BaseComponent::_setComponentId( hash32 val )
        {
            // m_componentId = val;
            //  m_handle->setHash(val);
        }

        hash32 BaseComponent::getComponentTypeId() const
        {
            return 0;
        }

        const String &BaseComponent::getComponentType() const
        {
            static String type = "BaseComponent";
            return type;
        }

        hash32 BaseComponent::getComponentId() const
        {
            return 0;
        }

        void BaseComponent::setDirty( bool dirty )
        {
            m_isDirty = dirty;
        }

        void BaseComponent::setDirty( Thread::Task taskId, Thread::UpdateState updateType, bool dirty )
        {
        }

        bool BaseComponent::isDirty( Thread::Task taskId, Thread::UpdateState updateType ) const
        {
            return m_isDirty;
        }

        bool BaseComponent::isDirty() const
        {
            return m_isDirty;
        }

        void BaseComponent::makeDirty()
        {
        }

        void BaseComponent::makeDirty( Thread::Task task )
        {
        }

        SmartPtr<IActor> BaseComponent::getActor() const
        {
            return m_actor.lock();
        }

        void BaseComponent::setActor( SmartPtr<IActor> actor )
        {
            m_actor = actor;
        }

        SmartPtr<IStateContext> BaseComponent::getStateObject() const
        {
            return m_stateObject;
        }

        void BaseComponent::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        void BaseComponent::reset()
        {
        }

        void BaseComponent::awake()
        {
        }

        void BaseComponent::play()
        {
        }

        void BaseComponent::edit()
        {
        }

        void BaseComponent::levelWasLoaded( s32 level )
        {
        }

        void BaseComponent::parentChanged( SmartPtr<IActor> newParent, SmartPtr<IActor> oldParent )
        {
        }

        void BaseComponent::hierarchyChanged()
        {
        }

        void BaseComponent::childAdded( SmartPtr<IActor> child )
        {
        }

        void BaseComponent::childRemoved( SmartPtr<IActor> child )
        {
        }

        void BaseComponent::childAddedInHierarchy( SmartPtr<IActor> child )
        {
        }

        void BaseComponent::childRemovedInHierarchy( SmartPtr<IActor> child )
        {
        }

        void BaseComponent::visibilityChanged()
        {
        }

        void BaseComponent::enable()
        {
        }

        void BaseComponent::disable()
        {
        }

        void BaseComponent::triggerEnter( SmartPtr<IComponent> collision )
        {
        }

        void BaseComponent::triggerLeave( SmartPtr<IComponent> collision )
        {
        }

        void BaseComponent::componentLoaded( SmartPtr<IComponent> component )
        {
        }

        void BaseComponent::actorReset()
        {
        }

        void BaseComponent::actorUnload()
        {
            setActor( nullptr );
        }

        void BaseComponent::setupUpdateRegistration()
        {
        }

        String BaseComponent::toJson() const
        {
            if( auto data = toData() )
            {
                auto componentData = data->getDataAsType<data::component_data>();
                return DataUtil::toString( componentData );
            }

            return "";
        }

        SmartPtr<IData> BaseComponent::toData() const
        {
            auto pComponentData = fb::make_ptr<Data<data::component_data>>();
            auto componentData = pComponentData->getDataAsType<data::component_data>();

            if( auto handle = getHandle() )
            {
                auto uuid = handle->getUUID();
                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                componentData->uuid = uuid;
            }

            if( auto properties = getProperties() )
            {
                auto propertiesData = properties->toData();
                componentData->properties_ = *propertiesData->getDataAsType<data::properties>();
            }

            auto subComponents = getSubComponents();
            for( auto &subComponent : subComponents )
            {
                auto subComponentData = subComponent->toData();
                auto pSubComponentData = *subComponentData->getDataAsType<data::sub_component_data>();
                componentData->subComponentData.push_back( pSubComponentData );
            }

            auto events = getEvents();
            for( auto &event : events )
            {
                data::component_eventhandler_data eventData;

                auto listeners = event->getListeners();
                for( auto &listener : listeners )
                {
                    if( auto properties = listener->getProperties() )
                    {
                        auto propertiesData = properties->toData();
                        eventData.listeners.push_back(
                            *propertiesData->getDataAsType<data::properties>() );
                    }
                }

                componentData->events.push_back( eventData );
            }

            return pComponentData;
        }

        void BaseComponent::fromData( SmartPtr<IData> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto componentData = data->getDataAsType<data::component_data>();
            auto name = componentData->name;

            if( auto handle = getHandle() )
            {
                auto uuid = componentData->uuid;
                if( StringUtil::isNullOrEmpty( uuid ) )
                {
                    uuid = StringUtil::getUUID();
                }

                handle->setUUID( uuid );
            }

            if( auto pProperties = PropertiesUtil::getProperties( &componentData->properties_ ) )
            {
                sceneManager->queueProperties( this, pProperties );
                //setProperties( pProperties );
            }

            //auto& subComponentData = componentData->subComponentData;
            //for(auto &rSubComponentData : subComponentData)
            //{
            //    rSubComponentData.componentType
            //}

            auto components = Array<SmartPtr<ISubComponent>>();
            components.reserve( componentData->subComponentData.size() );

            for( auto &component : componentData->subComponentData )
            {
                auto pComponent =
                    factoryManager->createObjectFromType<BaseSubComponent>( component.componentType );
                if( !pComponent )
                {
                    auto componentType = StringUtil::replaceAll( component.componentType, "fb::", "" );
                    pComponent = factoryManager->createObjectFromType<ISubComponent>( componentType );
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

            for( size_t i = 0; i < components.size(); ++i )
            {
                try
                {
                    auto &pComponent = components[i];
                    auto &pComponentData = componentData->subComponentData[i];
                    // FB_ASSERT(pComponent);

                    if( pComponent )
                    {
                        pComponent->setParent( this );

                        auto componentData = factoryManager->make_ptr<Data<data::sub_component_data>>();
                        componentData->setData( &pComponentData );

                        pComponent->fromData( componentData );

                        pComponent->load( nullptr );
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            auto events = getEvents();

            auto count = 0;
            for( auto &rEvent : componentData->events )
            {
                auto event = events[count];
                event->removeListeners();

                for( auto &rListener : rEvent.listeners )
                {
                    auto eventListener = fb::make_ptr<ComponentEventListener>();
                    eventListener->setEvent( event );

                    if( auto pProperties = PropertiesUtil::getProperties( &rListener ) )
                    {
                        sceneManager->queueProperties( eventListener, pProperties );
                        //eventListener->setProperties( pProperties );
                    }

                    event->addListener( eventListener );
                }

                count++;
            }
        }

        Array<SmartPtr<ISharedObject>> BaseComponent::getChildObjects() const
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

        SmartPtr<Properties> BaseComponent::getProperties() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto properties = CResource<IComponent>::getProperties();

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

        void BaseComponent::setProperties( SmartPtr<Properties> properties )
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

        void BaseComponent::updateTransform()
        {
        }

        void BaseComponent::updateVisibility()
        {
        }

        bool BaseComponent::compareTag( const String &tag ) const
        {
            return false;
        }

        void BaseComponent::handleActorReparent()
        {
        }

        void BaseComponent::setState( State state )
        {
            if( m_componentFSM )
            {
                m_componentFSM->setState( state );
            }
        }

        IComponent::State BaseComponent::getState() const
        {
            if( m_componentFSM )
            {
                return m_componentFSM->getState<State>();
            }

            return State::Count;
        }

        u32 BaseComponent::getEntity() const
        {
            return m_entity;
        }

        void BaseComponent::setEntity( u32 entity )
        {
            m_entity = entity;
        }

        void *BaseComponent::getDataPtr() const
        {
            return m_dataPtr;
        }

        void BaseComponent::setDataPtr( void *data )
        {
            m_dataPtr = data;
        }

        Array<SmartPtr<IComponentEvent>> BaseComponent::getEvents() const
        {
            return m_events;
        }

        void BaseComponent::setEvents( Array<SmartPtr<IComponentEvent>> events )
        {
            m_events = events;
        }

        void BaseComponent::addEvent( SmartPtr<IComponentEvent> event )
        {
            m_events.push_back( event );
        }

        void BaseComponent::removeEvent( SmartPtr<IComponentEvent> event )
        {
            auto it = std::find( m_events.begin(), m_events.end(), event );
            if( it != m_events.end() )
            {
                m_events.erase( it );
            }
        }

        void BaseComponent::addSubComponent( SmartPtr<ISubComponent> child )
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

        void BaseComponent::removeSubComponent( SmartPtr<ISubComponent> child )
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

        void BaseComponent::removeSubComponentByIndex( u32 index )
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

        u32 BaseComponent::getNumSubComponents() const
        {
            if( auto p = getSubComponentsPtr() )
            {
                return p->size();
            }

            return 0;
        }

        Array<SmartPtr<ISubComponent>> BaseComponent::getSubComponents() const
        {
            if( auto p = getSubComponentsPtr() )
            {
                return Array<SmartPtr<ISubComponent>>( p->begin(), p->end() );
            }

            return Array<SmartPtr<ISubComponent>>();
        }

        void BaseComponent::setSubComponents( Array<SmartPtr<ISubComponent>> children )
        {
            auto p = fb::make_shared<ConcurrentArray<SmartPtr<ISubComponent>>>( children.begin(),
                                                                                children.end() );
            setSubComponentsPtr( p );
        }

        SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> BaseComponent::getSubComponentsPtr() const
        {
            return m_children;
        }

        void BaseComponent::setSubComponentsPtr(
            SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children )
        {
            m_children = children;
        }

        SmartPtr<IDirector> BaseComponent::getDirector() const
        {
            return m_director;
        }

        void BaseComponent::setDirector( SmartPtr<IDirector> director )
        {
            m_director = director;
        }

        BaseComponent::BaseComponentStateListener::BaseComponentStateListener()
        {
        }

        BaseComponent::BaseComponentStateListener::~BaseComponentStateListener()
        {
        }

        void BaseComponent::BaseComponentStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void BaseComponent::BaseComponentStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void BaseComponent::BaseComponentStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        BaseComponent *BaseComponent::BaseComponentStateListener::getComponent() const
        {
            return m_component;
        }

        void BaseComponent::BaseComponentStateListener::setComponent( BaseComponent *component )
        {
            m_component = component;
        }

        BaseComponent::BaseComponentFSMListener::BaseComponentFSMListener()
        {
        }

        BaseComponent::BaseComponentFSMListener::~BaseComponentFSMListener()
        {
        }

        IFSM::ReturnType BaseComponent::BaseComponentFSMListener::handleEvent( u32 state,
                                                                               IFSM::Event eventType )
        {
            FB_ASSERT( m_owner );
            return m_owner->handleComponentEvent( state, eventType );
        }

        BaseComponent *BaseComponent::BaseComponentFSMListener::getOwner() const
        {
            return m_owner;
        }

        void BaseComponent::BaseComponentFSMListener::setOwner( BaseComponent *owner )
        {
            m_owner = owner;
        }
    }  // namespace scene
}  // end namespace fb
