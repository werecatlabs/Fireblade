#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StatePhysicsDynamicState2.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StatePhysicsDynamicState2, StateMessage );

    Vector2<real_Num> StatePhysicsDynamicState2::getPosition() const
    {
        return m_position;
    }

    void StatePhysicsDynamicState2::setPosition( const Vector2<real_Num> &val )
    {
        m_position = val;
    }

    Vector2<real_Num> StatePhysicsDynamicState2::getVelocity() const
    {
        return m_velocity;
    }

    void StatePhysicsDynamicState2::setVelocity( const Vector2<real_Num> &val )
    {
        m_velocity = val;
    }

    Vector2<real_Num> StatePhysicsDynamicState2::getForce() const
    {
        return m_force;
    }

    void StatePhysicsDynamicState2::setForce( const Vector2<real_Num> &val )
    {
        m_force = val;
    }

    real_Num StatePhysicsDynamicState2::getAngularVelocity() const
    {
        return m_angularVelocity;
    }

    void StatePhysicsDynamicState2::setAngularVelocity( real_Num val )
    {
        m_angularVelocity = val;
    }

    real_Num StatePhysicsDynamicState2::getOrientation() const
    {
        return m_orientation;
    }

    void StatePhysicsDynamicState2::setOrientation( real_Num val )
    {
        m_orientation = val;
    }
}  // end namespace fb
