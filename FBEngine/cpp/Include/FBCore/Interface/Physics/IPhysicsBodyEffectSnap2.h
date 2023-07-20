#ifndef IPhysicsBodyEffectSnap2_h__
#define IPhysicsBodyEffectSnap2_h__

#include "FBCore/Interface/Physics/IPhysicsBodyEffect2.h"

namespace fb
{
    namespace physics
    {

        class IPhysicsBodyEffectSnap2 : public IPhysicsBodyEffect2
        {
        public:
            ~IPhysicsBodyEffectSnap2() override = default;

            virtual Vector2<real_Num> getTarget() const = 0;
            virtual void setTarget( const Vector2<real_Num> &target ) = 0;

            virtual bool getUseAxis( int axis ) const = 0;
            virtual void setUseAxis( int axis, bool val ) = 0;
        };

    }  // namespace physics
}  // end namespace fb

#endif  // IPhysicsBodyEffectSnap2_h__
