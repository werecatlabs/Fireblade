#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxConstraintD6.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxManager.h>
#include "FBPhysx/PhysxUtil.h"
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>
#include <PxExtensionsAPI.h>

namespace fb::physics
{
    PhysxConstraintD6::~PhysxConstraintD6()
    {
        unload( nullptr );
    }

    void PhysxConstraintD6::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            using namespace physx;

            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            auto physicsManager =
                fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );

            auto physics = physicsManager->getPhysics();
            auto j = fb::make_ptr<PhysxConstraintD6>();

            RawPtr<PxRigidDynamic> pxActor0;
            RawPtr<PxRigidDynamic> pxActor1;

            if( auto actor0 = getBodyA() )
            {
                auto pActor0 = fb::static_pointer_cast<PhysxRigidDynamic>( actor0 );
                pxActor0 = pActor0->getActor();
            }

            if( auto actor1 = getBodyB() )
            {
                auto pActor1 = fb::static_pointer_cast<PhysxRigidDynamic>( actor1 );
                pxActor1 = pActor1->getActor();
            }

            auto localFrame0 = getLocalPose( JointActorIndex::Enum::eACTOR0 );
            auto localFrame1 = getLocalPose( JointActorIndex::Enum::eACTOR1 );

            auto pxLocalFrame0 = PhysxUtil::toPx( localFrame0 );
            auto pxLocalFrame1 = PhysxUtil::toPx( localFrame1 );

            auto d6joint = PxD6JointCreate( *physics, pxActor0, pxLocalFrame0, pxActor1, pxLocalFrame1 );
            setJoint( d6joint );

            setLoadingState( LoadingState::Loaded );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxConstraintD6::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            if( m_joint )
            {
                m_joint->release();
                m_joint = nullptr;
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxConstraintD6::update()
    {
        if( isDirty() )
        {
            if( m_joint )
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

    auto PhysxConstraintD6::getDrivePosition() const -> Transform3<real_Num>
    {
        return m_drivePosition;
    }

    auto PhysxConstraintD6::getJoint() const -> RawPtr<physx::PxD6Joint>
    {
        return m_joint;
    }

    void PhysxConstraintD6::setJoint( RawPtr<physx::PxD6Joint> joint )
    {
        m_joint = joint;
    }

    void PhysxConstraintD6::setDrive( D6Drive::Enum index, SmartPtr<IConstraintDrive> drive )
    {
        if( m_joint )
        {
            m_joint->setDrive(
                static_cast<physx::PxD6Drive::Enum>( index ),
                physx::PxD6JointDrive( drive->getStiffness(), drive->getDamping(),
                                       drive->getForceLimit(), drive->isAcceleration() ) );
        }
    }

    auto PhysxConstraintD6::getDrive( D6Drive::Enum index ) const -> SmartPtr<IConstraintDrive>
    {
        return nullptr;
    }

    void PhysxConstraintD6::setLinearLimit( SmartPtr<IConstraintLinearLimit> limit )
    {
        if( m_joint )
        {
            auto value = limit->getValue();
            physx::PxJointLinearLimit pxLimit( physx::PxTolerancesScale(), ( value ) );
            m_joint->setLinearLimit( pxLimit );
        }
    }

    auto PhysxConstraintD6::getLinearLimit() const -> SmartPtr<IConstraintLinearLimit>
    {
        if( m_joint )
        {
            return nullptr;
        }

        return nullptr;
    }

    void PhysxConstraintD6::setMotion( D6Axis::Enum axis, D6Motion::Enum type )
    {
        if( m_joint )
        {
            m_joint->setMotion( static_cast<physx::PxD6Axis::Enum>( axis ),
                                static_cast<physx::PxD6Motion::Enum>( type ) );
        }
    }

    auto PhysxConstraintD6::getMotion( D6Axis::Enum axis ) const -> D6Motion::Enum
    {
        if( m_joint )
        {
            auto motion = m_joint->getMotion( static_cast<physx::PxD6Axis::Enum>( axis ) );
            return static_cast<D6Motion::Enum>( motion );
        }

        return static_cast<D6Motion::Enum>( 0 );
    }

    void PhysxConstraintD6::handleStateChanged( SmartPtr<IState> &state )
    {
        using namespace physx;

        RawPtr<PxRigidDynamic> pxActor0;
        RawPtr<PxRigidDynamic> pxActor1;

        if( auto actor0 = getBodyA() )
        {
            auto pActor0 = fb::static_pointer_cast<PhysxRigidDynamic>( actor0 );
            pxActor0 = pActor0->getActor();
        }

        if( auto actor1 = getBodyB() )
        {
            auto pActor1 = fb::static_pointer_cast<PhysxRigidDynamic>( actor1 );
            pxActor1 = pActor1->getActor();
        }

        auto constraint = getJoint();
        if( constraint )
        {
            constraint->setActors( pxActor0, pxActor1 );
        }
    }

    void PhysxConstraintD6::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
    }

    void PhysxConstraintD6::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

}  // namespace fb::physics
