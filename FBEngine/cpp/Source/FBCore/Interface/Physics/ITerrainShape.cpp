#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/ITerrainShape.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, ITerrainShape, IPhysicsShape3 );
    }  // end namespace physics
}  // end namespace fb
