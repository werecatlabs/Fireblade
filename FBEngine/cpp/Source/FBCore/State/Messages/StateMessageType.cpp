#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageType.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageType, StateMessage );

    //--------------------------------------------
    StateMessageType::StateMessageType()
    {
    }

    //--------------------------------------------
    StateMessageType::~StateMessageType() = default;

    //--------------------------------------------
    auto StateMessageType::getTypeValue() const -> u32
    {
        return m_typeValue;
    }

    //--------------------------------------------
    void StateMessageType::setTypeValue( u32 val )
    {
        m_typeValue = val;
    }
}  // end namespace fb
