#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Transform.h>
#include <FBCore/Scene/SceneManager.h>
#include <FBCore/FBCore.h>
#include <rttr/registration>

namespace fb
{
    namespace scene
    {

        const hash32 Transform::TRANSFORMATION_POSITION_HASH =
            StringUtil::getHash( "TransformationPosition" );
        FB_CLASS_REGISTER_DERIVED( fb, Transform, ITransform );

        Transform::Transform()
        {
        }

        Transform::~Transform()
        {
            unload( nullptr );
        }

        void Transform::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                auto factoryManager = applicationManager->getFactoryManager();

                FB_ASSERT( stateManager );
                FB_ASSERT( factoryManager );

                m_stateObject = stateManager->addStateObject();
                FB_ASSERT( m_stateObject );

                // m_state = factoryManager->make_ptr<TransformState>();
                // FB_ASSERT(m_state);

                // m_stateObject->setState(m_state);

                // auto data = stateManager->getStateDataByType<TransformStateData>();
                // if (data)
                //{
                //	data->addState(m_state);
                // }

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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                // if (m_state)
                //{
                //	if (m_stateObject)
                //	{
                //		m_stateObject->setState(nullptr);
                //	}

                //	m_state = nullptr;
                //}

                setActor( nullptr );

                if( auto stateManager = applicationManager->getStateManager() )
                {
                    if( m_stateObject )
                    {
                        stateManager->removeStateObject( m_stateObject );
                        m_stateObject->unload( nullptr );
                        m_stateObject = nullptr;
                    }
                }
                else
                {
                    m_stateObject = nullptr;
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
                    SpinRWMutex::ScopedLock lock( m_mutex, true);

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

        void Transform::parentChanged( SmartPtr<scene::IActor> newParent,
                                       SmartPtr<scene::IActor> oldParent )
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

        SmartPtr<scene::IActor> Transform::getActor() const
        {
            return m_actor;
        }

        void Transform::setActor( SmartPtr<scene::IActor> actor )
        {
            m_actor = actor;
        }

        bool Transform::isLocalDirty() const
        {
            // FB_ASSERT(m_state);
            return m_isLocalDirty;
        }

        void Transform::setLocalDirty( bool localDirty, bool cascade )
        {
            // FB_ASSERT(m_state);
            m_isLocalDirty = localDirty;

            if( m_isLocalDirty )
            {
                auto actor = getActor();
                if( actor )
                {
                    auto applicationManager = core::IApplicationManager::instance();
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

        bool Transform::isDirty() const
        {
            return m_isDirty;
        }

        void Transform::setDirty( bool dirty, bool cascade )
        {
            m_isDirty = dirty;

            if( m_isDirty )
            {
                if( auto actor = getActor() )
                {
                    auto applicationManager = core::IApplicationManager::instance();
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

        void Transform::setEnabled( bool enabled )
        {
            m_enabled = enabled;
        }

        bool Transform::isEnabled() const
        {
            return m_enabled;
        }

        SmartPtr<IStateContext> Transform::getStateObject() const
        {
            return m_stateObject;
        }

        void Transform::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        Transform3<real_Num> Transform::getLocalTransform() const
        {
            return m_localTransform;
        }

        void Transform::setLocalTransform( Transform3<real_Num> transform )
        {
            m_localTransform = transform;
        }

        Transform3<real_Num> Transform::getWorldTransform() const
        {
            return m_worldTransform;
        }

        void Transform::setWorldTransform( Transform3<real_Num> transform )
        {
            m_worldTransform = transform;
        }

        Vector3<real_Num> Transform::getLocalPosition() const
        {
            return m_localTransform.getPosition();
        }

        void Transform::setLocalPosition( const Vector3<real_Num> &localPosition )
        {
            auto p = getLocalPosition();
            if( !MathUtil<real_Num>::equals( p, localPosition ) )
            {
                m_localTransform.setPosition( localPosition );
                setDirty( true );
            }
        }

        Vector3<real_Num> Transform::getLocalScale() const
        {
            return m_localTransform.getScale();
        }

        void Transform::setLocalScale( const Vector3<real_Num> &localScale )
        {
            auto s = getScale();
            if( !MathUtil<real_Num>::equals( s, localScale ) )
            {
                m_localTransform.setScale( localScale );
                setDirty( true );
            }
        }

        Quaternion<real_Num> Transform::getLocalOrientation() const
        {
            return m_localTransform.getOrientation();
        }

        void Transform::setLocalOrientation( const Quaternion<real_Num> &localOrientation )
        {
            auto r = getLocalOrientation();
            if( !MathUtil<real_Num>::equals( r, localOrientation ) )
            {
                m_localTransform.setOrientation( localOrientation );
                setDirty( true );
            }
        }

        Vector3<real_Num> Transform::getLocalRotation() const
        {
            Euler<real_Num> euler( getLocalOrientation() );
            return Vector3<real_Num>( euler.pitch() * Math<real_Num>::rad_to_deg(),
                                      euler.yaw() * Math<real_Num>::rad_to_deg(),
                                      euler.roll() * Math<real_Num>::rad_to_deg() );
        }

        void Transform::setLocalRotation( const Vector3<real_Num> &localRotation )
        {
            Euler<real_Num> euler( localRotation.Y() * Math<real_Num>::deg_to_rad(),
                                   localRotation.X() * Math<real_Num>::deg_to_rad(),
                                   localRotation.Z() * Math<real_Num>::deg_to_rad() );

            auto orientation = euler.toQuaternion();
            setLocalOrientation( orientation );
        }

        Vector3<real_Num> Transform::getPosition() const
        {
            return m_worldTransform.getPosition();
        }

        void Transform::setPosition( const Vector3<real_Num> &position )
        {
            auto p = getPosition();
            if( !MathUtil<real_Num>::equals( p, position ) )
            {
                m_worldTransform.setPosition( position );
                setLocalDirty( true );
            }
        }

        Vector3<real_Num> Transform::getScale() const
        {
            return m_worldTransform.getScale();
        }

        void Transform::setScale( const Vector3<real_Num> &scale )
        {
            auto s = getScale();
            if( !MathUtil<real_Num>::equals( s, scale ) )
            {
                m_worldTransform.setScale( scale );
                setLocalDirty( true );
            }
        }

        Quaternion<real_Num> Transform::getOrientation() const
        {
            return m_worldTransform.getOrientation();
        }

        void Transform::setOrientation( const Quaternion<real_Num> &orientation )
        {
            auto r = getOrientation();
            if( !MathUtil<real_Num>::equals( r, orientation ) )
            {
                m_worldTransform.setOrientation( orientation );
                setLocalDirty( true );
            }
        }

        Vector3<real_Num> Transform::getRotation() const
        {
            Euler<real_Num> euler( getOrientation() );
            return Vector3<real_Num>( euler.pitch() * Math<real_Num>::rad_to_deg(),
                                      euler.yaw() * Math<real_Num>::rad_to_deg(),
                                      euler.roll() * Math<real_Num>::rad_to_deg() );
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
                        SpinRWMutex::ScopedLock lock( m_mutex, true);
                        auto localTransform = getLocalTransform();

                        auto worldTransform = getWorldTransform();
                        worldTransform.setTransform( localTransform );
                        setWorldTransform( worldTransform );
                    }
                }
                else
                {
                    SpinRWMutex::ScopedLock lock( m_mutex, true);
                    auto localTransform = getLocalTransform();

                    auto worldTransform = getWorldTransform();
                    worldTransform.setTransform( localTransform );
                    setWorldTransform( worldTransform );
                }

                m_isDirty = false;
            }
        }

        SmartPtr<Properties> Transform::getProperties() const
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

            Vector3<real_Num> localRotation;
            localOrientation.toDegrees( localRotation );

            Vector3<real_Num> rotation;
            orientation.toDegrees( rotation );

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

                if( currentRotation != orientation )
                {
                    worldTransform.setOrientation( orientation );
                }

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

    }  // namespace scene
}  // end namespace fb
