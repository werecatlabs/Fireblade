#ifndef __IPhysicsConstraint_h__
#define __IPhysicsConstraint_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {

        class IPhysicsConstraint : public ISharedObject
        {
        public:
            ~IPhysicsConstraint() override = default;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsConstraint_h__
