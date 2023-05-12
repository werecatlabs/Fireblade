#ifndef __IUIFileBrowser_H
#define __IUIFileBrowser_H

#include <FBCore/Interface/UI/IUIDialogBox.h>

namespace fb
{
    namespace ui
    {
        /** Interface for a file browser. */
        class IUIFileBrowser : public IUIDialogBox
        {
        public:
            enum class DialogMode
            {
                Select,
                // Select Directory Mode
                Open,
                // Open File mode
                Save  // Save File mode.
            };

            enum class FilterMode
            {
                FilterMode_Files = 0x01,
                FilterMode_Dirs = 0x02
            };

            /** Destructor. */
            ~IUIFileBrowser() override = default;

            /** Gets the file extension.
            @return A string containing the file extension.
            */
            virtual String getFilePath() const = 0;

            /** Sets the file extension.
            @param fileExtension A string containing the file extension.
            */
            virtual void setFilePath( const String &filePath ) = 0;

            /** Gets the file extension.
            @return A string containing the file extension.
            */
            virtual String getFileExtension() const = 0;

            /** Sets the file extension.
            @param fileExtension A string containing the file extension.
            */
            virtual void setFileExtension( const String &fileExtension ) = 0;

            virtual DialogMode getDialogMode() const = 0;
            virtual void setDialogMode( DialogMode mode ) = 0;

            virtual FilterMode getFilterMode() const = 0;
            virtual void setFilterMode( FilterMode mode ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // FB_IUIFRAME_H
