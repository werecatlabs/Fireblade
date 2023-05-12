#ifndef IProceduralBuilding_h__
#define IProceduralBuilding_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralBuilding : public ISharedObject
        {
        public:
            ~IProceduralBuilding() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralBuilding_h__
