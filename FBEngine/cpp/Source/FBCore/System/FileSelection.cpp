#include <FBCore/FBCorePCH.h>
#include <FBCore/System/FileSelection.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::editor
{

    FB_CLASS_REGISTER_DERIVED( editor, FileSelection, ISharedObject );

    FileSelection::FileSelection() = default;

    FileSelection::~FileSelection() = default;

    auto FileSelection::getFilePath() const -> String
    {
        return m_filePath;
    }

    void FileSelection::setFilePath( const String &filePath )
    {
        m_filePath = StringUtil::cleanupPath( filePath );
    }

    auto FileSelection::getFileInfo() const -> FileInfo
    {
        return m_fileInfo;
    }

    void FileSelection::setFileInfo( const FileInfo &fileInfo )
    {
        m_fileInfo = fileInfo;
    }

}  // namespace fb::editor
