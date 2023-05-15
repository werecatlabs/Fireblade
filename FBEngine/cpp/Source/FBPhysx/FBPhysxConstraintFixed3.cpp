#include <FBPhysx/FBPhysxPCH.h>
#include "FBPhysx/FBPhysxConstraintFixed3.h"
#include <FBCore/FBCore.h>
#include "extensions/PxFixedJoint.h"

namespace fb
{
    namespace physics
    {
        PhysxConstraintFixed3::PhysxConstraintFixed3()
        {
        }

        PhysxConstraintFixed3::~PhysxConstraintFixed3()
        {
            unload( nullptr );
        }

        void PhysxConstraintFixed3::unload( SmartPtr<ISharedObject> data )
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

        void PhysxConstraintFixed3::update()
        {
        }

        RawPtr<physx::PxFixedJoint> PhysxConstraintFixed3::getJoint() const
        {
            return m_joint;
        }

        void PhysxConstraintFixed3::setJoint( RawPtr<physx::PxFixedJoint> joint )
        {
            m_joint = joint;
        }
    } // end namespace physics
}     // end namespace fb
