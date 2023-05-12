#ifndef StateMessagePlay_h__
#define StateMessagePlay_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //---------------------------------------------
    class StateMessagePlay : public StateMessage
    {
    public:
        StateMessagePlay();
        ~StateMessagePlay() override;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // StateMessagePlay_h__
