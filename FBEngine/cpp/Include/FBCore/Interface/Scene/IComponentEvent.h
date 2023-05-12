#ifndef _FB_IComponentEvent_h__
#define _FB_IComponentEvent_h__

#include <FBCore/Interface/System/IEvent.h>

namespace fb
{
    namespace scene
    {
        /** Interface for an event. */
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

            virtual void addListener( SmartPtr<IComponentEventListener> listener ) = 0;

            virtual void removeListener( SmartPtr<IComponentEventListener> listener ) = 0;

            virtual void removeListeners() = 0;

            virtual Array<SmartPtr<IComponentEventListener>> getListeners() const = 0;

            virtual void setListeners( const Array<SmartPtr<IComponentEventListener>> &listeners ) = 0;

            virtual String getLabel() const = 0;

            virtual void setLabel( const String &label ) = 0;

            virtual hash_type getEventHash() const = 0;
            virtual void setEventHash( hash_type eventHash ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // IEvent_h__
