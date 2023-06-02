#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ICamera, IFrustum );
    }  // end namespace render
}  // end namespace fb
