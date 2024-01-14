#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IWindowListener.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, IWindowListener, ISharedObject );

    const hash_type IWindowListener::windowClosingHash = StringUtil::getHash( "windowClosing" );
    const hash_type IWindowListener::windowResizedHash = StringUtil::getHash( "windowResized" );
    const hash_type IWindowListener::windowMovedHash = StringUtil::getHash( "windowMoved" );

}  // namespace fb::render
