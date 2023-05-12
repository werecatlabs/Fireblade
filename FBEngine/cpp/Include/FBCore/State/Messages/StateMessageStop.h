#ifndef StateMessageStop_h__
#define StateMessageStop_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageStop : public StateMessage
    {
    public:
        StateMessageStop() = default;
        ~StateMessageStop() override = default;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // StateMessageStop_h__
