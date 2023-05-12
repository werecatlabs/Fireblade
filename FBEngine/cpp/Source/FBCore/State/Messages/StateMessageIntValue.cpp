#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageIntValue.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageIntValue, StateMessage );

    s32 StateMessageIntValue::getValue() const
    {
        return m_value;
    }

    void StateMessageIntValue::setValue( s32 value )
    {
        m_value = value;
    }
}  // end namespace fb
