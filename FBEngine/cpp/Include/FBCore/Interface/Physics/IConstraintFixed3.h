#ifndef IConstraintFixed3_h__
#define IConstraintFixed3_h__

#include <FBCore/Interface/Physics/IPhysicsConstraint3.h>

namespace fb
{
    namespace physics
    {

        class IConstraintFixed3 : public IPhysicsConstraint3
        {
        public:
            ~IConstraintFixed3() override = default;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IConstraintFixed3_h__
