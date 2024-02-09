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
            Vector2<real_Num> calculateElementPosition( const Vector2<real_Num> &parentPosition,
                                               const Vector2<real_Num> &parentSize, const Vector2<real_Num> &position,
                                               const Vector2<real_Num> &size, const Vector2<real_Num> &anchor,
                                               const Vector2<real_Num> &anchorMin, const Vector2<real_Num> &anchorMax );

            /** Calculates the size of an element. */
            void calculateElementPositionAndSize( const Vector2<real_Num> &parentPosition,
                                                  const Vector2<real_Num> &parentSize, const Vector2<real_Num> &position,
                                                  const Vector2<real_Num> &size, const Vector2<real_Num> &anchor,
                                                  const Vector2<real_Num> &anchorMin, const Vector2<real_Num> &anchorMax,
                                                  Vector2<real_Num> &elementPosition, Vector2<real_Num> &elementSize );

            /** Calculates the size of an element. */
            void calculateElementPositionAndSize( const Vector2<real_Num> &position, const Vector2<real_Num> &size,
                                                  const Vector2<real_Num> &anchor, const Vector2<real_Num> &anchorMin,
                                                  const Vector2<real_Num> &anchorMax, Vector2<real_Num> &elementPosition,
                                                  Vector2<real_Num> &elementSize );

            /** Calculates the size of an element. */
            void calculateElementPositionAndSize( const Vector2<real_Num> &parentPosition,
                                                  const Vector2<real_Num> &parentSize, const Vector2<real_Num> &position,
                                                  const Vector2<real_Num> &size, const Vector2<real_Num> &anchor,
                                                  const Vector2<real_Num> &anchorMin, const Vector2<real_Num> &anchorMax,
                                                  f32 &left, f32 &right, f32 &top, f32 &bottom );
        };
    } // namespace scene
}     // namespace fb

#endif  // CanvasTransformSystem_h__
