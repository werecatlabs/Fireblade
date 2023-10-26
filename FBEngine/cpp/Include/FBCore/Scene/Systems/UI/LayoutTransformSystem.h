#ifndef CanvasTransformSystem_h__
#define CanvasTransformSystem_h__

#include <FBCore/Scene/Systems/ComponentSystem.h>
#include <FBCore/Scene/Components/UI/LayoutTransform.h>

namespace fb
{
    namespace scene
    {

        class LayoutTransformSystem : public ComponentSystem
        {
        public:
            LayoutTransformSystem();
            ~LayoutTransformSystem();

            void update() override;

        protected:
            Vector2F calculateElementPosition( const Vector2F &parentPosition,
                                               const Vector2F &parentSize, const Vector2F &position,
                                               const Vector2F &size, const Vector2F &anchor,
                                               const Vector2F &anchorMin, const Vector2F &anchorMax );
            void calculateElementPositionAndSize( const Vector2F &parentPosition,
                                                  const Vector2F &parentSize, const Vector2F &position,
                                                  const Vector2F &size, const Vector2F &anchor,
                                                  const Vector2F &anchorMin, const Vector2F &anchorMax,
                                                  Vector2F &elementPosition, Vector2F &elementSize );

            void calculateElementPositionAndSize( const Vector2F &position, const Vector2F &size,
                                                  const Vector2F &anchor, const Vector2F &anchorMin,
                                                  const Vector2F &anchorMax, Vector2F &elementPosition,
                                                  Vector2F &elementSize );
            void calculateElementPositionAndSize( const Vector2F &parentPosition,
                                                  const Vector2F &parentSize, const Vector2F &position,
                                                  const Vector2F &size, const Vector2F &anchor,
                                                  const Vector2F &anchorMin, const Vector2F &anchorMax,
                                                  f32 &left, f32 &right, f32 &top, f32 &bottom );
        };

    }  // namespace scene
}  // namespace fb

#endif  // CanvasTransformSystem_h__
