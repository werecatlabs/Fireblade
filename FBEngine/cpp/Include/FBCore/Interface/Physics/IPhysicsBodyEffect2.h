#ifndef IPhysicsBodyEffect2_h__
#define IPhysicsBodyEffect2_h__

#include <FBCore/Interface/Physics/IPhysicsEffect2.h>

namespace fb
{
    namespace physics
    {
        /** Interface for effects. E.g. wind effects etc. */
        class IPhysicsBodyEffect2 : public IPhysicsEffect2
        {
        public:
            ~IPhysicsBodyEffect2() override = default;

            virtual IPhysicsBody2D *getOwner() const = 0;
            virtual void setOwner( IPhysicsBody2D *owner ) = 0;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsBodyEffect2_h__
