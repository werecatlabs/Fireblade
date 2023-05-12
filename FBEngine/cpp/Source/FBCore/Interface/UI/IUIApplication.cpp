#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUIApplication.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, IUIApplication, ISharedObject );
    }  // end namespace ui
}  // end namespace fb