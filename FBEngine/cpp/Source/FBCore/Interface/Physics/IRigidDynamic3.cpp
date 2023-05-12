#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IRigidDynamic3.h>

#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IRigidDynamic3, IRigidBody3 );
    }  // end namespace physics
}  // end namespace fb
