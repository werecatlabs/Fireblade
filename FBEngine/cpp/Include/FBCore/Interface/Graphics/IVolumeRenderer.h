#ifndef IVolumeRenderer_h__
#define IVolumeRenderer_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {

        class IVolumeRenderer : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            virtual ~IVolumeRenderer() override = default;
        };

    }  // namespace render
}  // namespace fb

#endif  // IVolumeRenderer_h__
