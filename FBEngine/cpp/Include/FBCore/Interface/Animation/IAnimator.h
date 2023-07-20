#ifndef _IAnimator_H
#define _IAnimator_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Use to animate an object.
     */
    class IAnimator : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAnimator() override = default;

        /** Sets the id of the animator. */
        virtual void setId( u32 id ) = 0;

        /** Gets the id of the animator. */
        virtual u32 getId() const = 0;

        /** Starts the animation. */
        virtual void start() = 0;

        /** Stops the animation. */
        virtual void stop() = 0;

        /** Sets whether the animation is looped. */
        virtual void setLoop( bool loop ) = 0;

        /** Gets whether the animation is looped. */
        virtual bool isLoop() const = 0;

        /** Sets whether the animation is reversed. */
        virtual void setReverse( bool reverse ) = 0;

        /** Gets whether the animation is reversed. */
        virtual bool isReverse() const = 0;

        /** Gets whether the animation is finished. */
        virtual bool isFinished() const = 0;

        /** */
        virtual void setAnimationLength( f32 animationLength ) = 0;
    };
}  // end namespace fb

#endif
