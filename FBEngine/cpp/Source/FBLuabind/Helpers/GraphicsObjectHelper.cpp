#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Helpers/GraphicsObjectHelper.h"
#include <FBCore/FBCore.h>

namespace fb
{

    void GraphicsObjectHelper::_setMaterialName( IGraphicsObject *obj, const char *materialName )
    {
        //obj->setMaterialName( materialName );
    }

    void GraphicsObjectHelper::_setMaterialNameStr( IGraphicsObject *obj, String materialName )
    {
        //obj->setMaterialName( materialName );
    }

    void GraphicsObjectHelper::_setMaterialNameIndx( IGraphicsObject *obj, const char *materialName,
                                                     lua_Integer idx )
    {
        //obj->setMaterialName( materialName, idx );
    }

    void GraphicsObjectHelper::_setVisibilityFlags( IGraphicsObject *object, lua_Integer flag )
    {
        u32 mask = *reinterpret_cast<u32 *>( &flag );
        object->setVisibilityFlags( mask );
    }

    lua_Integer GraphicsObjectHelper::_getVisibilityFlags( IGraphicsObject *object )
    {
        u32 mask = object->getVisibilityFlags();
        return *reinterpret_cast<lua_Integer *>( &mask );
    }

}  // end namespace fb
