#ifndef IFontManager_h__
#define IFontManager_h__

#include <FBCore/Interface/Resource/IResourceManager.h>

namespace fb
{
    namespace render
    {
        /** Interface for a font manager. */
        class IFontManager : public IResourceManager
        {
        public:
            /** Virtual destructor. */
            ~IFontManager() override = default;

            /** Function used to clone a font.
            @param name The name of the existing font.
            @param clonedFontName The name of the cloned font.
            */
            virtual SmartPtr<IFont> cloneFont( const String &name, const String &clonedFontName ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // IFontManager_h__
