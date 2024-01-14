#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageParamVector4.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageParamVector4, StateMessage );

    StateMessageParamVector4::StateMessageParamVector4() = default;

    StateMessageParamVector4::~StateMessageParamVector4() = default;

    auto StateMessageParamVector4::getId() const -> hash32
    {
        return m_id;
    }

    void StateMessageParamVector4::setId( hash32 val )
    {
        m_id = val;
    }

    auto StateMessageParamVector4::getValue() const -> Vector4F
    {
        return m_value;
    }

    void StateMessageParamVector4::setValue( const Vector4F &val )
    {
        m_value = val;
    }
}  // namespace fb
