#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/FileList.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FileList, IFileList );

    static const String emptyFileListEntry;

    FileList::FileList( const String &String, bool ignoreCase, bool ignorePaths ) : m_path( String )
    {
        m_path = StringUtil::replace( m_path, '\\', '/' );
    }

    FileList::~FileList()
    {
        setFilesPtr( nullptr );
    }

    auto FileList::getNumFiles() const -> u32
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            return static_cast<u32>( files.size() );
        }

        return 0;
    }

    void FileList::sort()
    {
        std::sort( getFiles().begin(), getFiles().end() );
    }

    auto FileList::getFileName( u32 index ) const -> String
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            if( index >= files.size() )
            {
                return emptyFileListEntry;
            }

            return String( files[index].fileName.c_str() );
        }

        return "";
    }

    //! Gets the full name of a file in the list, String included, based on an index.
    auto FileList::getFullFileName( u32 index ) const -> String
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            if( index >= files.size() )
            {
                return emptyFileListEntry;
            }

            return String( files[index].filePath.c_str() );
        }

        return "";
    }

    auto FileList::getFileSize( u32 index ) const -> u32
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            auto size = files.size();

            if( index < size )
            {
                return static_cast<u32>( index < size ? files[index].uncompressedSize : 0 );
            }
        }

        return 0;
    }

    auto FileList::getFileOffset( u32 index ) const -> u32
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            auto size = files.size();

            if( index < size )
            {
                return ( index < size ? files[index].offset : 0 );
            }
        }

        return 0;
    }

    auto FileList::findFile( const String &filename, bool isDirectory = false ) const -> s32
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;

            for( size_t i = 0; i < files.size(); ++i )
            {
                const auto &file = files[i];
                if( file.fileName == filename || file.fileNameLowerCase == filename )
                {
                    return static_cast<s32>( i );
                }
            }
        }

        return -1;
    }

    auto FileList::getPath() const -> String
    {
        return m_path;
    }

    auto FileList::getFiles() const -> Array<FileInfo>
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;
            return Array<FileInfo>( files.begin(), files.end() );
        }

        return {};
    }

    void FileList::setFiles( Array<FileInfo> files )
    {
        auto newPtr = fb::make_shared<ConcurrentArray<FileInfo>>();
        *newPtr = ConcurrentArray<FileInfo>( files.begin(), files.end() );
        setFilesPtr( newPtr );

        FB_ASSERT( getFilesPtr()->size() == files.size() );
    }

    void FileList::setPath( const String &path )
    {
        m_path = path;
    }

    auto FileList::getFilesPtr() const -> SharedPtr<ConcurrentArray<FileInfo>>
    {
        return m_files;
    }

    void FileList::setFilesPtr( SharedPtr<ConcurrentArray<FileInfo>> p )
    {
        m_files = p;
    }

    void FileList::addFilePtr( const FileInfo &file )
    {
        auto p = getFilesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<FileInfo>>();
            p->reserve( 2048 );
            m_files = p;
        }

        if( p )
        {
            auto &files = *p;
            files.push_back( file );
        }
    }

    void FileList::removeFilePtr( const FileInfo &file )
    {
        if( auto p = getFilesPtr() )
        {
            auto &archives = *p;

            auto newFiles = Array<FileInfo>( archives.begin(), archives.end() );

            auto it = std::find( newFiles.begin(), newFiles.end(), file );
            if( it != newFiles.end() )
            {
                newFiles.erase( it );
            }

            auto newPtr = fb::make_shared<ConcurrentArray<FileInfo>>( newFiles.begin(), newFiles.end() );
            setFilesPtr( newPtr );
        }
    }

    auto FileList::findFileInfo( hash64 id, FileInfo &fileInfo, bool ignorePath ) const -> bool
    {
        if( auto pFiles = getFilesPtr() )
        {
            auto &files = *pFiles;
            for( auto &file : files )
            {
                if( file.fileId == id )
                {
                    fileInfo = file;
                    return true;
                }
            }
        }

        return false;
    }

    auto FileList::findFileInfo( const String &filePath, FileInfo &fileInfo, bool ignorePath ) const
        -> bool
    {
        if( auto pFiles = getFilesPtr() )
        {
            auto &files = *pFiles;

            auto filePathHash = StringUtil::getHash( filePath );
            for( auto &file : files )
            {
                if( file.fileId == filePathHash )
                {
                    fileInfo = file;
                    return true;
                }
            }

            for( auto &file : files )
            {
                if( file.filePath == filePath || file.filePathLowerCase == filePath )
                {
                    fileInfo = file;
                    return true;
                }
            }

            if( ignorePath )
            {
                auto fileName = Path::getFileName( filePath );
                for( auto &file : files )
                {
                    if( file.fileName == fileName || file.fileNameLowerCase == fileName )
                    {
                        fileInfo = file;
                        return true;
                    }
                }
            }
        }

        if( auto pFiles = getFilesPtr() )
        {
            if( Path::isPathAbsolute( filePath ) )
            {
                auto &files = *pFiles;
                for( auto &file : files )
                {
                    if( file.absolutePath == filePath )
                    {
                        fileInfo = file;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    auto FileList::exists( const String &filePath, bool ignorePath, bool ignoreCase ) const -> bool
    {
        if( auto p = getFilesPtr() )
        {
            auto &files = *p;

            if( ignorePath )
            {
                auto fileName = Path::getFileName( filePath );
                auto fileNameLower = StringUtil::make_lower( fileName );
                for( auto &file : files )
                {
                    if( ignoreCase )
                    {
                        if( file.fileNameLowerCase == fileNameLower )
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if( file.fileName == fileName )
                        {
                            return true;
                        }
                    }
                }
            }
            else
            {
                auto filePathLower = StringUtil::make_lower( filePath );
                for( auto &file : files )
                {
                    if( ignoreCase )
                    {
                        if( file.filePathLowerCase == filePathLower )
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if( file.filePath == filePath )
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

}  // end namespace fb
