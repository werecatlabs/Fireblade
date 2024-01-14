#ifndef CGraphicsScene_h__
#define CGraphicsScene_h__

#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        class GraphicsScene : public SharedGraphicsObject<IGraphicsScene>
        {
        public:
            GraphicsScene();
            ~GraphicsScene() = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;
        };

    }  // namespace render
}  // namespace fb

#endif  // CGraphicsScene_h__
