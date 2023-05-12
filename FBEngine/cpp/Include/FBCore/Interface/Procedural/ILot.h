#ifndef ILot_h__
#define ILot_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {

        class ILot : public ISharedObject
        {
        public:
            ~ILot() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace procedural
}  // namespace fb

#endif  // ILot_h__
