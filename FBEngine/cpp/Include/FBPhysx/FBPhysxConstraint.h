#ifndef FBPhysxConstraint_h__
#define FBPhysxConstraint_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBPhysx/FBPhysxSharedObject.h>
#include <FBCore/Interface/Physics/IPhysicsConstraint3.h>
#include <FBCore/Interface/Physics/IPhysicsBody3.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace physics
    {
        template <class T>
        class PhysxConstraint : public PhysxSharedObject<T>
        {
        public:
            PhysxConstraint()
            {
            }

            ~PhysxConstraint() override
            {
            }

            virtual bool isDirty() const
            {
                return m_isDirty;
            }

            virtual void setDirty( bool bIsDirty )
            {
                m_isDirty = bIsDirty;
            }

            virtual void updateDirty()
            {
            }

            virtual SmartPtr<IPhysicsBody3> getBodyA() const
            {
                return m_bodyA;
            }

            virtual void setBodyA( SmartPtr<IPhysicsBody3> val )
            {
                m_bodyA = val;
                setDirty( true );
            }

            virtual SmartPtr<IPhysicsBody3> getBodyB() const
            {
                return m_bodyB;
            }

            virtual void setBodyB( SmartPtr<IPhysicsBody3> val )
            {
                m_bodyB = val;
                setDirty( true );
            }

            virtual void setLocalPose( PxJointActorIndex::Enum actor,
                                       const Transform3<real_Num> &localPose )
            {
                m_poses[actor] = localPose;
                setDirty( true );
            }

            virtual Transform3<real_Num> getLocalPose( PxJointActorIndex::Enum actor ) const
            {
                return m_poses[actor];
            }

            virtual void setConstraintFlag( PxConstraintFlag::Enum flag, bool value )
            {
                if(value)
                    m_flags |= flag;
                else
                    m_flags &= ~flag;

                setDirty( true );
            }

            virtual PxConstraintFlag::Enum getConstraintFlags() const
            {
                return static_cast<PxConstraintFlag::Enum>(m_flags);
            }

            virtual void setBreakForce( real_Num force, real_Num torque )
            {
                m_force = force;
                m_torque = torque;
                setDirty( true );
            }

            virtual void getBreakForce( real_Num &force, real_Num &torque ) const
            {
                force = m_force;
                torque = m_torque;
            }

            virtual void setProjectionLinearTolerance( real_Num tolerance )
            {
                m_linearTolerance = tolerance;
                setDirty( true );
            }

            virtual real_Num getProjectionLinearTolerance() const
            {
                return m_linearTolerance;
            }

            virtual void setProjectionAngularTolerance( real_Num tolerance )
            {
                if(m_angularTolerance != tolerance)
                {
                    m_angularTolerance = tolerance;
                    setDirty( true );
                }
            }

            virtual real_Num getProjectionAngularTolerance() const
            {
                return m_angularTolerance;
            }

            void *getUserData() const override
            {
                return m_userData;
            }

            void setUserData( void *val ) override
            {
                m_userData = val;
            }

        protected:
            SmartPtr<IPhysicsBody3> m_bodyA;
            SmartPtr<IPhysicsBody3> m_bodyB;
            Transform3<real_Num> m_poses[PxJointActorIndex::Enum::COUNT];
            real_Num m_force = static_cast<real_Num>(0.0);
            real_Num m_torque = static_cast<real_Num>(0.0);

            real_Num m_linearTolerance = 0.0;
            real_Num m_angularTolerance = 0.0;

            u32 m_flags = 0;
            atomic_bool m_isDirty = false;
            void *m_userData = nullptr;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxConstraint_h__
