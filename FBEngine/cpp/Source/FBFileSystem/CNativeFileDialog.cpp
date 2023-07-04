#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/CNativeFileDialog.h>
#include <FBFileSystem/extern/nativefiledialog/nfd.h>
#include <FBFileSystem/extern/nativefiledialog/common.h>
#include <FBCore/Core/StringUtil.h>
#include <iostream>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, CNativeFileDialog, SharedObject<INativeFileDialog> );

    CNativeFileDialog::CNativeFileDialog()
    {
    }

    CNativeFileDialog::~CNativeFileDialog()
    {
    }

    void CNativeFileDialog::load( SmartPtr<ISharedObject> data )
    {
    }

    void CNativeFileDialog::reload( SmartPtr<ISharedObject> data )
    {
    }

    void CNativeFileDialog::unload( SmartPtr<ISharedObject> data )
    {
    }

    INativeFileDialog::Result CNativeFileDialog::openDialog()
    {
        nfdchar_t *pOutPath = nullptr;
        auto result = static_cast<nfdresult_t>( 0 );

        auto filterList = StringUtil::replaceAll( getFileExtension(), ".", "" );
        auto defaultPath = getFilePath();

#ifdef FB_PLATFORM_WIN32
        auto path = StringUtil::replaceAll( defaultPath, "/", "\\" );
#else

        auto path = defaultPath;
#endif

        auto mode = getDialogMode();
        switch( mode )
        {
        case DialogMode::Select:
        {
            result = NFD_PickFolder( path.c_str(), &pOutPath );
        }
        break;
        case DialogMode::Open:
        {
            result = NFD_OpenDialog( filterList.c_str(), path.c_str(), &pOutPath );
        }
        break;
        case DialogMode::Save:
        {
            result = NFD_SaveDialog( filterList.c_str(), path.c_str(), &pOutPath );
        }
        break;
        }

        if( pOutPath )
        {
            auto outPath = StringUtil::cleanupPath( pOutPath );
            setFilePath( outPath );
        }

        return static_cast<INativeFileDialog::Result>( result );
    }

    String CNativeFileDialog::getFilePath() const
    {
        return m_filePath;
    }

    void CNativeFileDialog::setFilePath( const String &filePath )
    {
        m_filePath = filePath;
    }

    String CNativeFileDialog::getFileExtension() const
    {
        return m_fileExtension;
    }

    void CNativeFileDialog::setFileExtension( const String &fileExtension )
    {
        m_fileExtension = fileExtension;
    }

    INativeFileDialog::DialogMode CNativeFileDialog::getDialogMode() const
    {
        return m_dialogMode;
    }

    void CNativeFileDialog::setDialogMode( DialogMode mode )
    {
        m_dialogMode = mode;
    }

    INativeFileDialog::FilterMode CNativeFileDialog::getFilterMode() const
    {
        return m_filterMode;
    }

    void CNativeFileDialog::setFilterMode( FilterMode mode )
    {
        m_filterMode = mode;
    }
}  // namespace fb
