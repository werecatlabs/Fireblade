#ifndef ShapeState_h__
#define ShapeState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{

    class ShapeState : public BaseState
    {
    public:
        ShapeState();
        ~ShapeState() override;

        void setLocalPose( const Transform3<real_Num> &pose );

        Transform3<real_Num> getLocalPose() const;

        FB_CLASS_REGISTER_DECL;

    protected:
        Transform3<real_Num> m_localPose;
    };

}  // end namespace fb

#endif  // ShapeState_h__
