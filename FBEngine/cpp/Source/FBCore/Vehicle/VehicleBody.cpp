#include <FBCore/FBCorePCH.h>
#include <FBCore/Vehicle/VehicleBody.h>
#include <FBCore/FBCore.h>

namespace fb
{
    VehicleBody::VehicleBody()
    {
    }

    VehicleBody::~VehicleBody()
    {
        unload( nullptr );
    }

    void VehicleBody::unload( SmartPtr<ISharedObject> data )
    {
        m_parentAircraft = nullptr;
    }

    Vector3<real_Num> VehicleBody::getVelocity() const
    {
        return m_velocity;
    }

    void VehicleBody::setVelocity( const Vector3<real_Num> &val )
    {
        m_velocity = val;
    }

    Vector3<real_Num> VehicleBody::getAngularVelocity() const
    {
        return m_angularVelocity;
    }

    void VehicleBody::setAngularVelocity( const Vector3<real_Num> &val )
    {
        m_angularVelocity = val;
    }

    Vector3<real_Num> VehicleBody::getWorldCenterOfMass() const
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
        m_parentAircraft->addLocalForce( 0, val, pos );
    }

    void VehicleBody::addLocalForceAtLocalPosition( const Vector3<real_Num> &val,
                                                     const Vector3<real_Num> &pos )
    {
        m_parentAircraft->addLocalForce( 0, val, pos );
    }

    void VehicleBody::addForceAtPosition( const Vector3<real_Num> &val, const Vector3<real_Num> &pos,
                                           physics::PhysicsTypes::ForceMode forceMode )
    {
        m_parentAircraft->addForce( 0, val, pos );
    }

    void VehicleBody::addForceAtPosition( const Vector3<real_Num> &val, const Vector3<real_Num> &pos )
    {
        m_parentAircraft->addForce( 0, val, pos );
    }

    bool VehicleBody::castLocalRay( const Ray3<real_Num> &ray, SmartPtr<physics::IRaycastHit> &data )
    {
        FB_ASSERT( ray.isValid() );
        FB_ASSERT( data );

        auto callback = m_parentAircraft->getVehicleCallback();
        if( callback )
        {
            return callback->castLocalRay( ray, data );
        }

        return false;
    }

    bool VehicleBody::castWorldRay( const Ray3<real_Num> &ray, SmartPtr<physics::IRaycastHit> &data )
    {
        FB_ASSERT( ray.isValid() );
        FB_ASSERT( data );

        auto callback = m_parentAircraft->getVehicleCallback();
        if( callback )
        {
            return callback->castWorldRay( ray, data );
        }

        return false;
    }

    real_Num VehicleBody::getMass() const
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

    Vector3<real_Num> VehicleBody::getLocalVelocity() const
    {
        return m_localVelocity;
    }

    void VehicleBody::setLocalVelocity( Vector3<real_Num> val )
    {
        m_localVelocity = val;
    }

    Vector3<real_Num> VehicleBody::getLocalAngularVelocity() const
    {
        return m_localAngularVelocity;
    }

    void VehicleBody::setLocalAngularVelocity( Vector3<real_Num> val )
    {
        m_localAngularVelocity = val;
    }

    void VehicleBody::addTorque( const Vector3<real_Num> &val )
    {
        m_parentAircraft->addTorque( 0, val );
    }

    void VehicleBody::addLocalTorque( const Vector3<real_Num> &val )
    {
        m_parentAircraft->addLocalTorque( 0, val );
    }

    SmartPtr<IVehicleController> &VehicleBody::getParentAircraft()
    {
        return m_parentAircraft;
    }

    const SmartPtr<IVehicleController> &VehicleBody::getParentAircraft() const
    {
        return m_parentAircraft;
    }

    void VehicleBody::setParentVehicle( SmartPtr<IVehicleController> val )
    {
        m_parentAircraft = val;
    }

    void VehicleBody::update()
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        m_velocity = m_parentAircraft->getLinearVelocity();
        m_angularVelocity = m_parentAircraft->getAngularVelocity();

        m_localVelocity = m_parentAircraft->getLocalLinearVelocity();
        m_localAngularVelocity = m_parentAircraft->getLocalAngularVelocity();

        FB_ASSERT( m_velocity.length() < 1e4 );
        FB_ASSERT( m_angularVelocity.length() < 1e4 );

        auto aircraftWorldTransform = m_parentAircraft->getWorldTransform();
        auto worldCenterOfMass = aircraftWorldTransform.transformPoint( m_parentAircraft->getCG() );
        setWorldCenterOfMass( worldCenterOfMass );
    }

    Vector3<real_Num> VehicleBody::getPointVelocity( const Vector3<real_Num> &p )
    {
        return m_parentAircraft->getPointVelocity( p );
    }

    bool VehicleBody::isValid() const
    {
        return m_parentAircraft != nullptr;
    }
}  // namespace fb
