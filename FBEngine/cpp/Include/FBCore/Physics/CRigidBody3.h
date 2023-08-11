#ifndef FBCRigidBody3_h__
#define FBCRigidBody3_h__

#include <FBCore/Physics/CPhysicsBody3.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace physics
    {
        template <class T>
        class CRigidBody3 : public CPhysicsBody3<T>
        {
        public:
            CRigidBody3()
            {
            }

            ~CRigidBody3() override
            {
                unload( nullptr );
            }

            void unload( SmartPtr<ISharedObject> data )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                if( auto stateObject = getStateObject() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateObject->removeStateListener( stateListener );
                    }

                    stateManager->removeStateObject( stateObject );

                    stateObject->unload( nullptr );
                    setStateObject( nullptr );
                }

                if( auto stateListener = getStateListener() )
                {
                    stateListener->unload( nullptr );
                    setStateListener( nullptr );
                }
            }

            void setRigidBodyFlag( RigidBodyFlag::Enum flag, bool value ) override
            {
            }

            RigidBodyFlag::Enum getRigidBodyFlags() const override
            {
                return (RigidBodyFlag::Enum)0;
            }

            void addShape( SmartPtr<IPhysicsShape3> shape ) override
            {
            }

            void removeShape( SmartPtr<IPhysicsShape3> shape, bool wakeOnLostTouch = true ) override
            {
            }

            Array<SmartPtr<IPhysicsShape3>> getShapes() const override
            {
                return Array<SmartPtr<IPhysicsShape3>>();
            }

            u32 getNumShapes() const override
            {
                return 0;
            }

            void setLinearVelocity( const Vector3<real_Num> &linVel, bool autowake = true ) override
            {
            }

            Vector3<real_Num> getLinearVelocity() const override
            {
                return Vector3<real_Num>::zero();
            }

            void setAngularVelocity( const Vector3<real_Num> &angVel, bool autowake = true ) override
            {
            }

            Vector3<real_Num> getAngularVelocity() const override
            {
                return Vector3<real_Num>::zero();
            }

            void addForce( const Vector3<real_Num> &force ) override
            {
            }

            void clearForce( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override
            {
            }

            void addTorque( const Vector3<real_Num> &torque ) override
            {
            }

            void clearTorque( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override
            {
            }

            AABB3<real_Num> getLocalAABB() const override
            {
                return AABB3<real_Num>();
            }

            AABB3<real_Num> getWorldAABB() const override
            {
                return AABB3<real_Num>();
            }

            void setCMassLocalPose( const Transform3<real_Num> &pose ) override
            {
            }

            Transform3<real_Num> getCMassLocalPose() const override
            {
                return Transform3<real_Num>();
            }

            void setMassSpaceInertiaTensor( const Vector3<real_Num> &m ) override
            {
            }

            Vector3<real_Num> getMassSpaceInertiaTensor() const override
            {
                return Vector3<real_Num>::zero();
            }

            Vector3<real_Num> getMassSpaceInvInertiaTensor() const override
            {
                return Vector3<real_Num>::zero();
            }

            SmartPtr<IStateContext> getStateObject() const override;

            void setStateObject( SmartPtr<IStateContext> stateObject ) override;

            SmartPtr<IStateListener> getStateListener() const;

            void setStateListener( SmartPtr<IStateListener> stateListener );

            FB_CLASS_REGISTER_TEMPLATE_DECL( CRigidBody3, T );

        protected:
            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CRigidBody3, T, SharedObject<T> );

        template <class T>
        SmartPtr<IStateContext> CRigidBody3<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <class T>
        void CRigidBody3<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <class T>
        SmartPtr<IStateListener> CRigidBody3<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <class T>
        void CRigidBody3<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

    }  // namespace physics
}  // namespace fb

#endif  // FBCRigidBody3_h__
