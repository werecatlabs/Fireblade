#ifndef GraphicsObjectHelper_h__
#define GraphicsObjectHelper_h__

#include <FBCore/Interface/Graphics/IGraphicsObject.h>

namespace fb
{
    using namespace render;

    class GraphicsObjectHelper
    {
    public:
        static void _setMaterialName( IGraphicsObject *obj, const char *materialName );
        static void _setMaterialNameStr( IGraphicsObject *obj, String materialName );
        static void _setMaterialNameIndx( IGraphicsObject *obj, const char *materialName,
                                          lua_Integer idx );

        static void _setVisibilityFlags( IGraphicsObject *object, lua_Integer flag );

        static lua_Integer _getVisibilityFlags( IGraphicsObject *object );
    };

}  // end namespace fb

#endif  // GraphicsObjectHelper_h__
