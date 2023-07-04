#ifndef FBPhysxVehicle_h__
#define FBPhysxVehicle_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IPhysicsVehicle3.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace physics
    {

        class PhysxVehicle3 : public SharedObject<IPhysicsVehicle3>
        {
        public:
            PhysxVehicle3();
            ~PhysxVehicle3() override;

            void initialise( SmartPtr<scene::IDirector> objectTemplate );

            void update( const s32 &task, const time_interval &t, const time_interval &dt );

            IPhysicsVehicleWheel3 *addWheel() override;

            IPhysicsVehicleWheel3 *getWheel( u32 wheelIndex ) const override;

            u32 getNumWheels() const override;

            void finalize() override;

            void applyEngineForce( f32 engineForce, u32 wheelIndex ) override;

            void setBrake( f32 brakeForce, u32 wheelIndex ) override;

            void setSteeringValue( f32 steeringValue, u32 wheelIndex ) override;

            void setPosition( const Vector3F &position ) override;
            Vector3F getPosition() const override;

            void setOrientation( const QuaternionF &orientation ) override;
            QuaternionF getOrientation() const override;

            void setVelocity( const Vector3F &velocity ) override;

            Vector3F getVelocity() const override;

            void setMaterialId( u32 materialId ) override;

            u32 getMaterialId() const override;

            AABB3F getLocalAABB() const override;

            AABB3F getWorldAABB() const override;

            void setEnabled( bool enabled ) override;

            bool isEnabled() const override;

            SmartPtr<IPhysicsVehicleInput3> &getVehicleInput() override;
            const SmartPtr<IPhysicsVehicleInput3> &getVehicleInput() const override;

            physx::PxVehicleDrive4W *getVehicle() const;
            void setVehicle( physx::PxVehicleDrive4W *val );

            Array<Transform3F> getWheelTransformations() const override;

        protected:
            SmartPtr<IPhysicsVehicleInput3> m_vehicleInput;
            physx::PxVehicleDrive4W *m_vehicle;

            Vector3F m_position;
            QuaternionF m_orientation;

            Array<Transform3F> m_wheelTransforms;
        };

    }  // namespace physics
}  // namespace fb

// end namespace fb

#endif  // FBPhysxVehicle_h__
