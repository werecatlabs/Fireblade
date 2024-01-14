#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/FileInfo.h>

namespace fb
{
    auto FileInfo::operator==( const struct FileInfo &other ) const -> bool
    {
        if( isDirectory != other.isDirectory )
        {
            return false;
        }

        return filePath == other.filePath;
    }

    auto FileInfo::operator<( const struct FileInfo &other ) const -> bool
    {
        if( isDirectory != other.isDirectory )
        {
            return isDirectory;
        }

        return filePath < other.filePath;
    }
}  // end namespace fb
