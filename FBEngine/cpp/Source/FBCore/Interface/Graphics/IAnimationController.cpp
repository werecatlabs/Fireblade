#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IAnimationController.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( render, IAnimationController, ISharedObject );
    }  // end namespace render
}  // end namespace fb