#ifndef ComponentEventListener_h__
#define ComponentEventListener_h__

#include <FBCore/Interface/Scene/IComponentEventListener.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace scene
    {
        class ComponentEventListener : public SharedObject<IComponentEventListener>
        {
        public:
            ComponentEventListener();
            ~ComponentEventListener() override;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object,
                                   SmartPtr<IEvent> event ) override;

            SmartPtr<IComponent> getComponent() const override;

            void setComponent( SmartPtr<IComponent> component ) override;

            String getFunction() const override;

            void setFunction( const String &function ) override;

            /** @copydoc IObject::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IObject::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<IComponentEvent> getEvent() const;

            void setEvent( SmartPtr<IComponentEvent> event );

        protected:
            WeakPtr<IComponentEvent> m_event;
            SmartPtr<IComponent> m_component;
            String m_function;
        };
    }  // namespace scene
}  // namespace fb

#endif  // ComponentEventListener_h__
