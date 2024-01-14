#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IDebugCircle.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, IDebugCircle, ISharedObject );
}  // namespace fb::render
