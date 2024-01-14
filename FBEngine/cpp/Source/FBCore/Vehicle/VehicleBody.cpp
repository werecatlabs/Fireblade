#include <FBCore/FBCorePCH.h>
#include <FBCore/Vehicle/VehicleBody.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/Vehicle/IVehicleCallback.h>

namespace fb
{
    VehicleBody::VehicleBody() = default;

    VehicleBody::~VehicleBody()
    {
        unload( nullptr );
    }

    void VehicleBody::unload( SmartPtr<ISharedObject> data )
    {
        m_parentVehicle = nullptr;
    }

    auto VehicleBody::getVelocity() const -> Vector3<real_Num>
    {
        return m_velocity;
    }

    void VehicleBody::setVelocity( const Vector3<real_Num> &val )
    {
        m_velocity = val;
    }

    auto VehicleBody::getAngularVelocity() const -> Vector3<real_Num>
    {
        return m_angularVelocity;
    }

    void VehicleBody::setAngularVelocity( const Vector3<real_Num> &val )
    {
        m_angularVelocity = val;
    }

    auto VehicleBody::getWorldCenterOfMass() const -> Vector3<real_Num>
    {
        return m_worldCenterOfMass;
    }

    void VehicleBody::setWorldCenterOfMass( const Vector3<real_Num> &val )
    {
        m_worldCenterOfMass = val;
    }

    void VehicleBody::addLocalForceAtPosition( const Vector3<real_Num> &val,
                                               const Vector3<real_Num> &pos,
                                               physics::PhysicsTypes::ForceMode forceMode )
    {
        m_parentVehicle->addLocalForce( 0, val, pos );
    }

    void VehicleBody::addLocalForceAtLocalPosition( const Vector3<real_Num> &val,
                                                    const Vector3<real_Num> &pos )
    {
        m_parentVehicle->addLocalForce( 0, val, pos );
    }

    void VehicleBody::addForceAtPosition( const Vector3<real_Num> &val, const Vector3<real_Num> &pos,
                                          physics::PhysicsTypes::ForceMode forceMode )
    {
        m_parentVehicle->addForce( 0, val, pos );
    }

    void VehicleBody::addForceAtPosition( const Vector3<real_Num> &val, const Vector3<real_Num> &pos )
    {
        m_parentVehicle->addForce( 0, val, pos );
    }

    auto VehicleBody::castLocalRay( const Ray3<real_Num> &ray, SmartPtr<physics::IRaycastHit> &data )
        -> bool
    {
        FB_ASSERT( ray.isValid() );
        FB_ASSERT( data );

        auto callback = m_parentVehicle->getVehicleCallback();
        if( callback )
        {
            return callback->castLocalRay( ray, data );
        }

        return false;
    }

    auto VehicleBody::castWorldRay( const Ray3<real_Num> &ray, SmartPtr<physics::IRaycastHit> &data )
        -> bool
    {
        FB_ASSERT( ray.isValid() );
        FB_ASSERT( data );

        auto callback = m_parentVehicle->getVehicleCallback();
        if( callback )
        {
            return callback->castWorldRay( ray, data );
        }

        return false;
    }

    auto VehicleBody::getMass() const -> real_Num
    {
        FB_ASSERT( m_mass > 0 );
        FB_ASSERT( m_mass < static_cast<real_Num>( 1e10 ) );
        return m_mass;
    }

    void VehicleBody::setMass( real_Num mass )
    {
        FB_ASSERT( mass > 0 );
        FB_ASSERT( mass < static_cast<real_Num>( 1e10 ) );

        m_mass = mass;

        FB_ASSERT( m_mass > 0 );
        FB_ASSERT( m_mass < static_cast<real_Num>( 1e10 ) );
    }

    auto VehicleBody::getLocalVelocity() const -> Vector3<real_Num>
    {
        return m_localVelocity;
    }

    void VehicleBody::setLocalVelocity( Vector3<real_Num> val )
    {
        m_localVelocity = val;
    }

    auto VehicleBody::getLocalAngularVelocity() const -> Vector3<real_Num>
    {
        return m_localAngularVelocity;
    }

    void VehicleBody::setLocalAngularVelocity( Vector3<real_Num> val )
    {
        m_localAngularVelocity = val;
    }

    void VehicleBody::addTorque( const Vector3<real_Num> &val )
    {
        m_parentVehicle->addTorque( 0, val );
    }

    void VehicleBody::addLocalTorque( const Vector3<real_Num> &val )
    {
        m_parentVehicle->addLocalTorque( 0, val );
    }

    auto VehicleBody::getParentVehicle() -> SmartPtr<IVehicleController> &
    {
        return m_parentVehicle;
    }

    auto VehicleBody::getParentVehicle() const -> const SmartPtr<IVehicleController> &
    {
        return m_parentVehicle;
    }

    void VehicleBody::setParentVehicle( SmartPtr<IVehicleController> val )
    {
        m_parentVehicle = val;
    }

    void VehicleBody::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        m_velocity = m_parentVehicle->getLinearVelocity();
        m_angularVelocity = m_parentVehicle->getAngularVelocity();

        m_localVelocity = m_parentVehicle->getLocalLinearVelocity();
        m_localAngularVelocity = m_parentVehicle->getLocalAngularVelocity();

        FB_ASSERT( m_velocity.length() < 1e4 );
        FB_ASSERT( m_angularVelocity.length() < 1e4 );

        auto worldTransform = m_parentVehicle->getWorldTransform();
        auto worldCenterOfMass = worldTransform.transformPoint( m_parentVehicle->getCG() );

        setWorldCenterOfMass( worldCenterOfMass );
    }

    auto VehicleBody::getPointVelocity( const Vector3<real_Num> &p ) -> Vector3<real_Num>
    {
        return m_parentVehicle->getPointVelocity( p );
    }

    auto VehicleBody::isValid() const -> bool
    {
        return m_parentVehicle != nullptr;
    }
}  // namespace fb
