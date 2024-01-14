#ifndef _IWindowListener_H
#define _IWindowListener_H

#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace render
    {

        /**
         * Listens for render window events.
         */
        class IWindowListener : public IEventListener
        {
        public:
            static const hash_type windowClosingHash;
            static const hash_type windowResizedHash;
            static const hash_type windowMovedHash;

            /** Virtual destructor. */
            ~IWindowListener() override = default;

            /**
             * Handles the event.
             *
             * @param eventType The type of the event.
             * @param eventValue The value of the event.
             * @param arguments The arguments of the event.
             * @param sender The event sender.
             * @param event The event itself.
             *
             * @return The result of the event handling.
             */
            virtual Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                           const Array<Parameter> &arguments,
                                           SmartPtr<ISharedObject> sender,
                                           SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) = 0;

            /**
             * Handles the window event.
             *
             * @param event The window event.
             */
            virtual void handleEvent( SmartPtr<IWindowEvent> event ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
