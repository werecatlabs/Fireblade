#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IPhysicsShape.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::physics
{
    FB_CLASS_REGISTER_DERIVED( fb, IPhysicsShape, ISharedObject );

    const hash_type IPhysicsShape::CREATE_SHAPE_HASH = StringUtil::getHash( "createShape" );

}  // namespace fb::physics
