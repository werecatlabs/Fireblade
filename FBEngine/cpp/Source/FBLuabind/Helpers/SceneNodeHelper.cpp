#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuaBind/Helpers/SceneNodeHelper.h"

namespace fb
{

    void SceneNodeHelper::setMaterialName( ISceneNode *node, const String &materialName )
    {
        //node->setMaterialName( materialName );
    }

    void SceneNodeHelper::setMaterialNameCascade( ISceneNode *node, const String &materialName,
                                                  bool cascade )
    {
        //node->setMaterialName( materialName, cascade );
    }

    void SceneNodeHelper::_setVisibilityFlags( ISceneNode *node, lua_Integer flag )
    {
        u32 mask = *reinterpret_cast<u32 *>( &flag );
        //node->setVisibilityFlags( mask );
    }

    lua_Integer SceneNodeHelper::_getVisibilityFlags( ISceneNode *node )
    {
        u32 mask = 0;
        return *reinterpret_cast<lua_Integer *>( &mask );
    }

    fb::SmartPtr<ISceneNode> SceneNodeHelper::_addChildSceneNode( ISceneNode *node )
    {
        return node->addChildSceneNode();
    }

    fb::SmartPtr<ISceneNode> SceneNodeHelper::_addChildSceneNodeNamed( ISceneNode *node,
                                                                       const char *name )
    {
        return node->addChildSceneNode( name );
    }

    void SceneNodeHelper::_attachObject( ISceneNode *node, SmartPtr<IGraphicsObject> const &obj )
    {
        node->attachObject( obj );
    }

    void SceneNodeHelper::_attachMesh( ISceneNode *node, SmartPtr<IGraphicsMesh> const &obj )
    {
        node->attachObject( obj );
    }

    void SceneNodeHelper::_attachCamera( ISceneNode *node, SmartPtr<render::ICamera> const &obj )
    {
        node->attachObject( obj );
    }

    void SceneNodeHelper::_setVisible( ISceneNode *node, bool isVisible )
    {
        //node->setVisible( isVisible );
    }

}  // end namespace fb
