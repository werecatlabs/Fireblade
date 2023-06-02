#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IO/IFileListener.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IFileListener, ISharedObject );

    const u32 IFileListener::FILE_LISTENER_ACTION_ADD = 1;
    const u32 IFileListener::FILE_LISTENER_ACTION_DELETE = 2;
    const u32 IFileListener::FILE_LISTENER_ACTION_MODIFIED = 4;

}  // end namespace fb