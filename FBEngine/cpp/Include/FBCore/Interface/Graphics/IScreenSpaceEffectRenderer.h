#ifndef IScreenSpaceEffectRenderer_h__
#define IScreenSpaceEffectRenderer_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace render
    {
        //-------------------------------------------------
        class IScreenSpaceEffectRenderer : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IScreenSpaceEffectRenderer() override = default;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IScreenSpaceEffectRenderer_h__
