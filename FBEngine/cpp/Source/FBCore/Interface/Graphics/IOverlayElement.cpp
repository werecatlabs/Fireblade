#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IOverlayElement.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IOverlayElement, ISharedObject );

        const hash_type IOverlayElement::STATE_MESSAGE_LEFT = StringUtil::getHash( "left" );

        const hash_type IOverlayElement::STATE_MESSAGE_TOP = StringUtil::getHash( "top" );

        const hash_type IOverlayElement::STATE_MESSAGE_WIDTH = StringUtil::getHash( "width" );

        const hash_type IOverlayElement::STATE_MESSAGE_HEIGHT = StringUtil::getHash( "height" );

        const hash_type IOverlayElement::STATE_MESSAGE_METRICSMODE =
            StringUtil::getHash( "metricsmode" );

        const hash_type IOverlayElement::STATE_MESSAGE_ALIGN_HORIZONTAL =
            StringUtil::getHash( "align_horizontal" );

        const hash_type IOverlayElement::STATE_MESSAGE_ALIGN_VERTICAL =
            StringUtil::getHash( "align_vertical" );

        const hash_type IOverlayElement::STATE_MESSAGE_TEXT = StringUtil::getHash( "text" );

        const hash_type IOverlayElement::STATE_MESSAGE_ADDCHILD = StringUtil::getHash( "addChild" );
        const hash_type IOverlayElement::STATE_MESSAGE_REMOVECHILD =
            StringUtil::getHash( "removeChild" );

        const hash_type IOverlayElement::STATE_MESSAGE_ATTACH_OBJECT =
            StringUtil::getHash( "attachObject" );
        const hash_type IOverlayElement::STATE_MESSAGE_DETACH_OBJECT =
            StringUtil::getHash( "detachObject" );
        const hash_type IOverlayElement::STATE_MESSAGE_DETACH_ALL_OBJECTS =
            StringUtil::getHash( "detachAllObject" );

    }  // end namespace render
}  // end namespace fb
