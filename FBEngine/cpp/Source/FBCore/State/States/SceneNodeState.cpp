#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/SceneNodeState.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, SceneNodeState, BaseState );

    SceneNodeState::SceneNodeState() :
        m_id( 0 ),

        m_flags( 0 ),
        m_queueCount( 0 )
    {
        m_localTransform.setScale( Vector3<real_Num>::unit() );

        constexpr auto size = sizeof( SceneNodeState );
    }

    SceneNodeState::SceneNodeState( const SceneNodeState &state )
    {
        // SpinRWMutex::ScopedLock lock(state.Mutex);
        //*this = state;
    }

    auto SceneNodeState::getPosition() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getPosition();
    }

    auto SceneNodeState::operator!=( SceneNodeState *other ) const -> bool
    {
        return Memory::Memcmp( this, other, sizeof( SceneNodeState ) ) != 0;
    }

    auto SceneNodeState::operator==( SceneNodeState *other ) const -> bool
    {
        return Memory::Memcmp( this, other, sizeof( SceneNodeState ) ) == 0;
    }

    void SceneNodeState::setLookAt( const Vector3<real_Num> &lookAt )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_lookAt = lookAt;
    }

    auto SceneNodeState::getAbsolutePosition() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_absolutePosition;
    }

    void SceneNodeState::setAbsolutePosition( const Vector3<real_Num> &val )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_absolutePosition = val;
    }

    auto SceneNodeState::getAbsoluteScale() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_absoluteScale;
    }

    void SceneNodeState::setAbsoluteScale( const Vector3<real_Num> &scale )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_absoluteScale = scale;
    }

    auto SceneNodeState::getAbsoluteOrientation() const -> Quaternion<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_absoluteOrientation;
    }

    void SceneNodeState::setAbsoluteOrientation( const Quaternion<real_Num> &orientation )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_absoluteOrientation = orientation;
    }

    auto SceneNodeState::getLocalAABB() const -> AABB3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_localAABB;
    }

    void SceneNodeState::setLocalAABB( const AABB3<real_Num> &localAABB )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_localAABB = localAABB;
    }

    auto SceneNodeState::getWorldAABB() const -> AABB3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_worldAABB;
    }

    void SceneNodeState::setWorldAABB( const AABB3<real_Num> &worldAABB )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        m_worldAABB = worldAABB;
    }

    void SceneNodeState::setPosition( const Vector3<real_Num> &position )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        if( !MathUtil<real_Num>::equals( m_localTransform.getPosition(), position ) )
        {
            m_localTransform.setPosition( position );
            setDirty( true );
        }
    }

    auto SceneNodeState::getScale() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getScale();
    }

    void SceneNodeState::setScale( const Vector3<real_Num> &scale )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );

        if( !MathUtil<real_Num>::equals( m_localTransform.getScale(), scale ) )
        {
            m_localTransform.setScale( scale );
            setDirty( true );
        }
    }

    auto SceneNodeState::getOrientation() const -> Quaternion<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getOrientation();
    }

    void SceneNodeState::setOrientation( const Quaternion<real_Num> &orientation )
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        if( !MathUtil<real_Num>::equals( m_localTransform.getOrientation(), orientation ) )
        {
            m_localTransform.setOrientation( orientation );
            setDirty( true );
        }
    }

    auto SceneNodeState::getLookAt() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_lookAt;
    }

    auto SceneNodeState::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = BaseState::getProperties();

        auto sceneNodePosition = getPosition();
        auto sceneNodeScale = getScale();
        auto sceneNodeOrientation = getOrientation();

        Euler<real_Num> euler( sceneNodeOrientation );
        Vector3<real_Num> localRotation = euler.toDegrees();

        properties->setProperty( "sceneNodePosition", sceneNodePosition );
        properties->setProperty( "sceneNodeScale", sceneNodeScale );
        properties->setProperty( "sceneNodeOrientation", localRotation );

        return properties;
    }

    void SceneNodeState::setProperties( SmartPtr<Properties> properties )
    {
        BaseState::setProperties( properties );

        auto sceneNodePosition = getPosition();
        auto sceneNodeScale = getScale();
        auto sceneNodeOrientation = getOrientation();

        properties->getPropertyValue( "sceneNodePosition", sceneNodePosition );
        properties->getPropertyValue( "sceneNodeScale", sceneNodeScale );
        // properties->getPropertyValue("sceneNodeOrientation", sceneNodeOrientation);
    }

    auto SceneNodeState::clone() const -> SmartPtr<IState>
    {
        auto state = fb::make_ptr<SceneNodeState>();

        SpinRWMutex::ScopedLock lock( m_mutex );
        state->m_localTransform = m_localTransform;

        return state;
    }

}  // end namespace fb
