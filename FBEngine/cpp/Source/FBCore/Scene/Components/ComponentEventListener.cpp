#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/ComponentEventListener.h>
#include <FBCore/Scene/Components/UserComponent.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        ComponentEventListener::ComponentEventListener()
        {
        }

        ComponentEventListener::~ComponentEventListener()
        {
        }

        Parameter ComponentEventListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                       const Array<Parameter> &arguments,
                                                       SmartPtr<ISharedObject> sender,
                                                       SmartPtr<ISharedObject> object,
                                                       SmartPtr<IEvent> event )
        {
            if(auto event = getEvent())
            {
                if(eventValue == event->getEventHash())
                {
                    ////todo hack
                    //auto applicationManager = core::IApplicationManager::instance();
                    //FB_ASSERT( applicationManager );

                    //applicationManager->getSceneManager()->loadScene( "Workshop.fbscene" );

                    if(auto component = getComponent())
                    {
                        if(component->isDerived<UserComponent>())
                        {
                            auto userComponent = fb::static_pointer_cast<UserComponent>( component );
                            if(userComponent->getState() == IComponent::State::Edit)
                            {
                                if(userComponent->getUpdateInEditMode())
                                {
                                    if(auto invoker = userComponent->getInvoker())
                                    {
                                        const auto functionName = getFunction();
                                        invoker->callObjectMember( functionName );
                                    }
                                }
                            }
                            else
                            {
                                if(auto invoker = userComponent->getInvoker())
                                {
                                    const auto functionName = getFunction();
                                    invoker->callObjectMember( functionName );
                                }
                            }
                        }
                    }
                }
            }

            return Parameter();
        }

        SmartPtr<IComponent> ComponentEventListener::getComponent() const
        {
            return m_component;
        }

        void ComponentEventListener::setComponent( SmartPtr<IComponent> component )
        {
            m_component = component;
        }

        String ComponentEventListener::getFunction() const
        {
            return m_function;
        }

        void ComponentEventListener::setFunction( const String &function )
        {
            m_function = function;
        }

        SmartPtr<ISharedObject> ComponentEventListener::toData() const
        {
            return nullptr;
        }

        void ComponentEventListener::fromData( SmartPtr<ISharedObject> data )
        {
        }

        SmartPtr<Properties> ComponentEventListener::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();
            FB_ASSERT( properties );

            static const auto functionStr = String( "function" );
            static const auto componentStr = String( "component" );

            properties->setProperty( componentStr, m_component );
            properties->setProperty( functionStr, m_function );

            return properties;
        }

        void ComponentEventListener::setProperties( SmartPtr<Properties> properties )
        {
            static const auto functionStr = String( "function" );
            static const auto componentStr = String( "component" );

            SmartPtr<IComponent> component;
            properties->getPropertyValue( componentStr, component );
            properties->getPropertyValue( functionStr, m_function );

            m_component = component;
        }

        SmartPtr<IComponentEvent> ComponentEventListener::getEvent() const
        {
            return m_event.lock();
        }

        void ComponentEventListener::setEvent( SmartPtr<IComponentEvent> event )
        {
            m_event = event;
        }
    } // namespace scene
}     // namespace fb
