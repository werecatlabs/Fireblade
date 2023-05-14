#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, IUIDropTarget, ISharedObject );
    }  // end namespace ui
}  // end namespace fb
