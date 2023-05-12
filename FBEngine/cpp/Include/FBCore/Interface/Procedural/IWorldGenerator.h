#ifndef IWorldGenerator_h__
#define IWorldGenerator_h__

#include <FBCore/Interface/Procedural/IProceduralGenerator.h>

namespace fb
{
    namespace procedural
    {
        class IWorldGenerator : public IProceduralGenerator
        {
        public:
            ~IWorldGenerator() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IWorldGenerator_h__
