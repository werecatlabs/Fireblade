#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IPhysicsBody2D.h>

#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IPhysicsBody2D, ISharedObject );
    }  // end namespace physics
}  // end namespace fb
