#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IGraphicsSystem, ISharedObject );
    }  // end namespace render
}  // end namespace fb
