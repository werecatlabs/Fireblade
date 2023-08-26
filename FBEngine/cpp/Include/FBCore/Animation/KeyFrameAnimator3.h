#ifndef KeyFrameAnimator_h__
#define KeyFrameAnimator_h__

#include "FBCore/Animation/BaseAnimator.h"
#include "FBCore/Math/LinearSpline3.h"
#include "FBCore/Math/RotationalSpline3.h"
#include "FBCore/Animation/KeyFrameTransform3.h"

namespace fb
{

    class KeyFrameAnimator3 : public BaseAnimator
    {
    public:
        KeyFrameAnimator3();
        ~KeyFrameAnimator3();

        void update();

        void addKeyFrame( const KeyFrameTransform3Ptr &keyFrame );

        void addKeyFrames( const Array<KeyFrameTransform3Ptr> &keyFrames );

        void setAnimationLength( f32 animationLength );

        KeyFrameTransform3Ptr getCurrentKeyFrame() const
        {
            return m_currentKeyFrame;
        }

    private:
        KeyFrameTransform3Ptr m_currentKeyFrame;
        Array<KeyFrameTransform3Ptr> m_keyFrames;

        LinearSpline3F m_positionSpline;
        RotationalSpline3F m_orientationSpline;
    };

}  // end namespace fb

#endif  // KeyFrameAnimator_h__
