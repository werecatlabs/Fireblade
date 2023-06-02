#include <FBCore/FBCorePCH.h>
#include <FBCore/System/Plugin.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/StringUtil.h>

#if defined FB_PLATFORM_WIN32
#    include <windows.h>
#else
#    include <dlfcn.h>
#endif

namespace fb
{
    namespace core
    {
        Plugin::~Plugin()
        {
            unload( nullptr );
        }

        void Plugin::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loading );

            const auto filePath = getFilePath();

#if defined FB_PLATFORM_WIN32
            auto pFilePath = filePath.c_str();
            m_hMod = LoadLibraryW( pFilePath );
#elif defined FB_PLATFORM_APPLE
            auto filePathUTF8 = StringUtil::toUTF16to8( filePath );
            auto pFilePathUTF8 = filePathUTF8.c_str();
            auto mode = RTLD_LOCAL | RTLD_LAZY;
            m_hMod = dlopen( pFilePathUTF8, mode );
#elif defined FB_PLATFORM_LINUX
            auto filePathUTF8 = StringUtil::toUTF16to8( filePath );
            auto pFilePathUTF8 = filePathUTF8.c_str();
            auto mode = RTLD_LOCAL | RTLD_LAZY;
            m_hMod = dlopen( pFilePathUTF8, mode );
#endif

            if(!m_hMod)
            {
                const auto errorMessage =
                    "Unable to load library: " + StringUtil::toUTF16to8( filePath );
                FB_LOG_ERROR( errorMessage );
            }

            setLoadingState( LoadingState::Loaded );
        }

        void Plugin::reload( SmartPtr<ISharedObject> data )
        {
            unload( data );
            reload( data );
        }

        void Plugin::unload( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Unloading );

#if defined FB_PLATFORM_WIN32
            if(m_hMod)
            {
                FreeLibrary( m_hMod );
                m_hMod = nullptr;
            }
#else
            if( m_hMod )
            {
                dlclose( m_hMod );
                m_hMod = 0;
            }
#endif

            setLoadingState( LoadingState::Unloaded );
        }

        LibraryHandle Plugin::getLibraryHandle() const
        {
            return m_hMod;
        }

        void Plugin::setLibraryHandle( LibraryHandle handle )
        {
            m_hMod = handle;
        }

        LibraryFunction Plugin::getFunction( const String &name ) const
        {
            auto handle = getLibraryHandle();

#if defined FB_PLATFORM_WIN32
            auto address = GetProcAddress( handle, name.c_str() );
#else
            auto address = dlsym( handle, name.c_str() );
#endif

            return address;
        }

        StringW Plugin::getFilePath() const
        {
            return m_filename;
        }

        void Plugin::setFilePath( const StringW &fileName )
        {
            m_filename = fileName;
        }
    } // namespace core
}     // namespace fb
