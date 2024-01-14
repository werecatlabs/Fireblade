#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageContact2.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageContact2, StateMessage );

    StateMessageContact2::StateMessageContact2() = default;

    StateMessageContact2::~StateMessageContact2() = default;

    auto StateMessageContact2::getContactType() const -> hash32
    {
        return m_contactType;
    }

    void StateMessageContact2::setContactType( hash32 val )
    {
        m_contactType = val;
    }

    auto StateMessageContact2::getPosition() const -> Vector2<real_Num>
    {
        return m_position;
    }

    void StateMessageContact2::setPosition( const Vector2<real_Num> &val )
    {
        m_position = val;
    }

    auto StateMessageContact2::getNormal() const -> Vector2<real_Num>
    {
        return m_normal;
    }

    void StateMessageContact2::setNormal( const Vector2<real_Num> &val )
    {
        m_normal = val;
    }
}  // end namespace fb
