#ifndef FBPhysxRigidStatic3_h__
#define FBPhysxRigidStatic3_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/IRigidStatic3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBPhysx/FBPhysxRigidBody3.h>

namespace fb
{
    namespace physics
    {
        
        class PhysxRigidStatic : public PhysxRigidBody3<IRigidStatic3>
        {
        public:
            PhysxRigidStatic();
            ~PhysxRigidStatic() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IRigidStatic3::setTransform */
            void setTransform( const Transform3<real_Num> &transform ) override;

            /** @copydoc IRigidStatic3::getTransform */
            Transform3<real_Num> getTransform() const override;

            /** @copydoc IRigidStatic3::getMass */
            real_Num getMass() const override;

            /** @copydoc IRigidStatic3::setMass */
            void setMass( real_Num mass ) override;

            /** @copydoc IRigidStatic3::setCollisionType */
            void setCollisionType( u32 type ) override;

            /** @copydoc IRigidStatic3::getCollisionType */
            u32 getCollisionType() const override;

            /** @copydoc IRigidStatic3::setCollisionMask */
            void setCollisionMask( u32 mask ) override;

            /** @copydoc IRigidStatic3::getCollisionMask */
            u32 getCollisionMask() const override;

            /** @copydoc IRigidStatic3::setEnabled */
            void setEnabled( bool enabled ) override;

            /** @copydoc IRigidStatic3::isEnabled */
            bool isEnabled() const override;

            /** @copydoc IRigidStatic3::getKinematicMode */
            bool getKinematicMode() const override;

            /** @copydoc IRigidStatic3::setKinematicMode */
            void setKinematicMode( bool val ) override;

            /** @copydoc IRigidStatic3::getUserData */
            void *getUserData() const override;

            /** @copydoc IRigidStatic3::setUserData */
            void setUserData( void *userData ) override;

            /** @copydoc IRigidStatic3::setUserData */
            void *getUserDataById( u32 id ) const override;

            /** @copydoc IRigidStatic3::setUserData */
            void setUserDataById( u32 id, void *userData ) override;

            /** @copydoc IRigidStatic3::setScene */
            void setScene( SmartPtr<IPhysicsScene3> scene ) override;

            /** @copydoc IRigidStatic3::getScene */
            SmartPtr<IPhysicsScene3> getScene() const override;

            /** @copydoc IRigidStatic3::setActorFlag */
            void setActorFlag( PxActorFlag::Enum flag, bool value ) override;

            /** @copydoc IRigidStatic3::getActorFlags */
            PxActorFlag::Enum getActorFlags() const override;
            
            /** @copydoc IRigidStatic3::setRigidBodyFlag */
            void setRigidBodyFlag( PxRigidBodyFlag::Enum flag, bool value ) override;

            /** @copydoc IRigidStatic3::getRigidBodyFlags */
            PxRigidBodyFlag::Enum getRigidBodyFlags() const override;

            /** @copydoc IRigidStatic3::getShapes */
            Array<SmartPtr<IPhysicsShape3>> getShapes() const override;

            /** @copydoc IRigidStatic3::getNumShapes */
            u32 getNumShapes() const override;

            /** @copydoc IRigidStatic3::setLinearVelocity */
            void setLinearVelocity( const Vector3<real_Num> &linVel, bool autowake = true ) override;

            /** @copydoc IRigidStatic3::getLinearVelocity */
            Vector3<real_Num> getLinearVelocity() const override;

            /** @copydoc IRigidStatic3::setAngularVelocity */
            void setAngularVelocity( const Vector3<real_Num> &angVel, bool autowake = true ) override;

            /** @copydoc IRigidStatic3::getAngularVelocity */
            Vector3<real_Num> getAngularVelocity() const override;

            /** @copydoc IRigidStatic3::addForce */
            void addForce( const Vector3<real_Num> &force ) override;

            /** @copydoc IRigidStatic3::clearForce */
            void clearForce( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override;

            /** @copydoc IRigidStatic3::addTorque */
            void addTorque( const Vector3<real_Num> &torque ) override;

            /** @copydoc IRigidStatic3::clearTorque */
            void clearTorque( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override;

            /** @copydoc IRigidStatic3::getLocalAABB */
            AABB3<real_Num> getLocalAABB() const override;

            /** @copydoc IRigidStatic3::getWorldAABB */
            AABB3<real_Num> getWorldAABB() const override;

            /** @copydoc IRigidStatic3::setCMassLocalPose */
            void setCMassLocalPose( const Transform3<real_Num> &pose ) override;

            /** @copydoc IRigidStatic3::getCMassLocalPose */
            Transform3<real_Num> getCMassLocalPose() const override;

            /** @copydoc IRigidStatic3::setMassSpaceInertiaTensor */
            void setMassSpaceInertiaTensor( const Vector3<real_Num> &m ) override;

            /** @copydoc IRigidStatic3::getMassSpaceInertiaTensor */
            Vector3<real_Num> getMassSpaceInertiaTensor() const override;

            /** @copydoc IRigidStatic3::getMassSpaceInvInertiaTensor */
            Vector3<real_Num> getMassSpaceInvInertiaTensor() const override;

            /** @copydoc IRigidStatic3::clone */
            SmartPtr<IPhysicsBody3> clone() override;

            physx::PxRigidStatic *getRigidStatic() const;
            void setRigidStatic( physx::PxRigidStatic *rigidStatic );

            /** @copydoc ISharedObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IPhysicsBody3::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IPhysicsBody3::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class StateListener : public SharedObject<IStateListener>
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                void handleStateChanged( SmartPtr<IState> &state ) override;

                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                PhysxRigidStatic *getOwner() const;

                void setOwner( PhysxRigidStatic *owner );

            protected:
                RawPtr<PhysxRigidStatic> m_owner = nullptr;
            };

            AABB3<real_Num> m_localAABB;
            AABB3<real_Num> m_worldAABB;

            SmartPtr<IPhysicsScene3> m_scene;
            physx::PxRigidStatic *m_rigidStatic = nullptr;
            Transform3<real_Num> m_pose;

            void *m_userData = nullptr;

            u32 m_collisionType = 0;
            u32 m_collisionMask = 0;
            bool m_kinematicMode = false;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxRigidStatic3_h__
