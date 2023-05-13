#ifndef FBFileSystemPrerequisites_h__
#define FBFileSystemPrerequisites_h__

#include <FBCore/FBCorePrerequisites.h>

// Forward declaration for zziplib to avoid header file dependency.
using ZZIP_DIR = struct zzip_dir;
using ZZIP_FILE = struct zzip_file;

struct zip_t;

namespace fb
{
    class CFileList;
    class ObfuscatedZipArchive;
    class ZipArchive;
}  // namespace fb

#endif  // FBFileSystemPrerequisites_h__
