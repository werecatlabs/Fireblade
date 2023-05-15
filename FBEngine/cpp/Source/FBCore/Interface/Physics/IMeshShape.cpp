#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IMeshShape.h>

#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IMeshShape, IPhysicsShape3 );
    }  // end namespace physics
}  // end namespace fb
