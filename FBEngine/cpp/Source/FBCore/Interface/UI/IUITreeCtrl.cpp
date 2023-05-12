#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUITreeCtrl.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, IUITreeCtrl, IUIElement );

        const hash_type IUITreeCtrl::clearHash = StringUtil::getHash( "clear" );

    }  // end namespace ui
}  // end namespace fb
