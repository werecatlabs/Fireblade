#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageParamVector4.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageParamVector4, StateMessage );

    StateMessageParamVector4::StateMessageParamVector4()
    {
    }

    StateMessageParamVector4::~StateMessageParamVector4()
    {
    }

    hash32 StateMessageParamVector4::getId() const
    {
        return m_id;
    }

    void StateMessageParamVector4::setId( hash32 val )
    {
        m_id = val;
    }

    Vector4F StateMessageParamVector4::getValue() const
    {
        return m_value;
    }

    void StateMessageParamVector4::setValue( const Vector4F &val )
    {
        m_value = val;
    }
}  // namespace fb
