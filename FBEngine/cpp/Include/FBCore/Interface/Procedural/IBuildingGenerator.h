#ifndef IBuildingGenerator_h__
#define IBuildingGenerator_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IBuildingGenerator : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            virtual ~IBuildingGenerator() = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IBuildingGenerator_h__
