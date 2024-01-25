#ifndef CanvasTransformSystem_h__
#define CanvasTransformSystem_h__

#include <FBCore/Scene/Systems/ComponentSystem.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace scene
    {

        /** Component system for a layout transform. */
        class LayoutTransformSystem : public ComponentSystem
        {
        public:
            /** @brief Constructor. */
            LayoutTransformSystem();

            /** @brief Destructor. */
            ~LayoutTransformSystem() override;

            /** @copydoc ComponentSystem::update */
            void update() override;

        protected:
            /** Calculates the position of an element. */
            Vector2F calculateElementPosition( const Vector2F &parentPosition,
                                               const Vector2F &parentSize, const Vector2F &position,
                                               const Vector2F &size, const Vector2F &anchor,
                                               const Vector2F &anchorMin, const Vector2F &anchorMax );

            /** Calculates the size of an element. */
            void calculateElementPositionAndSize( const Vector2F &parentPosition,
                                                  const Vector2F &parentSize, const Vector2F &position,
                                                  const Vector2F &size, const Vector2F &anchor,
                                                  const Vector2F &anchorMin, const Vector2F &anchorMax,
                                                  Vector2F &elementPosition, Vector2F &elementSize );

            /** Calculates the size of an element. */
            void calculateElementPositionAndSize( const Vector2F &position, const Vector2F &size,
                                                  const Vector2F &anchor, const Vector2F &anchorMin,
                                                  const Vector2F &anchorMax, Vector2F &elementPosition,
                                                  Vector2F &elementSize );

            /** Calculates the size of an element. */
            void calculateElementPositionAndSize( const Vector2F &parentPosition,
                                                  const Vector2F &parentSize, const Vector2F &position,
                                                  const Vector2F &size, const Vector2F &anchor,
                                                  const Vector2F &anchorMin, const Vector2F &anchorMax,
                                                  f32 &left, f32 &right, f32 &top, f32 &bottom );
        };
    } // namespace scene
}     // namespace fb

#endif  // CanvasTransformSystem_h__
