#ifndef IPhysicsCompositeShape3_h__
#define IPhysicsCompositeShape3_h__

#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb
{
    namespace physics
    {
        //--------------------------------------------
        class IPhysicsCompositeShape3 : public IPhysicsShape3
        {
        public:
            ~IPhysicsCompositeShape3() override = default;

            virtual Array<SmartPtr<IPhysicsShape3>> getShapes() const = 0;
            virtual void setShapes( const Array<SmartPtr<IPhysicsShape3>> &shapes ) = 0;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsCompositeShape3_h__
