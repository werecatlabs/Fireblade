#ifndef _FB_IEventListener_h__
#define _FB_IEventListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/Parameter.h>
#include <FBCore/Interface/System/IEvent.h>

namespace fb
{
    /** Interface for an event listener. */
    class IEventListener : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IEventListener() override = default;

        /** Handles an event.
         @param eventType The event type.
         @param eventValue The event value.
         @param sender The object triggering the event. This can be null.
         @param event The event data. This can be null.
         @return Contains a return parameter. */
        virtual Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object,
                                       SmartPtr<IEvent> event ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // _FB_IEventListener_h__
