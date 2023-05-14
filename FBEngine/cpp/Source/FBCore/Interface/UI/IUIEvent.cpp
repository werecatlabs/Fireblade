#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUIEvent.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, IUIEvent, IEvent );
    }  // end namespace ui
}  // end namespace fb
