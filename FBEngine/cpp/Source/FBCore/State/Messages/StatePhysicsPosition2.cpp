#include <FBCore/FBCorePCH.h>
#include "FBCore/State/Messages/StatePhysicsPosition2.h"
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StatePhysicsPosition2, StateMessage );
    const hash_type StatePhysicsPosition2::TYPE = StringUtil::getHash( "StatePhysicsPosition2" );

    StatePhysicsPosition2::StatePhysicsPosition2( const Vector2<real_Num> &position ) :
        m_position( position ),
        m_subjectId( 0 )
    {
    }

    StatePhysicsPosition2::StatePhysicsPosition2( u32 subjectId, const Vector2<real_Num> &position ) :
        m_position( position ),
        m_subjectId( subjectId )
    {
    }

    StatePhysicsPosition2::StatePhysicsPosition2( u32 subjectId ) : m_subjectId( subjectId )
    {
    }

    StatePhysicsPosition2::StatePhysicsPosition2() = default;

    auto StatePhysicsPosition2::getPosition() const -> Vector2<real_Num>
    {
        return m_position;
    }

    void StatePhysicsPosition2::setPosition( const Vector2<real_Num> &val )
    {
        m_position = val;
    }

    auto StatePhysicsPosition2::getSubjectId() const -> u32
    {
        return m_subjectId;
    }

    void StatePhysicsPosition2::setSubjectId( u32 val )
    {
        m_subjectId = val;
    }
}  // end namespace fb
