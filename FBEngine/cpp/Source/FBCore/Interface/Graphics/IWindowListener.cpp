#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IWindowListener.h>
#include <FBCore/Rtti/RttiClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IWindowListener, ISharedObject );

        const hash_type IWindowListener::windowClosingHash = StringUtil::getHash( "windowClosing" );
        const hash_type IWindowListener::windowResizedHash = StringUtil::getHash( "windowResized" );
        const hash_type IWindowListener::windowMovedHash = StringUtil::getHash( "windowMoved" );

    }  // end namespace render
}  // end namespace fb
