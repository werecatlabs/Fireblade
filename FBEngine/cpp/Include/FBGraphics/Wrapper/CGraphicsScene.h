#ifndef CGraphicsScene_h__
#define CGraphicsScene_h__

#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBGraphics/Wrapper/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        class CGraphicsScene : public CSharedGraphicsObject<IGraphicsScene>
        {
        public:
            CGraphicsScene() = default;
            ~CGraphicsScene() = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // CGraphicsScene_h__
