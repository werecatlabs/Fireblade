#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/FileInfo.h>

namespace fb
{
    bool FileInfo::operator==( const struct FileInfo &other ) const
    {
        if( isDirectory != other.isDirectory )
        {
            return false;
        }

        return filePath == other.filePath;
    }

    bool FileInfo::operator<( const struct FileInfo &other ) const
    {
        if( isDirectory != other.isDirectory )
        {
            return isDirectory;
        }

        return filePath < other.filePath;
    }
}  // end namespace fb
