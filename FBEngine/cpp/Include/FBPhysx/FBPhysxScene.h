#ifndef FBPhysxWorld_h__
#define FBPhysxWorld_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <PxSimpleTypes.h>
#include <PxQueryReport.h>
#include <PxFiltering.h>
#include <PxSimulationEventCallback.h>
#include <PxContactModifyCallback.h>

namespace fb
{
    namespace physics
    {

        class PhysxScene : public IPhysicsScene3
        {
        public:
            PhysxScene();
            ~PhysxScene() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::preUpdate */
            void preUpdate() override;

            /** @copydoc ISharedObject::update */
            void update() override;

            /** @copydoc ISharedObject::postUpdate */
            void postUpdate() override;

            void clear() override;

            void setSize( const Vector3F &size ) override;
            Vector3F getSize() const override;

            bool rayTest( const Vector3F &start, const Vector3F &direction, Vector3F &hitPos,
                          Vector3F &hitNormal, u32 collisionType, u32 collisionMask ) override;
            bool intersects( const Vector3F &start, const Vector3F &end, Vector3F &hitPos,
                             Vector3F &hitNormal, SmartPtr<ISharedObject> &object, u32 collisionType,
                             u32 collisionMask ) override;

            physx::PxScene *getScene() const;
            void setScene( physx::PxScene *scene );

            void setGravity( const Vector3<real_Num> &vec ) override;
            Vector3<real_Num> getGravity() const override;

            void simulate( real_Num elapsedTime, void *scratchMemBlock, u32 scratchMemBlockSize,
                           bool controlSimulation ) override;

            bool fetchResults( bool block, u32 *errorState ) override;

            void addActor( SmartPtr<IPhysicsBody3> body ) override;
            void removeActor( SmartPtr<IPhysicsBody3> body ) override;

            bool castRay( const Vector3<real_Num> &origin, const Vector3<real_Num> &dir,
                          Array<SmartPtr<IRaycastHit>> &hits ) override;
            bool castRay( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit ) override;
            bool castRayDynamic( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit ) override;

            /** @copydoc IPhysicsScene3::getStateObject */
            SmartPtr<IStateContext> getStateObject() const override;

            /** @copydoc IPhysicsScene3::setStateObject */
            void setStateObject( SmartPtr<IStateContext> stateObject ) override;

        protected:
            static physx::PxFilterFlags simulationFilterShader(
                physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
                physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
                physx::PxPairFlags &pairFlags, const void *constantBlock,
                physx::PxU32 constantBlockSize );

            class CollisionCallback : public physx::PxSimulationFilterCallback
            {
            public:
                CollisionCallback();
                ~CollisionCallback() override;

                // physx::PxFilterFlags pairFound( u32 pairID, physx::PxFilterObjectAttributes
                // attributes0, 	physx::PxFilterData filterData0, physx::PxActor* a0, physx::PxShape*
                // s0,
                // physx::PxFilterObjectAttributes attributes1, 	physx::PxFilterData filterData1,
                // physx::PxActor* a1, physx::PxShape* s1, physx::PxPairFlags& pairFlags );

                physx::PxFilterFlags pairFound( u32 pairID, physx::PxFilterObjectAttributes attributes0,
                                                physx::PxFilterData filterData0,
                                                const physx::PxActor *a0, const physx::PxShape *s0,
                                                physx::PxFilterObjectAttributes attributes1,
                                                physx::PxFilterData filterData1,
                                                const physx::PxActor *a1, const physx::PxShape *s1,
                                                physx::PxPairFlags &pairFlags ) override;

                bool statusChange( u32 &pairID, physx::PxPairFlags &pairFlags,
                                   physx::PxFilterFlags &filterFlags ) override;

                void pairLost( u32 pairID, physx::PxFilterObjectAttributes attributes0,
                               physx::PxFilterData filterData0,
                               physx::PxFilterObjectAttributes attributes1,
                               physx::PxFilterData filterData1, bool objectDeleted ) override;
            };

            class SimulationEventCallback : public physx::PxSimulationEventCallback
            {
            public:
                SimulationEventCallback();
                ~SimulationEventCallback() override;

                void onConstraintBreak( physx::PxConstraintInfo *constraints, u32 count ) override;

                void onSleep( physx::PxActor **actors, u32 count ) override;
                void onWake( physx::PxActor **actors, u32 count ) override;
                void onContact( const physx::PxContactPairHeader &pairHeader,
                                const physx::PxContactPair *pairs, u32 nbPairs ) override;
                void checkBreakage( physx::PxRigidDynamic *rb0, physx::PxShape *shape );
                void onTrigger( physx::PxTriggerPair *pairs, u32 count ) override;
                // void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const Transform3<real_Num>*
                // poseBuffer, const u32 count) override;
            };

            class ContactModificationCallback : public physx::PxContactModifyCallback
            {
            public:
                void onContactModify( physx::PxContactModifyPair *pairs, u32 count ) override;
            };

            class RaycastCallback : public physx::PxRaycastCallback
            {
            public:
                RaycastCallback();

                physx::PxAgain processTouches( const physx::PxRaycastHit *buffer, u32 nbHits ) override;

                physx::PxRaycastHit buffer[10];
                physx::PxRaycastHit m_hit;
                f32 m_closestHit = 0.0f;
                bool m_checkStatic = true;
                bool m_checkDynamic = true;
            };

            SharedPtr<Array<SmartPtr<IRigidBody3>>> getRigidBodiesPtr() const;

            void setRigidBodiesPtr( SharedPtr<Array<SmartPtr<IRigidBody3>>> ptr );
            void addRigidBodiesPtr( SmartPtr<IRigidBody3> body );

            void removeRigidBodiesPtr( SmartPtr<IRigidBody3> body );

            RawPtr<physx::PxDefaultCpuDispatcher> m_cpuDispatcher;

            /// The state object.
            SmartPtr<IStateContext> m_stateObject;

            /// The state listener.
            SmartPtr<IStateListener> m_stateListener;

            Vector3<real_Num> m_size;

            mutable std::mutex m_rigidBodiesMutex;
            SharedPtr<Array<SmartPtr<IRigidBody3>>> m_rigidBodies;

            AtomicRawPtr<physx::PxScene> m_scene;

            RawPtr<CollisionCallback> m_collisionCallback;
            RawPtr<SimulationEventCallback> m_simulationEventCallback;
            RawPtr<ContactModificationCallback> m_contactModificationCallback;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxWorld_h__
