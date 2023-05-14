#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IRenderTexture.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IRenderTexture, IRenderTarget );
    }  // namespace render
}  // namespace fb
