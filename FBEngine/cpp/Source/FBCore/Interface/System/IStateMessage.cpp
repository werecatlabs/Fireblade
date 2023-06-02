#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IStateMessage, ISharedObject );

    const hash_type IStateMessage::STATE_MESSAGE_LEFT = StringUtil::getHash( "left" );
    const hash_type IStateMessage::STATE_MESSAGE_TOP = StringUtil::getHash( "top" );
    const hash_type IStateMessage::STATE_MESSAGE_WIDTH = StringUtil::getHash( "width" );
    const hash_type IStateMessage::STATE_MESSAGE_HEIGHT = StringUtil::getHash( "height" );
    const hash_type IStateMessage::STATE_MESSAGE_METRICSMODE = StringUtil::getHash( "metricsmode" );
    const hash_type IStateMessage::STATE_MESSAGE_ALIGN_HORIZONTAL =
        StringUtil::getHash( "align_horizontal" );
    const hash_type IStateMessage::STATE_MESSAGE_ALIGN_VERTICAL =
        StringUtil::getHash( "align_vertical" );
    const hash_type IStateMessage::STATE_MESSAGE_TEXT = StringUtil::getHash( "text" );

}  // end namespace fb
