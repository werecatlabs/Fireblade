#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Collision.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Base/Exception.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Math/Core/MathUtil.h>

namespace fb
{
    namespace scene
    {

        FB_CLASS_REGISTER_DERIVED( fb, Collision, Component );

        SmartPtr<physics::IPhysicsMaterial3> Collision::getMaterial() const
        {
            return m_material;
        }

        void Collision::setMaterial( SmartPtr<physics::IPhysicsMaterial3> material )
        {
            m_material = material;
        }

        SmartPtr<physics::IPhysicsShape3> Collision::getShape() const
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
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                if( physicsManager )
                {
                    auto shape = getShape();
                    if( shape )
                    {
                        physicsManager->destroyCollisionShape( shape );
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

        Vector3<real_Num> Collision::getExtents() const
        {
            return m_extents;
        }

        void Collision::setExtents( const Vector3<real_Num> &extents )
        {
            m_extents = extents;
        }

        Vector3<real_Num> Collision::getPosition() const
        {
            return m_position;
        }

        void Collision::setPosition( const Vector3<real_Num> &position )
        {
            m_position = position;
        }

        f32 Collision::getRadius() const
        {
            return m_radius;
        }

        void Collision::setRadius( f32 radius )
        {
            m_radius = radius;
        }

        AABB3F Collision::getBoundingBox() const
        {
            return AABB3F();
        }

        SmartPtr<Rigidbody> Collision::getRigidBody() const
        {
            return m_rigidBody;
        }

        void Collision::setRigidBody( SmartPtr<Rigidbody> rigidBody )
        {
            m_rigidBody = rigidBody;
        }

        Array<SmartPtr<ISharedObject>> Collision::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> objects;

            objects.push_back( m_rigidBody );
            objects.push_back( m_material );
            objects.push_back( m_shape );

            return objects;
        }

        SmartPtr<Properties> Collision::getProperties() const
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

        bool Collision::isTrigger() const
        {
            return m_isTrigger;
        }

        void Collision::setTrigger( bool trigger )
        {
            m_isTrigger = trigger;
        }

        bool Collision::isValid() const
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

    }  // namespace scene
}  // end namespace fb
