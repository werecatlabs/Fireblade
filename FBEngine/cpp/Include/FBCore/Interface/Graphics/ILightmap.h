#ifndef ILightmap_h__
#define ILightmap_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        class ILightmap : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~ILightmap() override = default;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // ILightmap_h__
