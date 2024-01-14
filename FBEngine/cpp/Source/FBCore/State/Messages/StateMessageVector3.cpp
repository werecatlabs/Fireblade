#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageVector3.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageVector3, StateMessage );

    StateMessageVector3::StateMessageVector3() = default;

    StateMessageVector3::StateMessageVector3( const Vector3F &position ) : m_position( position )
    {
    }

    StateMessageVector3::~StateMessageVector3() = default;

    auto StateMessageVector3::getValue() const -> Vector3F
    {
        return m_position;
    }

    void StateMessageVector3::setValue( const Vector3F &val )
    {
        m_position = val;
    }
}  // end namespace fb
