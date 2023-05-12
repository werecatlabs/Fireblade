#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IPhysicsShape.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, IPhysicsShape, ISharedObject );

        const hash_type IPhysicsShape::CREATE_SHAPE_HASH = StringUtil::getHash( "createShape" );

    }  // end namespace physics
}  // end namespace fb
