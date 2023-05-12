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

        QuaternionF getOrientation() const;
        void setOrientation( const QuaternionF &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        QuaternionF m_orientation;
    };
}  // end namespace fb

#endif  // StateMessageOrientation_h__
