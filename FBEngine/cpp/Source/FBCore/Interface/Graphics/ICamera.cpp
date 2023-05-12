#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ICamera, IFrustum );
    }  // end namespace render
}  // end namespace fb
