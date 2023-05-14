#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, IUIElement, ISharedObject );

        const hash_type IUIElement::STATE_MESSAGE_ADD_CHILD = StringUtil::getHash( "addChild" );
        const hash_type IUIElement::STATE_MESSAGE_REMOVE_CHILD = StringUtil::getHash( "removeChild" );
    }  // end namespace ui
}  // end namespace fb
