#ifndef _IStateMessage_H_
#define _IStateMessage_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /// An interface for a state message.
    class IStateMessage : public ISharedObject
    {
    public:
        static const hash_type STATE_MESSAGE_LEFT;
        static const hash_type STATE_MESSAGE_TOP;
        static const hash_type STATE_MESSAGE_WIDTH;
        static const hash_type STATE_MESSAGE_HEIGHT;
        static const hash_type STATE_MESSAGE_METRICSMODE;
        static const hash_type STATE_MESSAGE_ALIGN_HORIZONTAL;
        static const hash_type STATE_MESSAGE_ALIGN_VERTICAL;
        static const hash_type STATE_MESSAGE_TEXT;

        /** A virtual destructor. */
        ~IStateMessage() override = default;

        /** Gets the type of state message. This is useful for casting. */
        virtual hash_type getType() const = 0;

        /** Sets the type of state message. */
        virtual void setType( hash_type type ) = 0;

        /** Gets the sender. */
        virtual SmartPtr<ISharedObject> getSender() const = 0;

        /** Sets the sender. */
        virtual void setSender( SmartPtr<ISharedObject> object ) = 0;

        /** Gets the state object. */
        virtual SmartPtr<IStateContext> getStateObject() const = 0;

        /** Sets the state object. */
        virtual void setStateObject( SmartPtr<IStateContext> object ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
