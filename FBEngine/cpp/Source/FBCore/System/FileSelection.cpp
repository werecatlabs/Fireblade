#include <FBCore/FBCorePCH.h>
#include <FBCore/System/FileSelection.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace editor
    {

        FB_CLASS_REGISTER_DERIVED( editor, FileSelection, ISharedObject );

        FileSelection::FileSelection()
        {
        }

        FileSelection::~FileSelection()
        {
        }

        String FileSelection::getFilePath() const
        {
            return m_filePath;
        }

        void FileSelection::setFilePath( const String &filePath )
        {
            m_filePath = StringUtil::cleanupPath( filePath );
        }

        FileInfo FileSelection::getFileInfo() const
        {
            return m_fileInfo;
        }

        void FileSelection::setFileInfo( const FileInfo &fileInfo )
        {
            m_fileInfo = fileInfo;
        }

    }  // end namespace editor
}  // end namespace fb
