#include <FBGraphics/Wrapper/CGraphicsScene.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        void CGraphicsScene::load( SmartPtr<ISharedObject> data )
        {
        }

        void CGraphicsScene::unload( SmartPtr<ISharedObject> data )
        {
            CSharedGraphicsObject<IGraphicsScene>::unload( data );
        }

    }  // namespace render
}  // namespace fb
