#ifndef SceneNodeHelper_h__
#define SceneNodeHelper_h__

#include <FBCore/Interface/Graphics/ISceneNode.h>

namespace fb
{
    using namespace render;

    
    class SceneNodeHelper
    {
    public:
        static void setMaterialName( ISceneNode *node, const String &materialName );
        static void setMaterialNameCascade( ISceneNode *node, const String &materialName, bool cascade );

        static void _setVisibilityFlags( ISceneNode *node, lua_Integer flag );

        static lua_Integer _getVisibilityFlags( ISceneNode *node );

        static SmartPtr<ISceneNode> _addChildSceneNode( ISceneNode *node );
        static SmartPtr<ISceneNode> _addChildSceneNodeNamed( ISceneNode *node, const char *name );

        static void _attachObject( ISceneNode *node, SmartPtr<IGraphicsObject> const &obj );

        static void _attachMesh( ISceneNode *node, SmartPtr<IGraphicsMesh> const &obj );

        static void _attachCamera( ISceneNode *node, SmartPtr<render::ICamera> const &obj );

        static void _setVisible( ISceneNode *node, bool isVisible );
    };

}  // end namespace fb

#endif  // SceneNodeHelper_h__
