#ifndef __FileInfo_h__
#define __FileInfo_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/StringTypes.h>

namespace fb
{
    struct FileInfo
    {
        /// The full file path.
        String filePath;

        /// The full file path is lower case.
        String filePathLowerCase;

        /// Path name; separated by '/' and ending with '/'
        String path;

        /// Path name; separated by '/' and ending with '/'
        String absolutePath;

        /// Base filename
        String fileName;

        /// Base filename
        String fileNameLowerCase;

        /// Compressed size
        size_t compressedSize = 0;

        /// Uncompressed size
        size_t uncompressedSize = 0;

        //! The id of the file. */
        hash64 fileId = 0;

        //! The id of the archive. */
        hash64 archiveId = 0;

        //! Offset in archive
        u32 offset = 0;

        //! True if this is a folder, false if not.
        bool isDirectory = false;

        bool operator==( const struct FileInfo &other ) const;

        bool operator<( const struct FileInfo &other ) const;
    };
}  // end namespace fb

#endif  // __FileInfo_h__
