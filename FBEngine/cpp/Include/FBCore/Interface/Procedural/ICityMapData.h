#ifndef ICityMapData_h__
#define ICityMapData_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class ICityMapData : public ISharedObject
        {
        public:
            ~ICityMapData() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // ICityMapData_h__
