#ifndef IFont_h__
#define IFont_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace render
    {
        /** An interface for a font. */
        class IFont : public IResource
        {
        public:
            ~IFont() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace render
}  // namespace fb

#endif  // IFont_h__
