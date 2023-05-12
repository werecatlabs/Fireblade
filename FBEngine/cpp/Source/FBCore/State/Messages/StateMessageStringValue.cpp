#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageStringValue.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageStringValue, StateMessage );

    StateMessageStringValue::StateMessageStringValue()
    {
    }

    StateMessageStringValue::~StateMessageStringValue()
    {
    }

    void StateMessageStringValue::setValue( const String &val )
    {
        m_value = val;
    }

    String StateMessageStringValue::getValue() const
    {
        return m_value;
    }
}  // end namespace fb
