#ifndef __FBPhysxManager_h__
#define __FBPhysxManager_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Physics/CPhysicsManager.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentQueue.h>

#define MAX_NUM_INDEX_BUFFERS 16
#define NUM_PLAYER_CARS 1
#define NUM_NONPLAYER_4W_VEHICLES 0

namespace fb
{
    namespace physics
    {

        /** Implementation to wrap physx functionality.
        @author	Zane Desir
        @version 1.0
        */
        class PhysxManager : public CPhysicsManager
        {
        public:
            PhysxManager();
            ~PhysxManager() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::preUpdate */
            void preUpdate() override;

            /** @copydoc ISharedObject::update */
            void update() override;

            /** @copydoc ISharedObject::postUpdate */
            void postUpdate() override;

            /** @copydoc IObject::createScene */
            SmartPtr<IPhysicsScene3> addScene() override;

            /** @copydoc IObject::destroyScene */
            void removeScene( SmartPtr<IPhysicsScene3> scene ) override;

            /** @copydoc IPhysicsManager3::getEnableDebugDraw */
            bool getEnableDebugDraw() const override;

            /** @copydoc IPhysicsManager3::setEnableDebugDraw */
            void setEnableDebugDraw( bool enableDebugDraw ) override;

            /** @copydoc IPhysicsManager3::createMaterial */
            SmartPtr<IPhysicsMaterial3> addMaterial() override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            void removeMaterial( SmartPtr<IPhysicsMaterial3> material ) override;

