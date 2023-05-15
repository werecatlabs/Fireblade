#ifndef FileSystemHelper_h__
#define FileSystemHelper_h__

#include <FBCore/Interface/IO/IFileSystem.h>

namespace fb
{

    class FileSystemHelper
    {
    public:
        static Array<String> _getFileNamesInFolder( IFileSystem *system, const char *folderName );

        static Array<String> _getFileNamesWithExt( IFileSystem *system, const char *extension );
    };

}  // namespace fb

#endif  // FileSystemHelper_h__
