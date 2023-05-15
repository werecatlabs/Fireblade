#ifndef FBPhysxRigidBody_h__
#define FBPhysxRigidBody_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IRigidDynamic3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBPhysx/FBPhysxRigidBody3.h>

namespace fb
{
    namespace physics
    {
        
        class PhysxRigidDynamic : public PhysxRigidBody3<IRigidDynamic3>
        {
        public:
            PhysxRigidDynamic();
            ~PhysxRigidDynamic() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IRigidDynamic3::getShapes */
            Array<SmartPtr<IPhysicsShape3>> getShapes() const override;

            /** @copydoc IRigidDynamic3::setTransform */
            void setTransform( const Transform3<real_Num> &transform ) override;

            /** @copydoc IRigidDynamic3::getTransform */
            Transform3<real_Num> getTransform() const override;

            /** @copydoc IRigidDynamic3::addVelocity */
            void addVelocity( const Vector3<real_Num> &velocity, const Vector3<real_Num> &relPos );

            /** @copydoc IRigidDynamic3::setVelocity */
            void setVelocity( const Vector3<real_Num> &velocity );

            /** @copydoc IRigidDynamic3::setVelocity */
            Vector3<real_Num> getVelocity() const;

            /** @copydoc IRigidDynamic3::setVelocity */
            Vector3<real_Num> getAngularVelocity() const override;

            /** @copydoc IRigidDynamic3::setVelocity */
            void addForce( const Vector3<real_Num> &force ) override;

            /** @copydoc IRigidDynamic3::setVelocity */
            void setForce( const Vector3<real_Num> &force );

            /** @copydoc IRigidDynamic3::setVelocity */
            Vector3<real_Num> getForce() const;

            /** @copydoc IRigidDynamic3::setVelocity */
            void addTorque( const Vector3<real_Num> &torque ) override;

            /** @copydoc IRigidDynamic3::setVelocity */
            void setTorque( const Vector3<real_Num> &torque );

            /** @copydoc IRigidDynamic3::setVelocity */
            Vector3<real_Num> getTorque() const;

            void setMaterialId( u32 materialId );
            u32 getMaterialId() const;

            AABB3<real_Num> getLocalAABB() const override;
            AABB3<real_Num> getWorldAABB() const override;

            real_Num getMass() const override;
            void setMass( real_Num mass ) override;

            void setCollisionType( u32 type ) override;
            u32 getCollisionType() const override;

            void setCollisionMask( u32 mask ) override;
            u32 getCollisionMask() const override;

            void setEnabled( bool enabled ) override;
            bool isEnabled() const override;

            void *getUserData() const override;
            void setUserData( void *userData ) override;

            physx::PxRigidDynamic *getActorDynamic() const;
            void setActorDynamic( physx::PxRigidDynamic *val );

            bool getKinematicMode() const override;
            void setKinematicMode( bool val ) override;

            void *getUserDataById( u32 id ) const override;
            void setUserDataById( u32 id, void *userData ) override;

            void setRigidBodyFlag( PxRigidBodyFlag::Enum flag, bool value ) override;
            PxRigidBodyFlag::Enum getRigidBodyFlags() const override;

            void setLinearVelocity( const Vector3<real_Num> &linVel, bool autowake = true ) override;
            Vector3<real_Num> getLinearVelocity() const override;

            void setAngularVelocity( const Vector3<real_Num> &angVel, bool autowake = true ) override;
            void clearForce( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override;

            void clearTorque( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override;
            void setCMassLocalPose( const Transform3<real_Num> &pose ) override;

            Transform3<real_Num> getCMassLocalPose() const override;
            void setMassSpaceInertiaTensor( const Vector3<real_Num> &m ) override;

            Vector3<real_Num> getMassSpaceInertiaTensor() const override;
            Vector3<real_Num> getMassSpaceInvInertiaTensor() const override;

            void setActorFlag( PxActorFlag::Enum flag, bool value ) override;
            PxActorFlag::Enum getActorFlags() const override;
            
            void setKinematicTarget( const Transform3<real_Num> &destination ) override;
            bool getKinematicTarget( Transform3<real_Num> &target ) override;

            bool isKinematic() const override;
            void setKinematic( bool kinematic ) override;

            void setLinearDamping( real_Num damping ) override;
            real_Num getLinearDamping() const override;

            void setAngularDamping( real_Num angDamp ) override;
            real_Num getAngularDamping() const override;

            void setMaxAngularVelocity( real_Num maxAngVel ) override;
            real_Num getMaxAngularVelocity() const override;

            bool isSleeping() const override;

            void setSleepThreshold( real_Num threshold ) override;
            real_Num getSleepThreshold() const override;

            void setStabilizationThreshold( real_Num threshold ) override;
            real_Num getStabilizationThreshold() const override;

            void setWakeCounter( real_Num wakeCounterValue ) override;

            real_Num getWakeCounter() const override;

            void wakeUp() override;
            void putToSleep() override;

            void setSolverIterationCounts( u32 minPositionIters, u32 minVelocityIters = 1 ) override;
            void getSolverIterationCounts( u32 &minPositionIters, u32 &minVelocityIters ) const override;

            real_Num getContactReportThreshold() const override;
            void setContactReportThreshold( real_Num threshold ) override;

            u32 getNumShapes() const override;

            SmartPtr<IPhysicsBody3> clone() override;

            void setActiveTransform( const Transform3<real_Num> &transform );

            /** @copydoc ISharedObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IPhysicsBody3::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IPhysicsBody3::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<IStateContext> getStateObject() const override;

            void setStateObject( SmartPtr<IStateContext> stateObject ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class StateListener : public CSharedObject<IStateListener>
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                PhysxRigidDynamic *getOwner() const;
                void setOwner( PhysxRigidDynamic *owner );

            protected:
                RawPtr<PhysxRigidDynamic> m_owner = nullptr;
            };
            
            AABB3<real_Num> m_localAABB;
            AABB3<real_Num> m_worldAABB;

            Vector3<real_Num> m_force;

            Vector3<real_Num> m_torque;

            void *m_userData = nullptr;

            u32 m_collisionType = 0;
            u32 m_collisionMask = 0;

            bool m_enabled = true;
            bool m_kinematicMode = false;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxRigidBody_h__
