#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IAnimationStateController.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( render, IAnimationStateController, ISharedObject );
    }  // end namespace render
}  // end namespace fb
