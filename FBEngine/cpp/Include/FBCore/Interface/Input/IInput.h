#ifndef IInput_h__
#define IInput_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for input. */
    class IInput : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IInput() override = default;

        /** Sets axis input. */
        virtual void setChannel( s32 axis, f32 value ) = 0;

        /** Gets input from ranging from -1.0 to 1.0. */
        virtual f32 getChannel( s32 axis ) = 0;

        /** Gets raw input from the device. */
        virtual f32 getChannelRaw( s32 axis ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  //  namespace fb

#endif  // IInputManager_h__
