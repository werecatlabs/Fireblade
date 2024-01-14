#ifndef StateMessageIntValue_h__
#define StateMessageIntValue_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{

    class StateMessageIntValue : public StateMessage
    {
    public:
        StateMessageIntValue() = default;
        ~StateMessageIntValue() override = default;

        s32 getValue() const;
        void setValue( s32 value );

        FB_CLASS_REGISTER_DECL;

    protected:
        s32 m_value = 0;
    };
}  // end namespace fb

#endif  // StateMessageIntValue_h__
