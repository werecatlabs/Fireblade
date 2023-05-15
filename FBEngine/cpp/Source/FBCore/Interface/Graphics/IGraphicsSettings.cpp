#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IGraphicsSettings.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IGraphicsSettings, ISharedObject );
    }  // end namespace render
}  // end namespace fb
