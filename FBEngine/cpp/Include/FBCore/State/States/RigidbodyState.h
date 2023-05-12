#ifndef RigidbodyState_h__
#define RigidbodyState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/FixedArray.h>

namespace fb
{
    class RigidbodyState : public BaseState
    {
    public:
        RigidbodyState();
        ~RigidbodyState() override;

        Transform3<real_Num> getTransform() const;
        void setTransform( const Transform3<real_Num>& transform );
        void updateTransform( const Transform3<real_Num> &transform );

    protected:
        Transform3<real_Num> m_transform;

        Vector3<real_Num> position;
        Quaternion<real_Num> orientation;

        Vector3<real_Num> linearVelocity;
        Vector3<real_Num> angularVelocity;

        real_Num mass = static_cast<real_Num>(1.0);

        FB_CLASS_REGISTER_DECL;
    };
} // end namespace fb

#endif  // RigidbodyState_h__
