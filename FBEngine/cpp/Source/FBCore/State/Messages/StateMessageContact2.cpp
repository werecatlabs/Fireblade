#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StateMessageContact2.h"

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageContact2, StateMessage );

    StateMessageContact2::StateMessageContact2()
    {
    }

    StateMessageContact2::~StateMessageContact2()
    {
    }

    hash32 StateMessageContact2::getContactType() const
    {
        return m_contactType;
    }

    void StateMessageContact2::setContactType( hash32 val )
    {
        m_contactType = val;
    }

    Vector2<real_Num> StateMessageContact2::getPosition() const
    {
        return m_position;
    }

    void StateMessageContact2::setPosition( const Vector2<real_Num> &val )
    {
        m_position = val;
    }

    Vector2<real_Num> StateMessageContact2::getNormal() const
    {
        return m_normal;
    }

    void StateMessageContact2::setNormal( const Vector2<real_Num> &val )
    {
        m_normal = val;
    }
}  // end namespace fb
