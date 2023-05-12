#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/SceneNodeState.h>
#include <FBCore/Math/Core/MathUtil.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, SceneNodeState, BaseState );

    SceneNodeState::SceneNodeState() :
        m_isVisible( true ),
        m_isAttached( false ),
        m_id( 0 ),

        m_flags( 0 ),
        m_queueCount( 0 ),
        m_isRegistered( false )
    {
        m_localTransform.setScale( Vector3<real_Num>::unit() );
    }

    SceneNodeState::SceneNodeState( const SceneNodeState &state )
    {
        // FB_SPIN_LOCK_READ(state.Mutex);
        //*this = state;
    }

    Vector3<real_Num> SceneNodeState::getPosition() const
    {
        return m_localTransform.getPosition();
    }

    bool SceneNodeState::operator!=( SceneNodeState *other ) const
    {
        return Memory::Memcmp( this, other, sizeof( SceneNodeState ) ) != 0;
    }

    bool SceneNodeState::operator==( SceneNodeState *other ) const
    {
        return Memory::Memcmp( this, other, sizeof( SceneNodeState ) ) == 0;
    }

    void SceneNodeState::setLookAt( const Vector3<real_Num> &lookAt )
    {
        m_lookAt = lookAt;
    }

    Vector3<real_Num> SceneNodeState::getAbsolutePosition() const
    {
        return m_absolutePosition;
    }

    void SceneNodeState::setAbsolutePosition( const Vector3<real_Num> &val )
    {
        m_absolutePosition = val;
    }

    Vector3<real_Num> SceneNodeState::getAbsoluteScale() const
    {
        return m_absoluteScale;
    }

    void SceneNodeState::setAbsoluteScale( const Vector3<real_Num> &scale )
    {
        m_absoluteScale = scale;
    }

    Quaternion<real_Num> SceneNodeState::getAbsoluteOrientation() const
    {
        return m_absoluteOrientation;
    }

    void SceneNodeState::setAbsoluteOrientation( const Quaternion<real_Num> &orientation )
    {
        m_absoluteOrientation = orientation;
    }

    AABB3<real_Num> SceneNodeState::getLocalAABB() const
    {
        return m_localAABB;
    }

    void SceneNodeState::setLocalAABB( const AABB3<real_Num> &localAABB )
    {
        m_localAABB = localAABB;
    }

    AABB3<real_Num> SceneNodeState::getWorldAABB() const
    {
        return m_worldAABB;
    }

    void SceneNodeState::setWorldAABB( const AABB3<real_Num> &worldAABB )
    {
        m_worldAABB = worldAABB;
    }

    void SceneNodeState::setPosition( const Vector3<real_Num> &position )
    {
        if( !MathUtil<real_Num>::equals( m_localTransform.getPosition(), position ) )
        {
            m_localTransform.setPosition( position );
            setDirty( true );
        }
    }

    Vector3<real_Num> SceneNodeState::getScale() const
    {
        return m_localTransform.getScale();
    }

    void SceneNodeState::setScale( const Vector3<real_Num> &scale )
    {
        if( !MathUtil<real_Num>::equals( m_localTransform.getScale(), scale ) )
        {
            m_localTransform.setScale( scale );
            setDirty( true );
        }
    }

    Quaternion<real_Num> SceneNodeState::getOrientation() const
    {
        return m_localTransform.getOrientation();
    }

    void SceneNodeState::setOrientation( const Quaternion<real_Num> &orientation )
    {
        if( !MathUtil<real_Num>::equals( m_localTransform.getOrientation(), orientation ) )
        {
            m_localTransform.setOrientation( orientation );
            setDirty( true );
        }
    }

    Vector3<real_Num> SceneNodeState::getLookAt() const
    {
        return m_lookAt;
    }

    bool SceneNodeState::isRegistered() const
    {
        return m_isRegistered;
    }

    void SceneNodeState::setRegistered( bool val )
    {
        m_isRegistered = val;
    }

    SmartPtr<Properties> SceneNodeState::getProperties() const
    {
        auto properties = BaseState::getProperties();

        auto sceneNodePosition = getPosition();
        auto sceneNodeScale = getScale();
        auto sceneNodeOrientation = getOrientation();

        Vector3<real_Num> localRotation;
        sceneNodeOrientation.toDegrees( localRotation );

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

    SmartPtr<IState> SceneNodeState::clone() const
    {
        auto state = fb::make_ptr<SceneNodeState>();

        SpinRWMutex::ScopedLock lock( m_mutex );
        state->m_localTransform = m_localTransform;

        return state;
    }

    bool SceneNodeState::isVisible() const
    {
        return m_isVisible;
    }

    void SceneNodeState::setVisible( bool visible )
    {
        if( m_isVisible != visible )
        {
            m_isVisible = visible;
            setDirty( true );
        }
    }

}  // end namespace fb
