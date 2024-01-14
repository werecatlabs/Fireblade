#include <FBPhysx/FBPhysxPCH.h>
#include "FBPhysx/FBPhysxConstraintFixed3.h"
#include <FBCore/FBCore.h>
#include "extensions/PxFixedJoint.h"

namespace fb::physics
{
    PhysxConstraintFixed3::PhysxConstraintFixed3() = default;

    PhysxConstraintFixed3::~PhysxConstraintFixed3()
    {
        unload( nullptr );
    }

    void PhysxConstraintFixed3::unload( SmartPtr<ISharedObject> data )
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

    void PhysxConstraintFixed3::update()
    {
    }

    auto PhysxConstraintFixed3::getJoint() const -> RawPtr<physx::PxFixedJoint>
    {
        return m_joint;
    }

    void PhysxConstraintFixed3::setJoint( RawPtr<physx::PxFixedJoint> joint )
    {
        m_joint = joint;
    }

    void PhysxConstraintFixed3::handleStateChanged( SmartPtr<IState> &state )
    {
    }

    void PhysxConstraintFixed3::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
    }

    void PhysxConstraintFixed3::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

}  // namespace fb::physics
