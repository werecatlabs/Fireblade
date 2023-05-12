#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IGraphicsScene, ISharedObject );

        const u32 IGraphicsScene::VIEWPORT_MASK_TERRAIN = ( 1 << 27 );
        const u32 IGraphicsScene::VIEWPORT_MASK_OCCLUDER = ( 1 << 28 );
        const u32 IGraphicsScene::VIEWPORT_MASK_USER = ( 1 << 29 );
        const u32 IGraphicsScene::VIEWPORT_MASK_SHADOW = ( 1 << 30 );

    }  // end namespace render
}  // end namespace fb
