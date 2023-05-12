#ifndef IGameInputState_h__
#define IGameInputState_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * Interface for a game input state.
     */
    class IGameInputState : public ISharedObject
    {
    public:
        /** Enumeration for the types of game input actions. */
        enum class Action
        {
            Chord,
            Tap,
            Sequence,
            Pressed,
            Released,
            Count
        };

        /** Virtual destructor. */
        ~IGameInputState() override = default;

        /** Gets the hash code for the game input event type. */
        virtual hash32 getEventType() const = 0;

        /** Sets the hash code for the game input event type. */
        virtual void setEventType( hash32 eventType ) = 0;

        /** Gets the hash code for the game input action that was triggered. */
        virtual hash32 getAction() const = 0;

        /** Sets the hash code for the game input action that was triggered. */
        virtual void setAction( hash32 action ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IGameInputState_h__
