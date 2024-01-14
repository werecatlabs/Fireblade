#ifndef ZipFile_h__
#define ZipFile_h__

#include <FBCore/IO/DataStream.h>

namespace fb
{

    class ZipFile : public DataStream
    {
    public:
        ZipFile() = default;
        ZipFile( ZipArchive *archive, const String &fileName, ZZIP_FILE *zzipFile,
                 u32 uncompressedSize );
        ~ZipFile() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        bool isOpen() const override;

        void close() override;

        bool eof( void ) const override;

        size_t read( void *buffer, size_t sizeToRead ) override;
        size_t write( const void *buffer, size_t sizeToWrite ) override;

        bool seek( size_t finalPos ) override;

        void skip( long count ) override;

        size_t size() const override;

        size_t tell() const override;

        void setFreeMemory( bool freeMemory ) override;

        ZZIP_FILE *getZipFile() const;
        void setZipFile( ZZIP_FILE *zipFile );

        ZipArchive *getArchive() const;
        void setArchive( ZipArchive *archive );

        void setSize( size_t iSize );

        bool isReadable() const override;

        bool isWriteable() const override;

        bool isValid() const;

        FB_CLASS_REGISTER_DECL;

    protected:
        ZZIP_FILE *m_zipFile = nullptr;
        ZipArchive *m_archive = nullptr;
    };
}  // end namespace fb

#endif  // ObfuscatedZipFile_h__
