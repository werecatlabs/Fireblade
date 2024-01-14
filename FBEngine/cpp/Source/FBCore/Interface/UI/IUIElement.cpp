#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb::ui, IUIElement, ISharedObject );

    const hash_type IUIElement::STATE_MESSAGE_ADD_CHILD = StringUtil::getHash( "addChild" );
    const hash_type IUIElement::STATE_MESSAGE_REMOVE_CHILD = StringUtil::getHash( "removeChild" );
}  // namespace fb::ui
