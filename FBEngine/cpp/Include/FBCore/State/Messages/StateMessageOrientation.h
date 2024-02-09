#ifndef StateMessageOrientation_h__
#define StateMessageOrientation_h__

#include <FBCore/State/Messages/StateMessage.h>
#include "FBCore/Math/Quaternion.h"

namespace fb
{
    //---------------------------------------------
    class StateMessageOrientation : public StateMessage
    {
    public:
        StateMessageOrientation() = default;
        ~StateMessageOrientation() override = default;

        Quaternion<real_Num> getOrientation() const;
        void setOrientation( const Quaternion<real_Num> &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Quaternion<real_Num> m_orientation;
    };
}  // end namespace fb

#endif  // StateMessageOrientation_h__
