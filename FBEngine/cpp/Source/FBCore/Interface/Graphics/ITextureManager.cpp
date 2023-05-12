#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ITextureManager.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ITextureManager, IResourceManager );
    }  // end namespace render
}  // end namespace fb