#ifndef _FB_IInputConverter_H
#define _FB_IInputConverter_H

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IInputConverter : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IInputConverter() override = default;

        /** Gets a game input id from the name passed. */
        virtual u32 getInputId( const String &inputName ) const = 0;

        /** Gets a game name from the id passed. */
        virtual String getInputName( u32 inputId ) const = 0;

        /** Gets the id for the action name. */
        virtual u32 getActionId( const String &actionName ) const = 0;

        /** Gets an action name from the id provided. */
        virtual String getActionName( u32 actionId ) const = 0;
    };

}  // namespace fb

#endif  // _FB_IInputConverter_H
