#ifndef IJointFixed2_h__
#define IJointFixed2_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace physics
    {

        class IConstraintFixed2 : public ISharedObject
        {
        public:
            ~IConstraintFixed2() = default;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IJointFixed2_h__
