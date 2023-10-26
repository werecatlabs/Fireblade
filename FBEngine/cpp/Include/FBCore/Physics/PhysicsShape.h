#ifndef CPhysicsShape_h__
#define CPhysicsShape_h__

#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb
{
    namespace physics
    {

        class PhysicsShape : public IPhysicsShape3
        {
        public:
            PhysicsShape(){}
            ~PhysicsShape(){}
            
        };

    }  // namespace physics
}  // namespace fb

#endif  // CPhysicsShape_h__
