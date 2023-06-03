#ifndef DetailPrototype_h__
#define DetailPrototype_h__

#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    class DetailPrototype : public SharedObject<ISharedObject>
    {
    public:
        DetailPrototype();
        ~DetailPrototype();
    };
}  // namespace fb

#endif  // DetailPrototype_h__
