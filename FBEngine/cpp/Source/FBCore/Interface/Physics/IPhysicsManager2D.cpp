#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/Physics/IPhysicsManager2D.h>

#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IPhysicsManager2D, ISharedObject );
    }  // end namespace physics
}  // end namespace fb
