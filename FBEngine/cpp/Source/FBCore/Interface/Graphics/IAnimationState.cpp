#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IAnimationState.h>
#include <FBCore/Rtti/RttiClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( render, IAnimationState, ISharedObject );
    }  // end namespace render
}  // end namespace fb
