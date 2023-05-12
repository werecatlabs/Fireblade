#ifndef IBoxShape2_h__
#define IBoxShape2_h__

#include <FBCore/Interface/Physics/IPhysicsShape2.h>
#include <FBCore/Math/AABB2.h>

namespace fb
{
    namespace physics
    {
        
        class IBoxShape2 : public IPhysicsShape2
        {
        public:
            ~IBoxShape2() override = default;

            virtual void setAABB( const AABB2<real_Num> &box ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IBoxShape2_h__
