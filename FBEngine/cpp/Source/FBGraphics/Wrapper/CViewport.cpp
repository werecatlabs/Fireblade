#include <FBGraphics/Wrapper/CViewport.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        CViewport::CViewport()
        {
        }

        CViewport::~CViewport()
        {
        }

        void CViewport::load( SmartPtr<ISharedObject> data )
        {
            CSharedGraphicsObject<IViewport>::load( data );
        }

        void CViewport::unload( SmartPtr<ISharedObject> data )
        {
            CSharedGraphicsObject<IViewport>::unload( data );
        }

    }  // namespace render
}  // namespace fb
