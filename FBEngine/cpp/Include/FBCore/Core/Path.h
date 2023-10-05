#ifndef __FB_BasePath_h__
#define __FB_BasePath_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    /** Provides some useful file functions. */
    template <class T>
    class BasePath
    {
    public:
        /** Gets a file path from a file path. */
        static T getFilePath( const T &filePath );

        /** Gets a file name from a file path. */
        static T getFileName( const T &filePath );

        /** Gets the file extension from a file path. */
        static T getFileExtension( const T &filePath );

        static T getFileNameWithoutExtension( const T &path );

        static void setWorkingDirectory( const T &dir );
        static T getWorkingDirectory();

        static Array<T> getFolders( const T &path, bool recursive = false );
        static Array<T> getPaths( const T &path, bool recursive = false );

        static Array<T> getFiles( const T &path );
        static Array<T> getFilesAsAbsolutePaths( const T &path, bool recursive = false );

        static Array<T> getFiles( const T &path, const T &extension );
        static Array<T> getFilesAsAbsolutePaths( const T &path, const T &extension, bool recursive = false );

        static T getLeaf( const T &path );

        static bool isFolder( const T &path );
        static bool isFile( const T &filePath );

        static bool isExistingFolder( const T &path );
        static bool isExistingFile( const T &filePath );

        static Array<T> getFilePathFolders( const T &filePath );

        static bool isFilesEqual( const T &lFilePath, const T &rFilePath );

        static size_t getFileSize( const T &filename );

        static void copyFolder( const T &srcPath, const T &dstPath );
        static void copyFile( const T &srcPath, const T &dstPath );
        static void createDirectories( const T &dst );

        static String getMD5( const T &filePath );
        static String getMD5( void *data, size_t size );

        static void deleteFolder( const T &path );
        static void deleteFile( const T &filePath );
        static void deleteFiles( const T &path );

        static void rename( const T &pathOld, const T &pathNew );

        static T getAbsolutePath( const T &path );
        static T getAbsolutePath( const T &path, const T &relativePath );
        static T getRelativePath( const T &path, const T &relativePath );

        static T lexically_normal( const T &path );
        static T lexically_normal( const T &path, const T &relativePath );
        static T lexically_relative( const T &path, const T &relativePath );
        static T lexically_proximate( const T &path, const T &relativePath );

        static T macBundlePath();

        static bool isPathAbsolute( const T &basePath );
        static bool isPathRelative( const T &basePath, const T &relativePath );

        static bool hasFileName( const String &path );

        static T readAllText( const T &path );
        static void writeAllText( const T &path, const T &contents );
    };

    using Path = BasePath<String>;
    using PathW = BasePath<StringW>;
}  // end namespace fb

#endif  // __FB_BasePath_h__
