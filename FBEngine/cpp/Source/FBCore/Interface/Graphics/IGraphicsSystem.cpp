#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IGraphicsSystem, ISharedObject );
    }  // end namespace render
}  // end namespace fb
