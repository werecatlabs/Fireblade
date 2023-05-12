#ifndef DetailPrototype_h__
#define DetailPrototype_h__

#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    class DetailPrototype : public CSharedObject<ISharedObject>
    {
    public:
        DetailPrototype();
        ~DetailPrototype();
    };
}  // namespace fb

#endif  // DetailPrototype_h__
