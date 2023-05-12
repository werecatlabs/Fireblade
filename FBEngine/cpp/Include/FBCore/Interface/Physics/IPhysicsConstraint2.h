#ifndef IPhysicsConstraint2_h__
#define IPhysicsConstraint2_h__

#include <FBCore/Interface/Physics/IPhysicsConstraint.h>

namespace fb
{
    namespace physics
    {
        //--------------------------------------------
        class IPhysicsConstraint2 : public IPhysicsConstraint
        {
        public:
            ~IPhysicsConstraint2() override = default;

            virtual SmartPtr<IPhysicsBody2D> getBodyA() const = 0;
            virtual void setBodyA( SmartPtr<IPhysicsBody2D> val ) = 0;

            virtual SmartPtr<IPhysicsBody2D> getBodyB() const = 0;
            virtual void setBodyB( SmartPtr<IPhysicsBody2D> val ) = 0;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsConstraint2_h__
