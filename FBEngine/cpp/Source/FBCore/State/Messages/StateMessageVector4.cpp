#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageVector4.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageVector4, StateMessage );

    StateMessageVector4::StateMessageVector4()
    {
    }

    StateMessageVector4::StateMessageVector4( const Vector4F &position ) : m_position( position )
    {
    }

    StateMessageVector4::~StateMessageVector4()
    {
    }

    Vector4F StateMessageVector4::getValue() const
    {
        return m_position;
    }

    void StateMessageVector4::setValue( const Vector4F &val )
    {
        m_position = val;
    }
}  // end namespace fb
