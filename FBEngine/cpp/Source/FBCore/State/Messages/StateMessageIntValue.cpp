#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageIntValue.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageIntValue, StateMessage );

    auto StateMessageIntValue::getValue() const -> s32
    {
        return m_value;
    }

    void StateMessageIntValue::setValue( s32 value )
    {
        m_value = value;
    }
}  // end namespace fb
