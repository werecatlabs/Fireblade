#ifndef TransformState_h__
#define TransformState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Atomics/Atomics.h>

namespace fb
{
    class TransformState : public BaseState
    {
    public:
        TransformState() = default;
        ~TransformState() override = default;

        Vector3<real_Num> getPosition() const;

        void setPosition( const Vector3<real_Num> &position );

        Vector3<real_Num> getScale() const;

        void setScale( const Vector3<real_Num> &scale );

        Quaternion<real_Num> getOrientation() const;

        void setOrientation( const Quaternion<real_Num> &orientation );

        Transform3<real_Num> getActualTransform() const;

        void setActualTransform( const Transform3<real_Num> &actualTransform );

        Transform3<real_Num> getLocalTransform() const;

        void setLocalTransform( const Transform3<real_Num> &localTransform );

        SmartPtr<IState> clone();

        FB_CLASS_REGISTER_DECL;

    protected:
        Transform3<real_Num> m_localTransform;
        Transform3<real_Num> m_actualTransform;
    };
}  // end namespace fb

#endif  // TransformState_h__
