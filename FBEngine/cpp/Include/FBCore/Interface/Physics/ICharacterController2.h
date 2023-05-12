#ifndef ICharacterController2_h__
#define ICharacterController2_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {
        //---------------------------------------------
        class ICharacterController2 : public ISharedObject
        {
        public:
            ~ICharacterController2() override = default;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // ICharacterController2_h__
