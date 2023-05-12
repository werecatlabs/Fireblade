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

            virtual bool isDirty() const = 0;
            virtual void setDirty( bool dirty ) = 0;
            virtual void updateDirty() = 0;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsConstraint_h__
