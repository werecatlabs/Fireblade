#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/ShapeState.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ShapeState, BaseState );

    ShapeState::ShapeState()
    {
    }

    ShapeState::~ShapeState()
    {
    }

    void ShapeState::setLocalPose( const Transform3<real_Num> &pose )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_localPose = pose;
        setDirty( true );
    }

    Transform3<real_Num> ShapeState::getLocalPose() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_localPose;
    }

}  // end namespace fb
