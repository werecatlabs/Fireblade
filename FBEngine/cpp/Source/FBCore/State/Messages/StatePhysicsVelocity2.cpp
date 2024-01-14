#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StatePhysicsVelocity2.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StatePhysicsVelocity2, StateMessage );

    const hash_type StatePhysicsVelocity2::ADD_VELOCITY_HASH = StringUtil::getHash( "addVelocity" );
    const hash_type StatePhysicsVelocity2::SET_VELOCITY_HASH = StringUtil::getHash( "setVelocity" );

    StatePhysicsVelocity2::StatePhysicsVelocity2( const Vector2<real_Num> &velocity ) :
        m_velocity( velocity )
    {
    }

    StatePhysicsVelocity2::StatePhysicsVelocity2() = default;

    auto StatePhysicsVelocity2::getVelocity() const -> Vector2<real_Num>
    {
        return m_velocity;
    }

    void StatePhysicsVelocity2::setVelocity( const Vector2<real_Num> &val )
    {
        m_velocity = val;
    }

    auto StatePhysicsVelocity2::getRelativePosition() const -> Vector2<real_Num>
    {
        return m_relativePosition;
    }

    void StatePhysicsVelocity2::setRelativePosition( const Vector2<real_Num> &val )
    {
        m_relativePosition = val;
    }
}  // end namespace fb
