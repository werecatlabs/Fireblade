//
// Created by Zane Desir on 31/10/2021.
//

#ifndef FB_CPHYSICSMANAGER_H
#define FB_CPHYSICSMANAGER_H

#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace physics
    {
        class CPhysicsManager : public SharedObject<IPhysicsManager>
        {
        public:
            CPhysicsManager();
            ~CPhysicsManager() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            bool getEnableDebugDraw() const override;

            void setEnableDebugDraw( bool enableDebugDraw ) override;

            SmartPtr<IPhysicsMaterial3> createMaterial() override;

            void destroyMaterial( SmartPtr<IPhysicsMaterial3> material ) override;

            SmartPtr<IPhysicsScene3> createScene() override;

            void destroyScene( SmartPtr<IPhysicsScene3> scene ) override;

            bool destroyCollisionShape( SmartPtr<IPhysicsShape3> collisionShape ) override;

            bool destroyPhysicsBody( SmartPtr<IRigidBody3> body ) override;

            SmartPtr<ICharacterController3> addCharacter() override;

            SmartPtr<IRigidStatic3> createRigidStatic( const Transform3<real_Num> &transform ) override;

            SmartPtr<IRigidDynamic3> createRigidDynamic(
                const Transform3<real_Num> &transform ) override;

            SmartPtr<IRigidStatic3> createRigidStatic(
                SmartPtr<IPhysicsShape3> collisionShape ) override;

            SmartPtr<IRigidStatic3> createRigidStatic( SmartPtr<IPhysicsShape3> collisionShape,
                                                       SmartPtr<Properties> properties ) override;

            SmartPtr<IPhysicsSoftBody3> createSoftBody( const String &filePath ) override;

            SmartPtr<IPhysicsVehicle3> addVehicle( SmartPtr<IRigidBody3> chassis,
                                                   const SmartPtr<Properties> &properties ) override;

            SmartPtr<IPhysicsVehicle3> addVehicle( SmartPtr<scene::IDirector> vehicleTemplate ) override;

            bool destroyVehicle( SmartPtr<IPhysicsVehicle3> vehicle ) override;

            bool rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &direction,
                          Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal, u32 collisionType = 0,
                          u32 collisionMask = 0 ) override;

            bool intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                             Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                             SmartPtr<ISharedObject> &object, u32 collisionType = 0,
                             u32 collisionMask = 0 ) override;

            SmartPtr<IConstraintD6> d6JointCreate( SmartPtr<IPhysicsBody3> actor0,
                                                   const Transform3<real_Num> &localFrame0,
                                                   SmartPtr<IPhysicsBody3> actor1,
                                                   const Transform3<real_Num> &localFrame1 ) override;

            SmartPtr<IConstraintFixed3> fixedJointCreate(
                SmartPtr<IPhysicsBody3> actor0, const Transform3<real_Num> &localFrame0,
                SmartPtr<IPhysicsBody3> actor1, const Transform3<real_Num> &localFrame1 ) override;

            SmartPtr<IConstraintDrive> createConstraintDrive() override;

            SmartPtr<IConstraintLinearLimit> createConstraintLinearLimit(real_Num extent, real_Num contactDist = real_Num(-1.0)) override;

            SmartPtr<IRaycastHit> createRaycastHitData() override;

            Thread::Task getStateTask() const override;

            Thread::Task getPhysicsTask() const override;

            void loadObject( SmartPtr<ISharedObject> object, bool forceQueue = false ) override;

            void unloadObject( SmartPtr<ISharedObject> object, bool forceQueue = false ) override;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FB_CPHYSICSMANAGER_H
