#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/BoxShapeState.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Math/MathUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, BoxShapeState, ShapeState );

    BoxShapeState::BoxShapeState() = default;

    BoxShapeState::~BoxShapeState() = default;

    auto BoxShapeState::getExtents() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_extents;
    }

    void BoxShapeState::setExtents( const Vector3<real_Num> &extents )
    {
        if( !MathUtil<real_Num>::equals( m_extents, extents ) )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_extents = extents;
            setDirty( true );
        }
    }
}  // end namespace fb
