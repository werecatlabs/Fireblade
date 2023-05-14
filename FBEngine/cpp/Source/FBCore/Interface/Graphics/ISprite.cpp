#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ISprite.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ISprite, ISharedObject );
    }  // end namespace render
}  // end namespace fb

