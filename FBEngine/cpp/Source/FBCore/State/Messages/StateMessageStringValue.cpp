#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageStringValue.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageStringValue, StateMessage );

    StateMessageStringValue::StateMessageStringValue() = default;

    StateMessageStringValue::~StateMessageStringValue() = default;

    void StateMessageStringValue::setValue( const String &val )
    {
        m_value = val;
    }

    auto StateMessageStringValue::getValue() const -> String
    {
        return m_value;
    }
}  // end namespace fb
