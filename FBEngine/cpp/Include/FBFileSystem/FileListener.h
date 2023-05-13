#ifndef FileListener_h__
#define FileListener_h__

#ifndef _FINAL_

#    include <FBFileSystem/FBFileSystemPrerequisites.h>
#    include <FileWatcher/FileWatcher.h>
#    include <FBCore/Base/StringTypes.h>

namespace fb
{
    /// Processes a file action
    class FileListener : public FW::FileWatchListener
    {
    public:
        FileListener();
        ~FileListener() override;

        // void handleFileAction(FW::WatchID watchid, const FW::String& dirStr, const FW::String&
        // filenameStr, FW::Action action);

        void reloadParticles();

        void reloadLuaScripts();

        void reloadTexture( const String &fileName );

        bool reloadAResourceGroupWithoutDestroyingIt( const std::string &pResourceGroupName );

        bool resourceGroupExist( const std::string &pResourceGroupName );
    };
}  // end namespace fb

#endif

#endif  // FileListener_h__
