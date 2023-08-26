#ifndef FileSelection_h__
#define FileSelection_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/FileInfo.h>
#include "FBCore/System/RttiClass.h"

namespace fb
{
    namespace editor
    {

        class FileSelection : public ISharedObject
        {
        public:
            FileSelection();
            ~FileSelection();

            String getFilePath() const;
            void setFilePath( const String &filePath );

            FileInfo getFileInfo() const;
            void setFileInfo( const FileInfo &fileInfo );

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_filePath;
            FileInfo m_fileInfo;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // FileSelection_h__
