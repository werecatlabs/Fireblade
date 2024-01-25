#ifndef IAnimation_h__
#define IAnimation_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief Interface for an animation.
     */
    class IAnimation : public ISharedObject
    {
    public:
        /**
         * @brief The types of animation interpolation available.
         */
        enum class InterpolationMode
        {
            /**
             * @brief Values are interpolated along straight lines.
             */
            IM_LINEAR,

            /**
             * @brief Values are interpolated along a spline, resulting in smoother changes in direction.
             */
            IM_SPLINE
        };

        /**
         * @brief The types of rotational interpolation available.
         */
        enum class RotationInterpolationMode
        {
            /**
             * @brief Values are interpolated linearly. This is faster but does not
             * necessarily give a completely accurate result.
             */
            RIM_LINEAR,

            /**
             * @brief Values are interpolated spherically. This is more accurate but
             * has a higher cost.
             */
            RIM_SPHERICAL
        };

        /**
         * @brief Virtual destructor.
         */
        ~IAnimation() override = default;

        /**
         * @brief Gets the total length of the animation.
         * @return Returns the length of the animation as a floating-point value.
         */
        virtual f32 getLength() const = 0;

        /**
         * @brief Sets the length of the animation.
         * @param length The new length for the animation as a floating-point value.
         * @note Changing the length of an animation may invalidate existing AnimationState
         * instances, which will need to be recreated.
         */
        virtual void setLength( f32 length ) = 0;
    };

}  // end namespace fb

#endif  // IAnimation_h__
