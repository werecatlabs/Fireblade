#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StatePhysicsForce2.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StatePhysicsForce2, StateMessage );

    const hash_type StatePhysicsForce2::ADD_FORCE_HASH = StringUtil::getHash( "addForce" );
    const hash_type StatePhysicsForce2::SET_FORCE_HASH = StringUtil::getHash( "setForce" );

    StatePhysicsForce2::StatePhysicsForce2()
    {
    }

    Vector2<real_Num> StatePhysicsForce2::getForce() const
    {
        return m_force;
    }

    void StatePhysicsForce2::setForce( const Vector2<real_Num> &val )
    {
        m_force = val;
    }

    Vector2<real_Num> StatePhysicsForce2::getRelativePosition() const
    {
        return m_relativePosition;
    }

    void StatePhysicsForce2::setRelativePosition( const Vector2<real_Num> &val )
    {
        m_relativePosition = val;
    }
}  // end namespace fb
