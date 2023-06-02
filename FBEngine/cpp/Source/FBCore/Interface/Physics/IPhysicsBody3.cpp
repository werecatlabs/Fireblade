#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Physics/IPhysicsBody3.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    namespace physics
    {
        const hash_type IPhysicsBody3::STATE_MESSAGE_ATTACH_SHAPE = StringUtil::getHash( "attachShape" );
        const hash_type IPhysicsBody3::STATE_MESSAGE_DETACH_SHAPE = StringUtil::getHash( "detachShape" );
        const hash_type IPhysicsBody3::STATE_MESSAGE_MASS = StringUtil::getHash( "mass" );
        const hash_type IPhysicsBody3::STATE_MESSAGE_INERTIA_TENSOR = StringUtil::getHash( "inertia" );

        FB_CLASS_REGISTER_DERIVED( fb, IPhysicsBody3, ISharedObject );
    }  // end namespace physics
}  // end namespace fb
