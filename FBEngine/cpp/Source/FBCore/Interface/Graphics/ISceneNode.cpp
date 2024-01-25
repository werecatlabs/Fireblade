#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, ISceneNode, ISharedObject );

    const hash_type ISceneNode::STATE_QUERY_TYPE_LOCAL_AABB = StringUtil::getHash( "LocalAABB" );
    const hash_type ISceneNode::STATE_QUERY_TYPE_WORLD_AABB = StringUtil::getHash( "WorldAABB" );

    const hash_type ISceneNode::STATE_MESSAGE_POSITION = StringUtil::getHash( "position" );
    const hash_type ISceneNode::STATE_MESSAGE_SCALE = StringUtil::getHash( "scale" );
    const hash_type ISceneNode::STATE_MESSAGE_ORIENTATION = StringUtil::getHash( "orientation" );
    const hash_type ISceneNode::STATE_MESSAGE_LOOK_AT = StringUtil::getHash( "lookAt" );

    const hash_type ISceneNode::STATE_MESSAGE_ADD = StringUtil::getHash( "add" );
    const hash_type ISceneNode::STATE_MESSAGE_REMOVE = StringUtil::getHash( "remove" );

    const hash_type ISceneNode::STATE_MESSAGE_ADD_CHILD = StringUtil::getHash( "addChild" );
    const hash_type ISceneNode::STATE_MESSAGE_REMOVE_CHILD = StringUtil::getHash( "removeChild" );
    const hash_type ISceneNode::STATE_MESSAGE_ATTACH_OBJECT = StringUtil::getHash( "attachObject" );
    const hash_type ISceneNode::STATE_MESSAGE_DETACH_OBJECT = StringUtil::getHash( "detachObject" );
    const hash_type ISceneNode::STATE_MESSAGE_DETACH_ALL_OBJECTS =
        StringUtil::getHash( "detachAllObject" );
}  // namespace fb::render
