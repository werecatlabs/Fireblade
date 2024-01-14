#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IGraphicsSystem, ISharedObject );
}  // namespace fb::render
