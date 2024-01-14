#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
#include <FBCore/Scene/Components/UserComponent.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Scene/IComponentEvent.h>
#include <FBCore/Interface/Scene/IComponentEventListener.h>
#include <FBCore/Interface/Script/IScriptInvoker.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{

    const String ComponentEventListener::actorStr = String( "actor" );
    const String ComponentEventListener::functionStr = String( "function" );
    const String ComponentEventListener::componentStr = String( "component" );

    ComponentEventListener::ComponentEventListener() = default;

    ComponentEventListener::~ComponentEventListener() = default;

    auto ComponentEventListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                              const Array<Parameter> &arguments,
                                              SmartPtr<ISharedObject> sender,
                                              SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( auto event = getEvent() )
        {
            if( eventValue == event->getEventHash() )
            {
                if( auto component = getComponent() )
                {
                    if( component->isDerived<UserComponent>() )
                    {
                        if( auto actor = component->getActor() )
                        {
                            if( actor->isEnabledInScene() )
                            {
                                auto userComponent = fb::static_pointer_cast<UserComponent>( component );

#if !FB_FINAL
                                auto className = userComponent->getClassName();
                                if( StringUtil::isNullOrEmpty( className ) )
                                {
                                    FB_LOG( "Class name null." );
                                }
#endif

                                if( userComponent->getState() == IComponent::State::Edit )
                                {
                                    if( userComponent->getUpdateInEditMode() )
                                    {
                                        if( auto invoker = userComponent->getInvoker() )
                                        {
                                            const auto functionName = getFunction();
                                            invoker->callObjectMember( functionName );
                                        }
                                    }
                                }
                                else
                                {
                                    if( auto invoker = userComponent->getInvoker() )
                                    {
                                        const auto functionName = getFunction();
                                        invoker->callObjectMember( functionName );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return {};
    }

    auto ComponentEventListener::getComponent() const -> SmartPtr<IComponent>
    {
        return m_component;
    }

    void ComponentEventListener::setComponent( SmartPtr<IComponent> component )
    {
        m_component = component;
    }

    auto ComponentEventListener::getFunction() const -> String
    {
        return m_function;
    }

    void ComponentEventListener::setFunction( const String &function )
    {
        m_function = function;
    }

    auto ComponentEventListener::toData() const -> SmartPtr<ISharedObject>
    {
        return nullptr;
    }

    void ComponentEventListener::fromData( SmartPtr<ISharedObject> data )
    {
    }

    auto ComponentEventListener::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();
        FB_ASSERT( properties );

        properties->setProperty( actorStr, m_actor );
        properties->setProperty( componentStr, m_component );
        properties->setProperty( functionStr, m_function );

        return properties;
    }

    void ComponentEventListener::setProperties( SmartPtr<Properties> properties )
    {
        SmartPtr<IComponent> component;

        SmartPtr<IActor> actor;
        properties->getPropertyValue( actorStr, actor );

        properties->getPropertyValue( componentStr, component );
        properties->getPropertyValue( functionStr, m_function );

        if( m_actor != actor )
        {
            m_actor = actor;

            if( m_actor )
            {
                auto userComponent = m_actor->getComponent<UserComponent>();
                m_component = userComponent;
            }
        }
        else
        {
            m_component = component;
        }
    }

    auto ComponentEventListener::getEvent() const -> SmartPtr<IComponentEvent>
    {
        return m_event.lock();
    }

    void ComponentEventListener::setEvent( SmartPtr<IComponentEvent> event )
    {
        m_event = event;
    }
}  // namespace fb::scene
