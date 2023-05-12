#ifndef ICityCenter_h__
#define ICityCenter_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {

        class ICityCenter : public ISharedObject
        {
        public:
            ~ICityCenter() override = default;
        };

    }  // end namespace procedural
}  // end namespace fb

#endif  // ICityCenter_h__
