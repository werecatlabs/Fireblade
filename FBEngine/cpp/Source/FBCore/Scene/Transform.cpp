#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Transform.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/Math/Euler.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <rttr/registration>

namespace fb::scene
{

    const hash_type Transform::TRANSFORMATION_POSITION_HASH =
        StringUtil::getHash( "TransformationPosition" );
    FB_CLASS_REGISTER_DERIVED( fb::scene, Transform, ITransform );

    Transform::Transform() = default;

    Transform::~Transform()
    {
        unload( nullptr );
    }

    void Transform::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            auto factoryManager = applicationManager->getFactoryManager();

            FB_ASSERT( stateManager );
            FB_ASSERT( factoryManager );

            m_stateContext = stateManager->addStateObject();
            FB_ASSERT( m_stateContext );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Transform::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto &applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            setActor( nullptr );

            if( auto &stateManager = applicationManager->getStateManager() )
            {
                if( m_stateContext )
                {
                    stateManager->removeStateObject( m_stateContext );
                    m_stateContext->unload( nullptr );
                    m_stateContext = nullptr;
                }
            }
            else
            {
                m_stateContext = nullptr;
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Transform::updateTransform()
    {
        if( isLocalDirty() )
        {
            updateLocalFromWorld();
        }

        if( isDirty() )
        {
            updateWorldFromLocal();
        }
    }

    void Transform::updateLocalFromWorld()
    {
        // FB_ASSERT(m_state);

        auto actor = getActor();
        if( actor )
        {
            auto pParent = actor->getParent();
            if( pParent )
            {
                auto parentTransform = pParent->getTransform();
                if( parentTransform )
                {
                    auto parentWorldTransform = parentTransform->getWorldTransform();

                    auto worldTransform = getWorldTransform();
                    auto worldPosition = worldTransform.getPosition();
                    auto worldOrientation = worldTransform.getOrientation();

                    auto localPosition =
                        parentWorldTransform.convertWorldToLocalPosition( worldPosition );
                    auto localOrientation =
                        parentWorldTransform.convertWorldToLocalOrientation( worldOrientation );

                    auto localTransform = getLocalTransform();
                    localTransform.setPosition( localPosition );
                    localTransform.setOrientation( localOrientation );
                    setLocalTransform( localTransform );
                }
            }
            else
            {
                auto localTransform = getWorldTransform();
                setLocalTransform( localTransform );
            }
        }
        else
        {
            auto localTransform = getWorldTransform();
            setLocalTransform( localTransform );
        }

        m_isLocalDirty = false;
    }

    void Transform::parentChanged( SmartPtr<scene::IActor> newParent, SmartPtr<scene::IActor> oldParent )
    {
        // m_state->parent = newParent;

        // if (newParent)
        //{
        //	auto pTransform = newParent->getTransform();
        //	if (pTransform)
        //	{
        //		auto transform = fb::static_pointer_cast<TransformComponent>(pTransform);
        //		m_state->parentTransformState = transform->m_state;
        //	}
        // }
    }

    auto Transform::getActor() const -> SmartPtr<scene::IActor>
    {
        return m_actor;
    }

    void Transform::setActor( SmartPtr<scene::IActor> actor )
    {
        m_actor = actor;
    }

    auto Transform::isLocalDirty() const -> bool
    {
        // FB_ASSERT(m_state);
        return m_isLocalDirty;
    }

    void Transform::setLocalDirty( bool localDirty, bool cascade )
    {
        if( m_isLocalDirty != localDirty )
        {
            m_isLocalDirty = localDirty;

            if( m_isLocalDirty )
            {
                updateFrameTime();

                if( auto actor = getActor() )
                {
                    auto applicationManager = core::ApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto sceneManager =
                        fb::static_pointer_cast<SceneManager>( applicationManager->getSceneManager() );

                    auto transform = getSharedFromThis<ITransform>();
                    sceneManager->addDirtyTransform( transform );

                    auto components = actor->getComponents();
                    for( auto &component : components )
                    {
                        sceneManager->addDirtyComponentTransform( component );
                    }

                    if( cascade )
                    {
                        if( auto p = actor->getChildrenPtr() )
                        {
                            auto &children = *p;
                            for( auto &child : children )
                            {
                                FB_ASSERT( child );

                                auto childTransform = child->getTransform();
                                FB_ASSERT( childTransform );

                                childTransform->setLocalDirty( true );
                            }
                        }
                    }
                }
            }
        }
    }

    auto Transform::isDirty() const -> bool
    {
        return m_isDirty;
    }

    void Transform::setDirty( bool dirty, bool cascade )
    {
        if( m_isDirty != dirty )
        {
            m_isDirty = dirty;

            if( m_isDirty )
            {
                updateFrameTime();

                if( auto actor = getActor() )
                {
                    auto applicationManager = core::ApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto sceneManager =
                        fb::static_pointer_cast<SceneManager>( applicationManager->getSceneManager() );

                    auto transform = getSharedFromThis<ITransform>();
                    sceneManager->addDirtyTransform( transform );

                    auto components = actor->getComponents();
                    for( auto &component : components )
                    {
                        sceneManager->addDirtyComponentTransform( component );
                    }

                    if( cascade )
                    {
                        if( auto p = actor->getChildrenPtr() )
                        {
                            auto &children = *p;
                            for( auto &child : children )
                            {
                                FB_ASSERT( child );

                                auto childTransform = child->getTransform();
                                FB_ASSERT( childTransform );

                                childTransform->setDirty( true );
                            }
                        }
                    }
                }
            }
        }
    }

    void Transform::setEnabled( bool enabled )
    {
        m_enabled = enabled;
    }

    auto Transform::isEnabled() const -> bool
    {
        return m_enabled;
    }

    auto Transform::getStateContext() const -> SmartPtr<IStateContext>
    {
        return m_stateContext;
    }

    void Transform::setStateContext( SmartPtr<IStateContext> stateContext )
    {
        m_stateContext = stateContext;
    }

    auto Transform::getLocalTransform() const -> Transform3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform;
    }

