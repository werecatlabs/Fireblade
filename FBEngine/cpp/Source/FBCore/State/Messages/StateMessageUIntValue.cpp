#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageUIntValue.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageUIntValue, StateMessage );

    //--------------------------------------------
    auto StateMessageUIntValue::getValue() const -> u32
    {
        return m_value;
    }

    //--------------------------------------------
    void StateMessageUIntValue::setValue( u32 val )
    {
        m_value = val;
    }
}  // end namespace fb
