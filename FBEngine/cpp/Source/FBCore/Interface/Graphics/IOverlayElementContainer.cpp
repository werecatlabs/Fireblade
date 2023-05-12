#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IOverlayElementContainer.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IOverlayElementContainer, IOverlayElement );
    }  // end namespace render
}  // end namespace fb
