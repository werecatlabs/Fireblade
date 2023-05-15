#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxErrorOutput.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace physics
    {
        PhysxErrorOutput::PhysxErrorOutput()
        {
        }

        PhysxErrorOutput::~PhysxErrorOutput()
        {
        }

        void PhysxErrorOutput::reportError( physx::PxErrorCode::Enum e, const char *message,
                                            const char *file, int line )
        {
            auto messageStr = String( "Error: " ) + String( message ) + String( " File: " ) +
                              String( file ) + String( " Line: " ) + StringUtil::toString( line );
            FB_LOG_ERROR( messageStr );
        }
    } // end namespace physics
}     // end namespace fb
