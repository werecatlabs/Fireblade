#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/BoxShapeState.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Math/Core/MathUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, BoxShapeState, BaseState );

    BoxShapeState::BoxShapeState()
    {
    }

    BoxShapeState::~BoxShapeState()
    {
    }

    Vector3<real_Num> BoxShapeState::getExtents() const
    {
        FB_SPIN_LOCK_READ( m_mutex );
        return m_extents;
    }

    void BoxShapeState::setExtents( const Vector3<real_Num> &extents )
    {
        if( !MathUtil<real_Num>::equals( m_extents, extents ) )
        {
            FB_SPIN_LOCK_WRITE( m_mutex );
            m_extents = extents;
            setDirty( true );
        }
    }
}  // end namespace fb
