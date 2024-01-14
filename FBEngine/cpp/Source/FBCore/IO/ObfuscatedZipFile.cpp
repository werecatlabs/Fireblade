#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/ObfuscatedZipFile.h>
#include <FBCore/IO/ObfuscatedZipArchive.h>
#include <FBCore/FBCore.h>
#include <zzip/zzip.h>
#include <zzip/plugin.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ObfuscatedZipFile, DataStream );

    ObfuscatedZipFile::ObfuscatedZipFile( ObfuscatedZipArchive *archive, const String &fileName,
                                          ZZIP_FILE *zzipFile, u32 uncompressedSize ) :
        m_zipFile( zzipFile ),
        m_archive( archive )
    {
        setFileName( fileName );

        m_size = uncompressedSize;
        zzip_seek( m_zipFile, 0, SEEK_SET );
    }

    ObfuscatedZipFile::~ObfuscatedZipFile()
    {
        unload( nullptr );
    }

    void ObfuscatedZipFile::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );
        zzip_seek( m_zipFile, 0, SEEK_SET );
        setLoadingState( LoadingState::Loaded );
    }

    void ObfuscatedZipFile::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );
        close();
        setLoadingState( LoadingState::Unloaded );
    }

    auto ObfuscatedZipFile::eof() const -> bool
    {
        auto pos = tell();
        return pos >= m_size;
    }

    auto ObfuscatedZipFile::read( void *buffer, size_t sizeToRead ) -> size_t
    {
        // RecursiveMutex::ScopedLock lock(reader->Mutex);
        zzip_ssize_t r = zzip_file_read( m_zipFile, buffer, sizeToRead );
        if( r < 0 )
        {
            ZZIP_DIR *dir = zzip_dirhandle( m_zipFile );

            auto fileName = getFileName();
            auto msg = zzip_strerror_of( dir );
            auto exceptionMsg = fileName + String( " - error from zziplib: " ) + msg +
                                String( "ObfuscatedZipDataStream::read" );
            FB_EXCEPTION( exceptionMsg.c_str() );
        }

        return r;
    }

    auto ObfuscatedZipFile::write( [[maybe_unused]] const void *buffer,
                                   [[maybe_unused]] size_t sizeToWrite ) -> size_t
    {
        return 0;
    }

    auto ObfuscatedZipFile::seek( size_t finalPos ) -> bool
    {
        zzip_seek( m_zipFile, static_cast<zzip_off_t>( finalPos ), SEEK_SET );
        return true;
    }

    void ObfuscatedZipFile::skip( long count )
    {
        if( count > 0 )
        {
            zzip_seek( m_zipFile, count, SEEK_CUR );
        }
        else if( count < 0 )
        {
            zzip_seek( m_zipFile, count, SEEK_CUR );
        }
    }

    auto ObfuscatedZipFile::size() const -> size_t
    {
        return m_size;
    }

    auto ObfuscatedZipFile::tell() const -> size_t
    {
        return static_cast<size_t>( zzip_tell( m_zipFile ) );
    }

    auto ObfuscatedZipFile::isOpen() const -> bool
    {
        return m_zipFile != nullptr;
    }

    void ObfuscatedZipFile::close()
    {
        if( m_zipFile != nullptr )
        {
            zzip_file_close( m_zipFile );
            m_zipFile = nullptr;
        }
    }

    void ObfuscatedZipFile::setFreeMemory( [[maybe_unused]] bool freeMemory )
    {
    }

    auto ObfuscatedZipFile::isReadable() const -> bool
    {
        return true;
    }

    auto ObfuscatedZipFile::isWriteable() const -> bool
    {
        return false;
    }

    auto ObfuscatedZipFile::isValid() const -> bool
    {
        return isOpen() && ( size() > 0 ) && ( size() < static_cast<size_t>( 2e+9 ) );
    }

}  // end namespace fb
