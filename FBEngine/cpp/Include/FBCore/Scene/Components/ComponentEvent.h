#ifndef ComponentEvent_h__
#define ComponentEvent_h__

#include <FBCore/Interface/Scene/IComponentEvent.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace scene
    {
        class ComponentEvent : public CSharedObject<IComponentEvent>
        {
        public:
            ComponentEvent();
            ~ComponentEvent() override;

            void addListener( SmartPtr<IComponentEventListener> listener ) override;

            void removeListener( SmartPtr<IComponentEventListener> listener ) override;

            void removeListeners() override;

            Array<SmartPtr<IComponentEventListener>> getListeners() const override;

            void setListeners( const Array<SmartPtr<IComponentEventListener>> &listeners ) override;

            String getLabel() const override;

            void setLabel( const String &label ) override;

            /** @copydoc IObject::toData */
            SmartPtr<IData> toData() const override;

            /** @copydoc IObject::fromData */
            void fromData( SmartPtr<IData> data ) override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties );

            hash_type getEventHash() const override;
            void setEventHash( hash_type eventHash ) override;

        protected:
            hash_type m_eventHash = 0;
            String m_label;
            Array<SmartPtr<IComponentEventListener>> m_listeners;
        };
    }  // namespace scene
}  // namespace fb

#endif  // ComponentEvent_h__
