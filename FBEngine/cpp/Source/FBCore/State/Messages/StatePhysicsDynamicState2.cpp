#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StatePhysicsDynamicState2.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StatePhysicsDynamicState2, StateMessage );

    auto StatePhysicsDynamicState2::getPosition() const -> Vector2<real_Num>
    {
        return m_position;
    }

    void StatePhysicsDynamicState2::setPosition( const Vector2<real_Num> &val )
    {
        m_position = val;
    }

    auto StatePhysicsDynamicState2::getVelocity() const -> Vector2<real_Num>
    {
        return m_velocity;
    }

    void StatePhysicsDynamicState2::setVelocity( const Vector2<real_Num> &val )
    {
        m_velocity = val;
    }

    auto StatePhysicsDynamicState2::getForce() const -> Vector2<real_Num>
    {
        return m_force;
    }

    void StatePhysicsDynamicState2::setForce( const Vector2<real_Num> &val )
    {
        m_force = val;
    }

    auto StatePhysicsDynamicState2::getAngularVelocity() const -> real_Num
    {
        return m_angularVelocity;
    }

    void StatePhysicsDynamicState2::setAngularVelocity( real_Num val )
    {
        m_angularVelocity = val;
    }

    auto StatePhysicsDynamicState2::getOrientation() const -> real_Num
    {
        return m_orientation;
    }

    void StatePhysicsDynamicState2::setOrientation( real_Num val )
    {
        m_orientation = val;
    }
}  // end namespace fb
