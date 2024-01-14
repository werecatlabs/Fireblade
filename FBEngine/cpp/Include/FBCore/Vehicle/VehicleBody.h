#ifndef __VehicleBody_h__
#define __VehicleBody_h__

#include <FBCore/Vehicle/VehicleComponent.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Interface/Vehicle/IVehicleBody.h>
#include <FBCore/Interface/Physics/FBPhysicsTypes.h>

namespace fb
{

    /** Implementation of the vehicle body. */
    class VehicleBody : public VehicleComponent<IVehicleBody>
    {
    public:
        /** Constructor. */
        VehicleBody();

        /** Destructor. */
        ~VehicleBody() override;

        /** @copydoc IVehicleBody::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IVehicleBody::update */
        void update() override;

        /** @copydoc IVehicleBody::isValid */
        bool isValid() const override;

        /** @copydoc IVehicleBody::getVelocity */
        Vector3<real_Num> getVelocity() const override;

        /** @copydoc IVehicleBody::setVelocity */
        void setVelocity( const Vector3<real_Num> &val ) override;

        Vector3<real_Num> getAngularVelocity() const override;
        void setAngularVelocity( const Vector3<real_Num> &val ) override;

        Vector3<real_Num> getWorldCenterOfMass() const override;
        void setWorldCenterOfMass( const Vector3<real_Num> &val ) override;

        void addLocalForceAtPosition( const Vector3<real_Num> &val, const Vector3<real_Num> &pos,
                                      physics::PhysicsTypes::ForceMode forceMode );
        void addForceAtPosition( const Vector3<real_Num> &val, const Vector3<real_Num> &pos,
                                 physics::PhysicsTypes::ForceMode forceMode );
        void addTorque( const Vector3<real_Num> &val ) override;
        void addLocalTorque( const Vector3<real_Num> &val ) override;

        SmartPtr<IVehicleController> &getParentVehicle();
        const SmartPtr<IVehicleController> &getParentVehicle() const;
        void setParentVehicle( SmartPtr<IVehicleController> val );

        Vector3<real_Num> getPointVelocity( const Vector3<real_Num> &p ) override;

        void addLocalForceAtLocalPosition( const Vector3<real_Num> &val,
                                           const Vector3<real_Num> &pos ) override;
        void addForceAtPosition( const Vector3<real_Num> &val, const Vector3<real_Num> &pos ) override;

        bool castLocalRay( const Ray3<real_Num> &ray, SmartPtr<physics::IRaycastHit> &data ) override;
        bool castWorldRay( const Ray3<real_Num> &ray, SmartPtr<physics::IRaycastHit> &data ) override;

        real_Num getMass() const override;
        void setMass( real_Num mass ) override;

        Vector3<real_Num> getLocalVelocity() const override;
        void setLocalVelocity( Vector3<real_Num> val ) override;

        Vector3<real_Num> getLocalAngularVelocity() const override;
        void setLocalAngularVelocity( Vector3<real_Num> val ) override;

    protected:
        SmartPtr<IVehicleController> m_parentVehicle;

        Vector3<real_Num> m_velocity;
        Vector3<real_Num> m_angularVelocity;

        Vector3<real_Num> m_localVelocity;
        Vector3<real_Num> m_localAngularVelocity;

        Vector3<real_Num> m_worldCenterOfMass;
        real_Num m_mass = static_cast<real_Num>( 1500.0 );
    };
}  // end namespace fb

#endif  // Rigidbody_h__
