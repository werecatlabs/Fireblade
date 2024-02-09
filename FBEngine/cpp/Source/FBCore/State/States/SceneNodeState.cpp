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
        constexpr auto size = sizeof( SceneNodeState );
    }

    SceneNodeState::SceneNodeState( const SceneNodeState &state )
    {
        // SpinRWMutex::ScopedLock lock(state.Mutex);
        //*this = state;
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

    auto SceneNodeState::getLookAt() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex );
        return m_lookAt;
    }

    auto SceneNodeState::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = BaseState::getProperties();


        properties->setProperty( "lookAt", m_lookAt );

        return properties;
    }

    void SceneNodeState::setProperties( SmartPtr<Properties> properties )
    {
        BaseState::setProperties( properties );

        properties->getPropertyValue( "lookAt", m_lookAt );
    }

    auto SceneNodeState::clone() const -> SmartPtr<IState>
    {
        auto state = fb::make_ptr<SceneNodeState>();

        SpinRWMutex::ScopedLock lock( m_mutex );
        state->m_lookAt = m_lookAt;

        return state;
    }

}  // end namespace fb
