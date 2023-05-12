#ifndef IProceduralLevel25_h__
#define IProceduralLevel25_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Procedural/IProceduralGenerator.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralLevel25 : public IProceduralGenerator
        {
        public:
            ~IProceduralLevel25() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralLevel25_h__
