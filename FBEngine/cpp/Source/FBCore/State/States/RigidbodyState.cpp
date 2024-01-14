#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/RigidbodyState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, RigidbodyState, BaseState );

    RigidbodyState::RigidbodyState() = default;

    RigidbodyState::~RigidbodyState() = default;

    auto RigidbodyState::getTransform() const -> Transform3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_transform;
    }

    void RigidbodyState::setTransform( const Transform3<real_Num> &transform )
    {
        if( getTransform() != transform )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_transform = transform;
            setDirty( true );
        }
    }

    void RigidbodyState::updateTransform( const Transform3<real_Num> &transform )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_transform = transform;
    }

}  // end namespace fb
