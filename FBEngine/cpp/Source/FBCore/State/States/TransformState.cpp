#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/TransformState.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Math/MathUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, TransformState, BaseState );

    auto TransformState::getPosition() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getPosition();
    }

    void TransformState::setPosition( const Vector3<real_Num> &position )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        if( !MathUtil<real_Num>::equals( m_actualTransform.getPosition(), position ) )
        {
            m_localTransform.setPosition( position );
            setDirty( true );
        }
    }

    auto TransformState::getScale() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getScale();
    }

    void TransformState::setScale( const Vector3<real_Num> &scale )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );

        if( !MathUtil<real_Num>::equals( m_actualTransform.getScale(), scale ) )
        {
            m_localTransform.setScale( scale );
            setDirty( true );
        }
    }

    auto TransformState::getOrientation() const -> Quaternion<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getOrientation();
    }

    void TransformState::setOrientation( const Quaternion<real_Num> &orientation )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        if( !MathUtil<real_Num>::equals( m_actualTransform.getOrientation(), orientation ) )
        {
            m_localTransform.setOrientation( orientation );
            setDirty( true );
        }
    }

    auto TransformState::getActualTransform() const -> Transform3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_actualTransform;
    }

    void TransformState::setActualTransform( const Transform3<real_Num> &actualTransform )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_actualTransform = actualTransform;
    }

    auto TransformState::getLocalTransform() const -> Transform3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform;
    }

    void TransformState::setLocalTransform( const Transform3<real_Num> &localTransform )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_localTransform = localTransform;
    }

    auto TransformState::clone() -> SmartPtr<IState>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        auto other = fb::make_ptr<TransformState>();
        makeClone( other );

        other->m_localTransform = m_localTransform;
        other->m_actualTransform = m_actualTransform;
        return other;
    }

}  // namespace fb
