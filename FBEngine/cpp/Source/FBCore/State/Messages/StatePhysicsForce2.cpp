#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StatePhysicsForce2.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StatePhysicsForce2, StateMessage );

    const hash_type StatePhysicsForce2::ADD_FORCE_HASH = StringUtil::getHash( "addForce" );
    const hash_type StatePhysicsForce2::SET_FORCE_HASH = StringUtil::getHash( "setForce" );

    StatePhysicsForce2::StatePhysicsForce2() = default;

    auto StatePhysicsForce2::getForce() const -> Vector2<real_Num>
    {
        return m_force;
    }

    void StatePhysicsForce2::setForce( const Vector2<real_Num> &val )
    {
        m_force = val;
    }

    auto StatePhysicsForce2::getRelativePosition() const -> Vector2<real_Num>
    {
        return m_relativePosition;
    }

    void StatePhysicsForce2::setRelativePosition( const Vector2<real_Num> &val )
    {
        m_relativePosition = val;
    }
}  // end namespace fb
