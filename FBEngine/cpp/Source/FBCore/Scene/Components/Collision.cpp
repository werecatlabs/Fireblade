#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Collision.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb, Collision, Component );

    auto Collision::getMaterial() const -> SmartPtr<physics::IPhysicsMaterial3>
    {
        return m_material;
    }

    void Collision::setMaterial( SmartPtr<physics::IPhysicsMaterial3> material )
    {
        m_material = material;
    }

    auto Collision::getShape() const -> SmartPtr<physics::IPhysicsShape3>
    {
        return m_shape;
    }

    void Collision::setShape( SmartPtr<physics::IPhysicsShape3> shape )
    {
        m_shape = shape;
    }

    void Collision::load( SmartPtr<ISharedObject> data )
    {
        Component::load( data );
    }

    void Collision::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            if( physicsManager )
            {
                auto shape = getShape();
                if( shape )
                {
                    physicsManager->removeCollisionShape( shape );
                    setShape( nullptr );
                }
            }

            Component::unload( data );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Collision::getExtents() const -> Vector3<real_Num>
    {
        return m_extents;
    }

    void Collision::setExtents( const Vector3<real_Num> &extents )
    {
        m_extents = extents;
    }

    auto Collision::getPosition() const -> Vector3<real_Num>
    {
        return m_position;
    }

    void Collision::setPosition( const Vector3<real_Num> &position )
    {
        m_position = position;
    }

    auto Collision::getRadius() const -> f32
    {
        return m_radius;
    }

    void Collision::setRadius( f32 radius )
    {
        m_radius = radius;
    }

    auto Collision::getBoundingBox() const -> AABB3F
    {
        return {};
    }

    auto Collision::getRigidBody() const -> SmartPtr<Rigidbody>
    {
        return m_rigidBody;
    }

    void Collision::setRigidBody( SmartPtr<Rigidbody> rigidBody )
    {
        m_rigidBody = rigidBody;
    }

    auto Collision::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> objects;

        objects.emplace_back( m_rigidBody );
        objects.emplace_back( m_material );
        objects.emplace_back( m_shape );

        return objects;
    }

    auto Collision::getProperties() const -> SmartPtr<Properties>
    {
        try
        {
            auto properties = Component::getProperties();

            properties->setProperty( "isTrigger", m_isTrigger );
            properties->setProperty( "extents", m_extents );

            return properties;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    void Collision::setProperties( SmartPtr<Properties> properties )
    {
        try
        {
            properties->getPropertyValue( "isTrigger", m_isTrigger );

            Vector3<real_Num> extents;
            properties->getPropertyValue( "extents", extents );

            if( !MathUtil<real_Num>::equals( extents, m_extents ) )
            {
                setExtents( extents );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Collision::isTrigger() const -> bool
    {
        return m_isTrigger;
    }

    void Collision::setTrigger( bool trigger )
    {
        m_isTrigger = trigger;
    }

    auto Collision::isValid() const -> bool
    {
        switch( auto state = getState() )
        {
        case IComponent::State::None:
        {
            return true;
        }
        break;
        case IComponent::State::Edit:
        case IComponent::State::Play:
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                if( m_shape )
                {
                    if( m_shape->isValid() )
                    {
                        return true;
                    }
                }
            }
            else if( loadingState == LoadingState::Unloaded )
            {
                if( m_shape )
                {
                    return false;
                }
            }
        }
        break;
        default:
        {
        }
        }

        return false;
    }

    void Collision::updateTransform()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto actor = getActor() )
            {
                auto physicsManager = applicationManager->getPhysicsManager();
                if( physicsManager )
                {
                    auto shape = getShape();
                    if( shape )
                    {
                        auto localTransform = actor->getWorldTransform();
                        localTransform.setPosition( Vector3F::zero() );
                        localTransform.setOrientation( QuaternionF::identity() );
                        shape->setLocalPose( localTransform );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Collision::createPhysicsShape()
    {
    }

    void Collision::updateRigidBody()
    {
        if( auto actor = getActor() )
        {
            auto rigidBodies = actor->getComponentsByType<Rigidbody>();
            for( auto rigidBody : rigidBodies )
            {
                rigidBody->updateShapes();
            }
        }
    }

}  // namespace fb::scene
