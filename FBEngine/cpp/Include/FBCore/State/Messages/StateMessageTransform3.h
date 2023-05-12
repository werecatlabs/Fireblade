#ifndef StateMessageTransform3_h__
#define StateMessageTransform3_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    class StateMessageTransform3 : public StateMessage
    {
    public:
        StateMessageTransform3();
        ~StateMessageTransform3() override;

        Transform3<real_Num> getTransform() const;
        void setTransform( const Transform3<real_Num> &val );

        FB_CLASS_REGISTER_DECL;

    private:
        Transform3<real_Num> m_transform;
    };
}  // end namespace fb

#endif  // StateMessageTransform3_h__
