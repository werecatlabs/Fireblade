#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IBoxShape2.h>

#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb::physics, IBoxShape2, IPhysicsShape2 );
    }  // end namespace physics
}  // end namespace fb
