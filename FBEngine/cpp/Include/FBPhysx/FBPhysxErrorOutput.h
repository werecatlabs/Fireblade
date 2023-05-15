#ifndef FBPhysxErrorOutput_h__
#define FBPhysxErrorOutput_h__

#include "foundation/PxErrorCallback.h"

namespace fb
{
    namespace physics
    {
        class PhysxErrorOutput : public physx::PxErrorCallback
        {
        public:
            PhysxErrorOutput();
            ~PhysxErrorOutput() override;

            void reportError( physx::PxErrorCode::Enum e, const char *message, const char *file,
                              int line ) override;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxErrorOutput_h__
