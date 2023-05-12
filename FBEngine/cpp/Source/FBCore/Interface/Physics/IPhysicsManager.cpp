#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>

#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IPhysicsManager, ISharedObject );
    }  // end namespace physics
}  // end namespace fb
