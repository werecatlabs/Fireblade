#include "FBPhysx/FBPhysxPCH.h"
#include "FBPhysx/FBPhysxVehicle.h"
#include "FBPhysx/FBPhysxVehicleInput.h"
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

using namespace physx;

PxVehicleKeySmoothingData gKeySmoothingData = { {
                                                    3.0f,   //rise rate eANALOG_INPUT_ACCEL
                                                    3.0f,   //rise rate eANALOG_INPUT_BRAKE
                                                    10.0f,  //rise rate eANALOG_INPUT_HANDBRAKE
                                                    2.5f,   //rise rate eANALOG_INPUT_STEER_LEFT
                                                    2.5f,   //rise rate eANALOG_INPUT_STEER_RIGHT
                                                },
                                                {
                                                    5.0f,   //fall rate eANALOG_INPUT__ACCEL
                                                    5.0f,   //fall rate eANALOG_INPUT__BRAKE
                                                    10.0f,  //fall rate eANALOG_INPUT__HANDBRAKE
                                                    5.0f,   //fall rate eANALOG_INPUT_STEER_LEFT
                                                    5.0f    //fall rate eANALOG_INPUT_STEER_RIGHT
                                                } };

PxVehiclePadSmoothingData gCarPadSmoothingData = { {
                                                       6.0f,   //rise rate eANALOG_INPUT_ACCEL
                                                       6.0f,   //rise rate eANALOG_INPUT_BRAKE
                                                       12.0f,  //rise rate eANALOG_INPUT_HANDBRAKE
                                                       2.5f,   //rise rate eANALOG_INPUT_STEER_LEFT
                                                       2.5f,   //rise rate eANALOG_INPUT_STEER_RIGHT
                                                   },
                                                   {
                                                       10.0f,  //fall rate eANALOG_INPUT_ACCEL
                                                       10.0f,  //fall rate eANALOG_INPUT_BRAKE
                                                       12.0f,  //fall rate eANALOG_INPUT_HANDBRAKE
                                                       5.0f,   //fall rate eANALOG_INPUT_STEER_LEFT
                                                       5.0f    //fall rate eANALOG_INPUT_STEER_RIGHT
                                                   } };

PxF32 gSteerVsForwardSpeedData[2 * 8] = { 0.0f,       0.75f,      5.0f,       0.75f,
                                          30.0f,      0.125f,     120.0f,     0.1f,
                                          PX_MAX_F32, PX_MAX_F32, PX_MAX_F32, PX_MAX_F32,
                                          PX_MAX_F32, PX_MAX_F32, PX_MAX_F32, PX_MAX_F32 };
PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable( gSteerVsForwardSpeedData, 4 );

namespace fb
{
    namespace physics
    {

        PhysxVehicle3::PhysxVehicle3()
        {
            m_vehicleInput = SmartPtr<IPhysicsVehicleInput3>( new PhysxVehicleInput );
        }

        PhysxVehicle3::~PhysxVehicle3()
        {
            unload( nullptr );
        }

        void PhysxVehicle3::initialise( SmartPtr<scene::IDirector> objectTemplate )
        {
        }

        void PhysxVehicle3::update( const s32 &task, const time_interval &t, const time_interval &dt )
        {
            PxVehicleDriveDynData &driveDynData = m_vehicle->mDriveDynData;

            driveDynData.forceGearChange( PxVehicleGearsData::eFIRST );

            SmartPtr<PhysxVehicleInput> input;  // = m_vehicleInput;
            PxVehicleDrive4WRawInputData pxVehicleInput = *input->getInputs();
            PxVehicleDrive4WSmoothDigitalRawInputsAndSetAnalogInputs(
                gKeySmoothingData, gSteerVsForwardSpeedTable, pxVehicleInput, (physx::PxReal)dt, false, *m_vehicle );

#define PX_MAX_NUM_WHEELS 4
            const PxRigidDynamic *actor = m_vehicle->getRigidDynamicActor();
            PxShape *shapeBuffer[PX_MAX_NUM_WHEELS];
            actor->getShapes( shapeBuffer, m_vehicle->mWheelsSimData.getNbWheels() );
            const PxTransform vehGlobalPose = actor->getGlobalPose();
            const PxU32 numWheels = m_vehicle->mWheelsSimData.getNbWheels();

            m_wheelTransforms.resize( 0 );
            for( PxU32 j = 0; j < numWheels; j++ )
            {
                const PxTransform wheelTransform = shapeBuffer[j]->getLocalPose();

                Transform3F transform;
                //transform.position = Vector3F(wheelTransform.p.x, wheelTransform.p.y, wheelTransform.p.z);
                //transform.orientation = QuaternionF(wheelTransform.q.w, wheelTransform.q.x, wheelTransform.q.y, wheelTransform.q.z);
                m_wheelTransforms.push_back( transform );
            }

            PxTransform transform = m_vehicle->getRigidDynamicActor()->getGlobalPose();
            m_position = Vector3F( transform.p.x, transform.p.y, transform.p.z );
            m_orientation = QuaternionF( transform.q.w, transform.q.x, transform.q.y, transform.q.z );
        }

