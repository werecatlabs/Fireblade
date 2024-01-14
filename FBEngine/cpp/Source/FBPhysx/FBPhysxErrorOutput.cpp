#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxErrorOutput.h>
#include <FBCore/FBCore.h>

namespace fb::physics
{
    PhysxErrorOutput::PhysxErrorOutput() = default;

    PhysxErrorOutput::~PhysxErrorOutput() = default;

    void PhysxErrorOutput::reportError( physx::PxErrorCode::Enum e, const char *message,
                                        const char *file, int line )
    {
        auto messageStr = String( "Error: " ) + String( message ) + String( " File: " ) +
                          String( file ) + String( " Line: " ) + StringUtil::toString( line );
        FB_LOG_ERROR( messageStr );
    }
}  // namespace fb::physics
