#ifndef INativeFileDialog_h__
#define INativeFileDialog_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{

    /** Interface for a native file dialog. */
    class INativeFileDialog : public ISharedObject
    {
    public:
        /** The mode of the file dialog. */
        enum class DialogMode
        {
            Select,  // Select File mode.
            Open,    // Open File mode.
            Save     // Save File mode.
        };

        /** The mode for filtering files and directories. */
        enum class FilterMode
        {
            FilterMode_Files = 0x01,  // Filter only files.
            FilterMode_Dirs = 0x02    // Filter only directories.
        };

        /** The result of the file dialog. */
        enum class Result
        {
            Dialog_Error,   // An error occurred.
            Dialog_Okay,    // The user selected a file.
            Dialog_Cancel,  // The user cancelled the operation.
            Count
        };

        /** Virtual destructor. */
        ~INativeFileDialog() override = default;

        /** Opens the file dialog.
         * @return The result of the file dialog.
         */
        virtual Result openDialog() = 0;

        /** Gets the selected file path.
         * @return A string containing the selected file path.
         */
        virtual String getFilePath() const = 0;

        /** Sets the selected file path.
         * @param filePath A string containing the selected file path.
         */
        virtual void setFilePath( const String &filePath ) = 0;

        /** Gets the file extension.
         * @return A string containing the file extension.
         */
        virtual String getFileExtension() const = 0;

        /** Sets the file extension.
         * @param fileExtension A string containing the file extension.
         */
        virtual void setFileExtension( const String &fileExtension ) = 0;

        /** Gets the mode of the file dialog.
         * @return The mode of the file dialog.
         */
        virtual DialogMode getDialogMode() const = 0;

        /** Sets the mode of the file dialog.
         * @param mode The mode of the file dialog.
         */
        virtual void setDialogMode( DialogMode mode ) = 0;

        /** Gets the filter mode for files and directories.
         * @return The filter mode for files and directories.
         */
        virtual FilterMode getFilterMode() const = 0;

        /** Sets the filter mode for files and directories.
         * @param mode The filter mode for files and directories.
         */
        virtual void setFilterMode( FilterMode mode ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // INativeFileDialog_h__
