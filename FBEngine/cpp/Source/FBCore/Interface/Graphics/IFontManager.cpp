#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IFontManager.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IFontManager, IResourceManager );
    }  // end namespace render
}  // end namespace fb