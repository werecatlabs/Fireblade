#ifndef StateMessageUIntValue_h__
#define StateMessageUIntValue_h__

#include <FBCore/State/Messages/StateMessage.h>

namespace fb
{
    //--------------------------------------------
    class StateMessageUIntValue : public StateMessage
    {
    public:
        StateMessageUIntValue() = default;
        ~StateMessageUIntValue() override = default;

        u32 getValue() const;
        void setValue( u32 val );

        FB_CLASS_REGISTER_DECL;

    protected:
        u32 m_value = 0;
    };
}  // end namespace fb

#endif  // StateMessageUIntValue_h__
