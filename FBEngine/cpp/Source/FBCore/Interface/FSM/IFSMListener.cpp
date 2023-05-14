#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IFSMListener, ISharedObject );
}  // end namespace fb
