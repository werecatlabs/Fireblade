#ifndef IRoadHitPoint_h__
#define IRoadHitPoint_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace procedural
    {
        class IRoadHitPoint : public ISharedObject
        {
        public:
            ~IRoadHitPoint() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadHitPoint_h__
