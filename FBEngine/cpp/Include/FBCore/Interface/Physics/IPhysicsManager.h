#ifndef __IPhysicsManager3__H
#define __IPhysicsManager3__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace physics
    {

        /** Interface for a 3d physics engine. */
        class IPhysicsManager : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IPhysicsManager() override = default;

            /** Gets whether the debug draw is enabled.
            @return A boolean indicating whether the debug draw is enabled.
            */
            virtual bool getEnableDebugDraw() const = 0;

            /** Sets whether the debug draw is enabled.
            @param enableDebugDraw A boolean indicating whether the debug draw is enabled.
            */
            virtual void setEnableDebugDraw( bool enableDebugDraw ) = 0;

            /** Creates an instance of a physics material.
            @return An instance of a physics material.
            */
            virtual SmartPtr<IPhysicsMaterial3> createMaterial() = 0;

            /* Destroy a material.
            @param material A pointer to the material destroy.
            */
            virtual void destroyMaterial( SmartPtr<IPhysicsMaterial3> material ) = 0;

            /* Create a scene with default properties. */
            virtual SmartPtr<IPhysicsScene3> createScene() = 0;

            /* Destroy a scene. */
            virtual void destroyScene( SmartPtr<IPhysicsScene3> scene ) = 0;

            /** Creates a collision from the type provided.
            @remarks Creates a collision shape from the type specified with default properties.
            @param type The of collision to be created.
            @param data The data used to create the shape.
            @return An instance of a collision shape.
            */
            virtual SmartPtr<IPhysicsShape3> createCollisionShapeByType(
                hash64 type, SmartPtr<ISharedObject> data ) = 0;

            /** Creates a collision shape from the template type provided.
            @param data The data used to create the shape.
            @return An instance of a collision shape.
            */
            template <class T>
            SmartPtr<T> createCollisionShape( SmartPtr<ISharedObject> data );

            /** Destroys a collision shape. */
            virtual bool destroyCollisionShape( SmartPtr<IPhysicsShape3> collisionShape ) = 0;

            /** Destroys a physics body. */
            virtual bool destroyPhysicsBody( SmartPtr<IRigidBody3> body ) = 0;

            /** Adds a character controller. */
            virtual SmartPtr<ICharacterController3> addCharacter() = 0;

            /** Creates a rigid body.
            @return A rigid body instance.
            */
            virtual SmartPtr<IRigidStatic3> createRigidStatic(
                const Transform3<real_Num> &transform ) = 0;

            /** Creates a rigid body.
            @return A rigid body instance.
            */
            virtual SmartPtr<IRigidDynamic3> createRigidDynamic(
                const Transform3<real_Num> &transform ) = 0;

            /** Creates a rigid body. */
            virtual SmartPtr<IRigidStatic3> createRigidStatic(
                SmartPtr<IPhysicsShape3> collisionShape ) = 0;

            /** Creates a rigid body. */
            virtual SmartPtr<IRigidStatic3> createRigidStatic( SmartPtr<IPhysicsShape3> collisionShape,
                                                               SmartPtr<Properties> properties ) = 0;

            /** Creates a soft body. */
            virtual SmartPtr<IPhysicsSoftBody3> createSoftBody( const String &filePath ) = 0;

            /** Adds a vehicle. */
            virtual SmartPtr<IPhysicsVehicle3> addVehicle( SmartPtr<IRigidBody3> chassis,
                                                           const SmartPtr<Properties> &properties ) = 0;

            /** Adds a vehicle. */
            virtual SmartPtr<IPhysicsVehicle3> addVehicle(
                SmartPtr<scene::IDirector> vehicleTemplate ) = 0;

            /** Destroys a vehicle. */
            virtual bool destroyVehicle( SmartPtr<IPhysicsVehicle3> vehicle ) = 0;

            /** Performs a ray intersection test. */
            virtual bool rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &direction,
                                  Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                  u32 collisionType = 0, u32 collisionMask = 0 ) = 0;

            /** Performs a line intersection test.
            @param start The start point of the line.
            @param end The end point of the line.
            @param hitPos The hit position of the intersection.
            @param normal The surface normal of the intersection.
            @param object The object that intersects.
            @param collisionType The collision type.
            @param collisionMask The collision mask.
            */
            virtual bool intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                                     Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                     SmartPtr<ISharedObject> &object, u32 collisionType = 0,
                                     u32 collisionMask = 0 ) = 0;

            /** Creates a 6 degree of freedom joint. */
            virtual SmartPtr<IConstraintD6> d6JointCreate( SmartPtr<IPhysicsBody3> actor0,
                                                           const Transform3<real_Num> &localFrame0,
                                                           SmartPtr<IPhysicsBody3> actor1,
                                                           const Transform3<real_Num> &localFrame1 ) = 0;

            /** Creates a fixed joint. */
            virtual SmartPtr<IConstraintFixed3> fixedJointCreate(
                SmartPtr<IPhysicsBody3> actor0, const Transform3<real_Num> &localFrame0,
                SmartPtr<IPhysicsBody3> actor1, const Transform3<real_Num> &localFrame1 ) = 0;

            /** Creates a constraint drive. */
            virtual SmartPtr<IConstraintDrive> createConstraintDrive() = 0;

            /** Creates a constraint linear limit. */
            virtual SmartPtr<IConstraintLinearLimit> createConstraintLinearLimit(real_Num extent, real_Num contactDist = real_Num(-1.0)) = 0;

            /** Creates raycast hit data. */
            virtual SmartPtr<IRaycastHit> createRaycastHitData() = 0;

            /** The task used to update. */
            virtual Thread::Task getStateTask() const = 0;

            /** The task the physics is updated on. */
            virtual Thread::Task getPhysicsTask() const = 0;

            /** Loads a physics object.
            @param graphicsObject The object to be loaded.
            @param forceQueue Forces the object to be queued for deferred loading.
            */
            virtual void loadObject( SmartPtr<ISharedObject> object, bool forceQueue = false ) = 0;

            /** Unloads a physics object.
            @param object The object to be unloaded.
            @param forceQueue Forces the object to be queued for deferred unloading.
            */
            virtual void unloadObject( SmartPtr<ISharedObject> object, bool forceQueue = false ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        SmartPtr<T> IPhysicsManager::createCollisionShape( SmartPtr<ISharedObject> data )
        {
            auto typeInfo = T::typeInfo();
            FB_ASSERT( typeInfo != 0 );

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            auto typeHash = typeManager->getHash( typeInfo );
            FB_ASSERT( typeHash != 0 );

            auto shape = createCollisionShapeByType( typeHash, data );
            FB_ASSERT( fb::dynamic_pointer_cast<T>( shape ) );
            return fb::static_pointer_cast<T>( shape );
        }

    }  // end namespace physics
}  // end namespace fb

#endif
