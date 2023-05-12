#ifndef IProceduralTexture_h__
#define IProceduralTexture_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace procedural
    {
        class IProceduralTexture : public ISharedObject
        {
        public:
            ~IProceduralTexture() override = default;
        };
    }  // end namespace procedural
}  // end namespace fb

#endif  // IProceduralTexture_h__
