#ifndef _FB_IComponentEvent_h__
#define _FB_IComponentEvent_h__

#include <FBCore/Interface/System/IEvent.h>

namespace fb
{
    namespace scene
    {
        /** Interface for a component event. */
        class IComponentEvent : public IEvent
        {
        public:
            enum class Type
            {
                Loading,
                Object,
                UI,

                Count
            };

            /** Virtual destructor. */
            ~IComponentEvent() override = default;

            /** Add a listener to the event. */
            virtual void addListener( SmartPtr<IComponentEventListener> listener ) = 0;

            /** Remove a listener from the event. */
            virtual void removeListener( SmartPtr<IComponentEventListener> listener ) = 0;

            /** Remove all listeners from the event. */
            virtual void removeListeners() = 0;

            /** Get the listeners of the event. */
            virtual Array<SmartPtr<IComponentEventListener>> getListeners() const = 0;

            /** Set the listeners of the event. */
            virtual void setListeners( const Array<SmartPtr<IComponentEventListener>> &listeners ) = 0;

            /** Get the type of the event. */
            virtual String getLabel() const = 0;

            /** Set the type of the event. */
            virtual void setLabel( const String &label ) = 0;

            /** Get the type of the event. */
            virtual hash_type getEventHash() const = 0;

            /** Set the type of the event. */
            virtual void setEventHash( hash_type eventHash ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // IEvent_h__
