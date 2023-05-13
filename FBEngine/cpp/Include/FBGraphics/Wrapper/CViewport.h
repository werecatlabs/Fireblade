#ifndef CViewport_h__
#define CViewport_h__


#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBGraphics/Wrapper/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        class CViewport : public CSharedGraphicsObject<IViewport>
        {
        public:
            CViewport();
            ~CViewport() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // CViewport_h__
