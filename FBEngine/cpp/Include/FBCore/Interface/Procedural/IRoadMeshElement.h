#ifndef IRoadMeshElement_h__
#define IRoadMeshElement_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IRoadMeshElement : public ISharedObject
        {
        public:
            ~IRoadMeshElement() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IRoadMeshElement_h__