    void Transform::setLocalTransform( Transform3<real_Num> transform )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_localTransform = transform;
    }

    auto Transform::getWorldTransform() const -> Transform3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_worldTransform;
    }

    void Transform::setWorldTransform( Transform3<real_Num> transform )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_worldTransform = transform;
    }

    auto Transform::getLocalPosition() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getPosition();
    }

    void Transform::setLocalPosition( const Vector3<real_Num> &localPosition )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_localTransform.setPosition( localPosition );
    }

    auto Transform::getLocalScale() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getScale();
    }

    void Transform::setLocalScale( const Vector3<real_Num> &localScale )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_localTransform.setScale( localScale );
    }

    auto Transform::getLocalOrientation() const -> Quaternion<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_localTransform.getOrientation();
    }

    void Transform::setLocalOrientation( const Quaternion<real_Num> &localOrientation )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_localTransform.setOrientation( localOrientation );
    }

    auto Transform::getLocalRotation() const -> Vector3<real_Num>
    {
        Euler<real_Num> euler( getLocalOrientation() );
        return { euler.pitch() * Math<real_Num>::rad_to_deg(),
                 euler.yaw() * Math<real_Num>::rad_to_deg(),
                 euler.roll() * Math<real_Num>::rad_to_deg() };
    }

    void Transform::setLocalRotation( const Vector3<real_Num> &localRotation )
    {
        Euler<real_Num> euler( localRotation.Y() * Math<real_Num>::deg_to_rad(),
                               localRotation.X() * Math<real_Num>::deg_to_rad(),
                               localRotation.Z() * Math<real_Num>::deg_to_rad() );

        auto orientation = euler.toQuaternion();
        setLocalOrientation( orientation );
    }

    auto Transform::getPosition() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_worldTransform.getPosition();
    }

    void Transform::setPosition( const Vector3<real_Num> &position )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_worldTransform.setPosition( position );
    }

    auto Transform::getScale() const -> Vector3<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_worldTransform.getScale();
    }

    void Transform::setScale( const Vector3<real_Num> &scale )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_worldTransform.setScale( scale );
    }

    auto Transform::getOrientation() const -> Quaternion<real_Num>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_worldTransform.getOrientation();
    }

    void Transform::setOrientation( const Quaternion<real_Num> &orientation )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_worldTransform.setOrientation( orientation );
    }

    auto Transform::getRotation() const -> Vector3<real_Num>
    {
        Euler<real_Num> euler( getOrientation() );
        return { euler.pitch() * Math<real_Num>::rad_to_deg(),
                 euler.yaw() * Math<real_Num>::rad_to_deg(),
                 euler.roll() * Math<real_Num>::rad_to_deg() };
    }

    void Transform::setRotation( const Vector3<real_Num> &rotation )
    {
        Euler<real_Num> euler( rotation.Y() * Math<real_Num>::deg_to_rad(),
                               rotation.X() * Math<real_Num>::deg_to_rad(),
                               rotation.Z() * Math<real_Num>::deg_to_rad() );

        auto orientation = euler.toQuaternion();
        setOrientation( orientation );
    }

    void Transform::updateWorldFromLocal()
    {
        if( isLoaded() )
        {
            if( auto actor = getActor() )
            {
                auto pParent = actor->getParent();
                if( pParent )
                {
                    auto parentTransform = pParent->getTransform();
                    if( parentTransform )
                    {
                        auto parentWorldTransform = parentTransform->getWorldTransform();
                        auto localTransform = getLocalTransform();

                        auto worldTransform = getWorldTransform();
                        worldTransform.transformFromParent( parentWorldTransform, localTransform );
                        setWorldTransform( worldTransform );
                    }
                }
                else
                {
                    auto localTransform = getLocalTransform();

                    auto worldTransform = getWorldTransform();
                    worldTransform.setTransform( localTransform );
                    setWorldTransform( worldTransform );
                }
            }
            else
            {
                auto localTransform = getLocalTransform();

                auto worldTransform = getWorldTransform();
                worldTransform.setTransform( localTransform );
                setWorldTransform( worldTransform );
            }

            m_isDirty = false;
        }
    }

    auto Transform::getProperties() const -> SmartPtr<Properties>
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );

        auto properties = fb::make_ptr<Properties>();

        auto localTransform = getLocalTransform();
        auto localPosition = localTransform.getPosition();
        auto localOrientation = localTransform.getOrientation();
        auto localScale = localTransform.getScale();

        auto worldTransform = getWorldTransform();
        auto position = worldTransform.getPosition();
        auto orientation = worldTransform.getOrientation();
        auto scale = worldTransform.getScale();

        Euler<real_Num> eular( localOrientation );
        Vector3<real_Num> localRotation = eular.toDegrees();

        Euler<real_Num> worldEular( orientation );
        Vector3<real_Num> rotation = worldEular.toDegrees();

        properties->setProperty( "Local Position", localPosition );
        properties->setProperty( "Local Rotation", localRotation );
        properties->setProperty( "Local Scale", localScale );

        properties->setProperty( "Position", position );
        properties->setProperty( "Rotation", rotation );
        properties->setProperty( "Scale", scale );

        return properties;
    }

    void Transform::setProperties( SmartPtr<Properties> properties )
    {
        try
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );

            auto localTransform = getLocalTransform();
            auto currentLocalPosition = localTransform.getPosition();
            auto currentLocalOrientation = localTransform.getOrientation();
            auto currentLocalScale = localTransform.getScale();

            auto worldTransform = getWorldTransform();
            auto currentPosition = worldTransform.getPosition();
            auto currentOrientation = worldTransform.getOrientation();
            auto currentScale = worldTransform.getScale();

            auto currentLocalRotation = Vector3<real_Num>::zero();
            auto currentRotation = Vector3<real_Num>::zero();

            currentLocalOrientation.fromDegrees( currentLocalRotation );
            currentOrientation.fromDegrees( currentRotation );

            currentLocalRotation = MathUtil<real_Num>::round( currentLocalRotation, 3 );
            currentRotation = MathUtil<real_Num>::round( currentRotation, 3 );

            auto localPosition = Vector3<real_Num>::zero();
            auto localOrientation = Quaternion<real_Num>::identity();
            auto localScale = Vector3<real_Num>::unit();

            auto position = Vector3<real_Num>::zero();
            auto orientation = Quaternion<real_Num>::identity();
            auto scale = Vector3<real_Num>::unit();

            auto localRotation = Vector3<real_Num>::zero();
            auto rotation = Vector3<real_Num>::zero();

            properties->getPropertyValue( "Local Position", localPosition );
            properties->getPropertyValue( "Local Rotation", localRotation );
            properties->getPropertyValue( "Local Scale", localScale );

            properties->getPropertyValue( "Position", position );
            properties->getPropertyValue( "Rotation", rotation );
            properties->getPropertyValue( "Scale", scale );

            localRotation = MathUtil<real_Num>::round( localRotation, 3 );
            rotation = MathUtil<real_Num>::round( rotation, 3 );

            localOrientation.fromDegrees( localRotation );
            orientation.fromDegrees( rotation );

            auto propertiesArray = properties->getPropertiesAsArray();
            for( auto &p : propertiesArray )
            {
                auto attibute = p.getAttribute( "changed" );
                if( attibute == "true" )
                {
                    auto propertyName = p.getName();
                    if( propertyName == "Local Position" || propertyName == "Local Rotation" ||
                        propertyName == "Local Scale" )
                    {
                        setDirty( true );
                    }

                    if( propertyName == "Position" || propertyName == "Rotation" ||
                        propertyName == "Scale" )
                    {
                        setLocalDirty( true );
                    }
                }
            }

            if( currentLocalPosition != localPosition )
            {
                localTransform.setPosition( localPosition );
            }

            if( currentLocalOrientation != localOrientation )
            {
                localTransform.setOrientation( localOrientation );
            }

            if( currentLocalScale != localScale )
            {
                localTransform.setScale( localScale );
            }

            if( currentPosition != position )
            {
                worldTransform.setPosition( position );
            }

            worldTransform.setOrientation( orientation );

            if( currentScale != scale )
            {
                worldTransform.setScale( scale );
            }

            auto updateActorTransform = isLocalDirty() || isDirty();
            if( updateActorTransform )
            {
                auto actor = getActor();
                if( actor )
                {
                    actor->updateTransform();
                }
            }

            if( isLocalDirty() )
            {
                updateLocalFromWorld();
            }

            if( isDirty() )
            {
                updateWorldFromLocal();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Transform::registerClass()
    {
        using namespace fb;
        using namespace rttr;

        registration::class_<Transform>( "Transform" )
            .property( "worldTransform", &Transform::m_worldTransform )
            .property( "localTransform", &Transform::m_localTransform );
    }

    auto Transform::getFrameTime() const -> time_interval
    {
        return m_frameTime;
    }

    void Transform::setFrameTime( time_interval frameTime )
    {
        m_frameTime = frameTime;
    }

    auto Transform::getFrameDeltaTime() const -> time_interval
    {
        return m_frameDeltaTime;
    }

    void Transform::setFrameDeltaTime( time_interval frameDeltaTime )
    {
        m_frameDeltaTime = frameDeltaTime;
    }

    void Transform::updateFrameTime()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto timer = applicationManager->getTimer();
        m_frameTime = timer->getTime();
        m_frameDeltaTime = timer->getDeltaTime();
    }

}  // namespace fb::scene