            /** @copydoc IPhysicsManager3::createCollisionShapeByType */
            SmartPtr<IPhysicsShape3> addCollisionShapeByType( hash64 type,
                                                                 SmartPtr<ISharedObject> data );

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<IRigidStatic3> addRigidStatic( const Transform3<real_Num> &transform ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<IRigidDynamic3> addRigidDynamic(
                const Transform3<real_Num> &transform ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<IRigidStatic3> addRigidStatic(
                SmartPtr<IPhysicsShape3> collisionShape ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<IRigidStatic3> addRigidStatic( SmartPtr<IPhysicsShape3> collisionShape,
                                                       SmartPtr<Properties> properties ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<IPhysicsSoftBody3> addSoftBody( const String &filePath ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<IPhysicsVehicle3> addVehicle( SmartPtr<IRigidBody3> chassis,
                                                   const SmartPtr<Properties> &properties ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<IPhysicsVehicle3> addVehicle( SmartPtr<scene::IDirector> vehicleTemplate );

            /** @copydoc IPhysicsManager3::destroyMaterial */
            bool removeCollisionShape( SmartPtr<IPhysicsShape3> collisionShape ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            bool removePhysicsBody( SmartPtr<IRigidBody3> body ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            bool removeVehicle( SmartPtr<IPhysicsVehicle3> vehicle ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            SmartPtr<ICharacterController3> addCharacter() override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            bool rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &direction,
                          Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal, u32 collisionType = 0,
                          u32 collisionMask = 0 ) override;

            /** @copydoc IPhysicsManager3::destroyMaterial */
            bool intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                             Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                             SmartPtr<ISharedObject> &object, u32 collisionType = 0,
                             u32 collisionMask = 0 ) override;

            /** @copydoc IPhysicsManager3::d6JointCreate */
            SmartPtr<IConstraintD6> addJointD6( SmartPtr<IPhysicsBody3> actor0,
                                                   const Transform3<real_Num> &localFrame0,
                                                   SmartPtr<IPhysicsBody3> actor1,
                                                   const Transform3<real_Num> &localFrame1 ) override;

            /** @copydoc IPhysicsManager3::fixedJointCreate */
            SmartPtr<IConstraintFixed3> addFixedJoint(
                SmartPtr<IPhysicsBody3> actor0, const Transform3<real_Num> &localFrame0,
                SmartPtr<IPhysicsBody3> actor1, const Transform3<real_Num> &localFrame1 ) override;

            /** @copydoc IPhysicsManager3::createConstraintDrive */
            SmartPtr<IConstraintDrive> addConstraintDrive() override;

            /** @copydoc IPhysicsManager3::createConstraintLinearLimit */
            SmartPtr<IConstraintLinearLimit> addConstraintLinearLimit(real_Num extent, real_Num contactDist = real_Num(-1.0)) override;

            /** @copydoc IPhysicsManager3::createRaycastHitData */
            SmartPtr<IRaycastHit> addRaycastHitData() override;

            /** @copydoc IPhysicsManager3::getStateTask */
            Thread::Task getStateTask() const override;

            /** @copydoc IPhysicsManager3::getPhysicsTask */
            Thread::Task getPhysicsTask() const override;

            /** @copydoc IPhysicsManager3::loadObject */
            void loadObject( SmartPtr<ISharedObject> object, bool forceQueue = false ) override;

            /** @copydoc IPhysicsManager3::unloadObject */
            void unloadObject( SmartPtr<ISharedObject> object, bool forceQueue = false ) override;

            physx::PxPhysics *getPhysics() const;
            void setPhysics( physx::PxPhysics *physics );

            physx::PxCooking *getCooking() const;
            void setCooking( physx::PxCooking *cooking );

            physx::PxMaterial *getDefaultMaterial() const;
            void setDefaultMaterial( physx::PxMaterial *defaultMaterial );

            PhysxVehicleManager *getVehicleManager() const;
            void setVehicleManager( PhysxVehicleManager *vehicleManager );

            PhysxCooker *getCooker() const;
            void setCooker( PhysxCooker *val );

            physx::PxMaterial *getStandardMaterials() const;

            /** Locks the object for thread safety. */
            virtual void lock();

            /** Unlocks the object for thread safety. */
            virtual void unlock();

            FB_CLASS_REGISTER_DECL;

        protected:
            void initialise( const SmartPtr<Properties> &properties );
            void initialise( SmartPtr<scene::IDirector> objectTemplate );

            SmartPtr<ISphereShape3> createSphere();

            SmartPtr<IBoxShape3> createBox();
            SmartPtr<IPlaneShape3> createPlane();
            SmartPtr<IMeshShape> createMesh();
            SmartPtr<ITerrainShape> createTerrain();

            SmartPtr<ITerrainShape> createTerrain( SmartPtr<scene::IDirector> objectTemplate );

            SmartPtr<IRigidBody3> createRigidBody();
            SmartPtr<IRigidBody3> createRigidBody( SmartPtr<IPhysicsShape3> collisionShape );
            SmartPtr<IRigidBody3> createRigidBody( SmartPtr<IPhysicsShape3> collisionShape,
                                                   SmartPtr<Properties> properties );

            SmartPtr<IRigidBody3> createRigidBody( const Transform3<real_Num> &transform );

            // static physx::PxFilterFlags simulationFilterShader(physx::PxFilterObjectAttributes
            // attributes0, physx::PxFilterData filterData0, 	physx::PxFilterObjectAttributes
            // attributes1,
            // physx::PxFilterData filterData1, 	physx::PxPairFlags& pairFlags, const void*
            // constantBlock, u32 constantBlockSize);

            // static physx::PxFilterFlags SampleParticlesFilterShader(physx::PxFilterObjectAttributes
            // attributes0, physx::PxFilterData filterData0, 	physx::PxFilterObjectAttributes
            // attributes1,
            // physx::PxFilterData filterData1, 	physx::PxPairFlags& pairFlags, const void*
            // constantBlock, u32 constantBlockSize);

            // static physx::PxFilterFlags checkModelShape(ShapeData* shape0, physx::PxActor* a0);

            SharedPtr<physx::PxAllocatorCallback> m_allocator;
            SharedPtr<physx::PxErrorCallback> m_errorOutput;

            RawPtr<physx::PxFoundation> m_foundation;

            AtomicRawPtr<physx::PxPhysics> m_physics;

            RawPtr<physx::PxCooking> m_cooking;
            RawPtr<physx::PxMaterial> m_defaultMaterial;
            RawPtr<PhysxVehicleManager> m_vehicleManager;
            RawPtr<physx::PxDefaultCpuDispatcher> m_cpuDispatcher;

            RawPtr<PhysxCooker> m_cooker;

            // Materials
            // physx::PxVehicleDrivableSurfaceType	mVehicleDrivableSurfaceTypes[MAX_NUM_INDEX_BUFFERS];
            RawPtr<physx::PxMaterial> mStandardMaterials[MAX_NUM_INDEX_BUFFERS];

            ConcurrentQueue<SmartPtr<ISharedObject>> m_loadQueue;

            Array<SmartPtr<ISphereShape3>> m_sphereShapes;
            Array<SmartPtr<IBoxShape3>> m_boxShapes;
            Array<SmartPtr<IMeshShape>> m_meshShapes;
            Array<SmartPtr<ITerrainShape>> m_terrainShapes;
            Array<SmartPtr<IPlaneShape3>> m_planeShapes;

            Array<SmartPtr<IPhysicsConstraint3>> m_constraints;
            Array<SmartPtr<PhysxRigidDynamic>> m_rigidBodies;
            Array<SmartPtr<PhysxRigidStatic>> m_staticBodies;
            Array<SmartPtr<PhysxCharacterController>> m_characters;
            Array<SmartPtr<IPhysicsVehicle3>> m_vehicles;

            Array<SmartPtr<PhysxScene>> m_scenes;

            bool m_enableDebugDraw = false;

            mutable RecursiveMutex PhysxMutex;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxManager_h__
