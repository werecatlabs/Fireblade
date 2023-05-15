#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxConstraintD6.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {
        PhysxConstraintD6::~PhysxConstraintD6()
        {
            unload( nullptr );
        }

        void PhysxConstraintD6::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                if(m_joint)
                {
                    m_joint->release();
                    m_joint = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
            catch(Exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxConstraintD6::update()
        {
            if(isDirty())
            {
                if(m_joint)
                {
                    m_joint->setBreakForce( PX_MAX_F32, PX_MAX_F32 );
                    m_joint->setConstraintFlag( physx::PxConstraintFlag::eVISUALIZATION, true );
                }

                setDirty( false );
            }
        }

        void PhysxConstraintD6::setDrivePosition( const Transform3<real_Num> &pose )
        {
            m_drivePosition = pose;
        }

        Transform3<real_Num> PhysxConstraintD6::getDrivePosition() const
        {
            return m_drivePosition;
        }

        RawPtr<physx::PxD6Joint> PhysxConstraintD6::getJoint() const
        {
            return m_joint;
        }

        void PhysxConstraintD6::setJoint( RawPtr<physx::PxD6Joint> joint )
        {
            m_joint = joint;
        }

        void PhysxConstraintD6::setDrive( PxD6Drive::Enum index, SmartPtr<IConstraintDrive> drive )
        {
            if(m_joint)
            {
                m_joint->setDrive(
                    static_cast<physx::PxD6Drive::Enum>(index),
                    physx::PxD6JointDrive( drive->getStiffness(), drive->getDamping(),
                                           drive->getForceLimit(), drive->isAcceleration() ) );
            }
        }

        SmartPtr<IConstraintDrive> PhysxConstraintD6::getDrive( PxD6Drive::Enum index ) const
        {
            return nullptr;
        }

        void PhysxConstraintD6::setLinearLimit( SmartPtr<IConstraintLinearLimit> limit )
        {
            if(m_joint)
            {
                auto value = limit->getValue();
                physx::PxJointLinearLimit pxLimit( physx::PxTolerancesScale(), ( value ) );
                m_joint->setLinearLimit( pxLimit );
            }
        }

        SmartPtr<IConstraintLinearLimit> PhysxConstraintD6::getLinearLimit() const
        {
            if(m_joint)
            {
                return nullptr;
            }

            return nullptr;
        }

        void PhysxConstraintD6::setMotion( PxD6Axis::Enum axis, PxD6Motion::Enum type )
        {
            if(m_joint)
            {
                m_joint->setMotion( static_cast<physx::PxD6Axis::Enum>(axis),
                                    static_cast<physx::PxD6Motion::Enum>(type) );
            }
        }

        PxD6Motion::Enum PhysxConstraintD6::getMotion( PxD6Axis::Enum axis ) const
        {
            if(m_joint)
            {
                auto motion = m_joint->getMotion( static_cast<physx::PxD6Axis::Enum>(axis) );
                return static_cast<PxD6Motion::Enum>(motion);
            }

            return static_cast<PxD6Motion::Enum>(0);
        }
    } // end namespace physics
}     // end namespace fb
