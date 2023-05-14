#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Input/IGameInput.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IGameInput, ISharedObject );
    const u32 IGameInput::UNASSIGNED = std::numeric_limits<u32>::max();
}  // end namespace fb


