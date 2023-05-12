#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ICubemap.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ICubemap, ISharedObject );
    }  // end namespace render
}  // end namespace fb