#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageUIntValue.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageUIntValue, StateMessage );

    //--------------------------------------------
    u32 StateMessageUIntValue::getValue() const
    {
        return m_value;
    }

    //--------------------------------------------
    void StateMessageUIntValue::setValue( u32 val )
    {
        m_value = val;
    }
}  // end namespace fb
