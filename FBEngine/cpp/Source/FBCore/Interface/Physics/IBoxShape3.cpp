#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IBoxShape3.h>

#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IBoxShape3, IPhysicsShape3 );
    }  // end namespace physics
}  // end namespace fb
