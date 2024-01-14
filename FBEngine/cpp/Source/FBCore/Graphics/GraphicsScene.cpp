#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/GraphicsScene.h>

namespace fb::render
{

    GraphicsScene::GraphicsScene()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "GraphicsScene" );
        }
    }

    void GraphicsScene::load( SmartPtr<ISharedObject> data )
    {
    }

    void GraphicsScene::unload( SmartPtr<ISharedObject> data )
    {
        SharedGraphicsObject<IGraphicsScene>::unload( data );
    }

}  // namespace fb::render