        IPhysicsVehicleWheel3 *PhysxVehicle3::addWheel()
        {
            return nullptr;
        }

        IPhysicsVehicleWheel3 *PhysxVehicle3::getWheel( u32 wheelIndex ) const
        {
            return nullptr;
        }

        u32 PhysxVehicle3::getNumWheels() const
        {
            return 0;
        }

        void PhysxVehicle3::finalize()
        {
        }

        void PhysxVehicle3::applyEngineForce( f32 engineForce, u32 wheelIndex )
        {
        }

        void PhysxVehicle3::setBrake( f32 brakeForce, u32 wheelIndex )
        {
        }

        void PhysxVehicle3::setSteeringValue( f32 steeringValue, u32 wheelIndex )
        {
        }

        void PhysxVehicle3::setPosition( const Vector3F &position )
        {
            //RecursiveMutex::ScopedLock lock(PhysxMutex);

            //FB_ASSERT(eVEHICLE_TYPE_DRIVE4W==m_vehicle->getVehicleType());
            auto vehDrive4W = m_vehicle;
            //Set the car back to its rest state.
            vehDrive4W->setToRestState();
            //Set the car to first gear.
            vehDrive4W->mDriveDynData.forceGearChange( PxVehicleGearsData::eFIRST );

            PxTransform transform = m_vehicle->getRigidDynamicActor()->getGlobalPose();
            transform.p = PxVec3( position.X(), position.Y(), position.Z() );
            m_vehicle->getRigidDynamicActor()->setGlobalPose( transform );
        }

        Vector3F PhysxVehicle3::getPosition() const
        {
            return m_position;
        }

        void PhysxVehicle3::setOrientation( const QuaternionF &orientation )
        {
        }

        QuaternionF PhysxVehicle3::getOrientation() const
        {
            return m_orientation;
        }

        void PhysxVehicle3::setVelocity( const Vector3F &velocity )
        {
        }

        Vector3F PhysxVehicle3::getVelocity() const
        {
            return Vector3F::zero();
        }

        void PhysxVehicle3::setMaterialId( u32 materialId )
        {
        }

        u32 PhysxVehicle3::getMaterialId() const
        {
            return 0;
        }

        AABB3F PhysxVehicle3::getLocalAABB() const
        {
            return AABB3F();
        }

        AABB3F PhysxVehicle3::getWorldAABB() const
        {
            return AABB3F();
        }

        void PhysxVehicle3::setEnabled( bool enabled )
        {
        }

        bool PhysxVehicle3::isEnabled() const
        {
            return false;
        }

        PxVehicleDrive4W *PhysxVehicle3::getVehicle() const
        {
            return m_vehicle;
        }

        void PhysxVehicle3::setVehicle( PxVehicleDrive4W *val )
        {
            m_vehicle = val;
        }

        SmartPtr<IPhysicsVehicleInput3> &PhysxVehicle3::getVehicleInput()
        {
            return m_vehicleInput;
        }

        const SmartPtr<IPhysicsVehicleInput3> &PhysxVehicle3::getVehicleInput() const
        {
            return m_vehicleInput;
        }

        Array<Transform3F> PhysxVehicle3::getWheelTransformations() const
        {
            return m_wheelTransforms;
        }

    }  // namespace physics
}  // namespace fb

// end namespace fb
