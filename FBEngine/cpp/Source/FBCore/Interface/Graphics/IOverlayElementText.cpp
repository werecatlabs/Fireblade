#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IOverlayElementText.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IOverlayElementText, IOverlayElement );
    }  // end namespace render
}  // end namespace fb
