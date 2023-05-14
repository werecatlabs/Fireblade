#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IRaycastHit.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IRaycastHit, ISharedObject );
    }  // end namespace physics
}  // end namespace fb
