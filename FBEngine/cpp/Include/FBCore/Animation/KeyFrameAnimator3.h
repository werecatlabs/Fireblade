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

        void addKeyFrame( const SmartPtr<KeyFrameTransform3> &keyFrame );

        void addKeyFrames( const Array<SmartPtr<KeyFrameTransform3>> &keyFrames );

        void setAnimationLength( f32 animationLength );

        SmartPtr<KeyFrameTransform3> getCurrentKeyFrame() const;

    private:
        SmartPtr<KeyFrameTransform3> m_currentKeyFrame;
        Array<SmartPtr<KeyFrameTransform3>> m_keyFrames;

        LinearSpline3F m_positionSpline;
        RotationalSpline3F m_orientationSpline;
    };

}  // end namespace fb

#endif  // KeyFrameAnimator_h__
