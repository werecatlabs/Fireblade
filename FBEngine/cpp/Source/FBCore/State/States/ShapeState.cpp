#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/ShapeState.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ShapeState, BaseState );

    ShapeState::ShapeState() = default;

    ShapeState::~ShapeState() = default;

    void ShapeState::setLocalPose( const Transform3<real_Num> &pose )
    {
        if( getLocalPose() != pose )
        {
            SpinRWMutex::ScopedLock lock( m_mutex );
            m_localPose = pose;
            setDirty( true );
        }
    }

    auto ShapeState::getLocalPose() const -> Transform3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_localPose;
    }

}  // end namespace fb
