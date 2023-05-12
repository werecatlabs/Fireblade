#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Input/IMouseState.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IMouseState, ISharedObject );

    const u32 IMouseState::MOUSE_LEFT_BUTTON = ( 1 << 0 );
    const u32 IMouseState::MOUSE_RIGHT_BUTTON = ( 1 << 1 );
    const u32 IMouseState::MOUSE_MIDDLE_BUTTON = ( 1 << 2 );

}  // end namespace fb

