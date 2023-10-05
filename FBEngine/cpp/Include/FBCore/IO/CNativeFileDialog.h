#ifndef CNativeFileDialog_h__
#define CNativeFileDialog_h__

#include <FBCore/Interface/IO/INativeFileDialog.h>

namespace fb
{

    class CNativeFileDialog : public INativeFileDialog
    {
    public:
        // Default constructor.
        CNativeFileDialog();

        // Virtual destructor.
        ~CNativeFileDialog() override;

        /** @copydoc ISharedObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::reload */
        void reload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        // Opens the native file dialog window, returns a Result value.
        Result openDialog() override;

        // Gets the selected file path.
        String getFilePath() const override;

        // Sets the selected file path.
        void setFilePath( const String &filePath ) override;

        // Gets the selected file extension.
        String getFileExtension() const override;

        // Sets the selected file extension.
        void setFileExtension( const String &fileExtension ) override;

        // Gets the dialog mode (Select, Open or Save).
        DialogMode getDialogMode() const override;

        // Sets the dialog mode (Select, Open or Save).
        void setDialogMode( DialogMode mode ) override;

        // Gets the filter mode (Files or Directories).
        FilterMode getFilterMode() const override;

        // Sets the filter mode (Files or Directories).
        void setFilterMode( FilterMode mode ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        // The selected file path.
        String m_filePath;

        // The selected file extension.
        String m_fileExtension;

        // The dialog mode (Select, Open or Save).
        DialogMode m_dialogMode = DialogMode::Open;

        // The filter mode (Files or Directories).
        FilterMode m_filterMode = FilterMode::FilterMode_Dirs;
    };

}  // namespace fb

#endif  // CNativeFileDialog_h__
