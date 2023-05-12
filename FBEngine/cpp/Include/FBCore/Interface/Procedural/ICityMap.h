#ifndef ICityMap_h__
#define ICityMap_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class ICityMap : public ISharedObject
        {
        public:
            ~ICityMap() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // ICityMap_h__
