#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IRigidStatic3.h>

#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IRigidStatic3, IRigidBody3 );
    }  // end namespace physics
}  // end namespace fb
