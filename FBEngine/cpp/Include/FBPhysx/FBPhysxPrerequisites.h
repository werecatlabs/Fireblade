#ifndef FBPhysxPrerequisites_h__
#define FBPhysxPrerequisites_h__

#include <FBCore/FBCorePrerequisites.h>

//
// forward declarations
//
namespace physx
{

    class PxFoundation;
    class PxAllocatorCallback;
    class PxCapsuleController;
    class PxControllerManager;
    class PxCooking;
    class PxD6Joint;
    class PxErrorCallback;
    class PxMaterial;
    class PxPhysics;
    class PxRigidActor;
    class PxRigidDynamic;
    class PxRigidStatic;
    class PxScene;
    class PxVehicleDrive4W;
    class PxCapsuleController;
    class PxControllerManager;
    class PxD6Joint;
    class PxFixedJoint;
    class PxShape;
    class PxDefaultCpuDispatcher;
    class PxContactModifyPair;
    class PxOutputStream;
    struct PxTriggerPair;
    struct PxConstraintInfo;

} // end namespace physx

namespace fb
{
    namespace physics
    {
        class PhysxCharacterController;
        class PhysxManager;
        class PhysxRigidDynamic;
        class PhysxRigidStatic;
        class PhysxVehicle3;
        class PhysxVehicleInput;
        class PhysxScene;
        class PhysxVehicleManager;
        class PhysxCooker;
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxPrerequisites_h__
