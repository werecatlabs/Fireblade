#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IWindow, IRenderTarget );

        const hash_type IWindow::RESIZE_HASH = StringUtil::getHash( "resize" );
        const hash_type IWindow::REPOSITION_HASH = StringUtil::getHash( "reposition" );
        const hash_type IWindow::MOVED_OR_RESIZED_HASH = StringUtil::getHash( "MOVED_OR_RESIZED" );

    }  // end namespace render
}  // end namespace fb
