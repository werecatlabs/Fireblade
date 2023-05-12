#ifndef ITerrainGenerator_h__
#define ITerrainGenerator_h__

#include <FBCore/Interface/Procedural/IProceduralGenerator.h>

namespace fb
{
    namespace procedural
    {
        class ITerrainGenerator : public IProceduralGenerator
        {
        public:
            ~ITerrainGenerator() override = default;

            virtual SmartPtr<IProceduralScene> getProceduralScene() const = 0;
            virtual void setProceduralScene( SmartPtr<IProceduralScene> scene ) = 0;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // ITerrainGenerator_h__
