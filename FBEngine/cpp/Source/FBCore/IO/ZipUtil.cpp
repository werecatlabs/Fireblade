#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/ZipUtil.h>
#include <FBCore/IO/FileDataStream.h>

#if defined FB_PLATFORM_WIN32
#    if FB_USE_ZZIP
#        include <FBCore/Core/Path.h>
#        include <FBCore/Core/StringUtil.h>
#        include <zzip/zzip.h>
#        include <zzip/plugin.h>
#        include <zip.h>
#        include <iostream>
#        include <fstream>
#        include <winerror.h>

namespace fb
{
    StringW currentZipFilePath;
    SmartPtr<IStream> currentZipFile;

    int zzip_custom_open( zzip_char_t *name, int flags, ... )
    {
        auto stream = new std::fstream( currentZipFilePath, std::fstream::in | std::fstream::binary );

        if( !stream->is_open() )
        {
            std::cout << "File open error." << std::endl;
        }

        auto dataStream = SmartPtr<IStream>( new FileDataStream( stream, true ) );
        dataStream->seek( 0 );
        currentZipFile = dataStream;
        return 72;  // StringUtilW::getHash(currentZipFilePath);
    }

    int zzip_custom_close( int fd )
    {
        currentZipFile->close();
        currentZipFile = nullptr;
        return 0;
    }

    zzip_ssize_t zzip_custom_read( int fd, void *buf, zzip_size_t len )
    {
        auto dataStream = currentZipFile;
        auto bytesRead = dataStream->read( buf, len );
        FB_ASSERT( bytesRead == len );
        return static_cast<zzip_ssize_t>( bytesRead );
    }

    zzip_off_t zzip_custom_seeks( int fd, zzip_off_t offset, int whence )
    {
        zzip_size_t newPos = -1;

        switch( whence )
        {
        case SEEK_CUR:
        {
            newPos = currentZipFile->tell() + offset;
            currentZipFile->skip( offset );
        }
        break;
        case SEEK_END:
        {
            newPos = currentZipFile->size() - offset;
            currentZipFile->seek( newPos );
        }
        break;
        case SEEK_SET:
        {
            newPos = static_cast<zzip_size_t>( offset );
            currentZipFile->seek( offset );
        }
        break;
        default:
            // bad whence - return an error - nonzero value.
            return -1;
            break;
        }

        if( newPos >= currentZipFile->size() )
        {
            // bad whence - return an error - nonzero value.
            return -1;
        }

        return static_cast<zzip_off_t>( currentZipFile->tell() );
    }

    zzip_off_t zzip_custom_filesize( int fd )
    {
        size_t size = currentZipFile->size();
        return static_cast<zzip_off_t>( size );
    }

    
    zzip_ssize_t zzip_custom_write( int fd, _zzip_const void *buf, zzip_size_t len )
    {
        // the files in this case are read only - return an error  - nonzero value.
        return -1;
    }

    void ZipUtil::extractZip( const StringW &src, const StringW &dst )
    {
#        if defined FB_PLATFORM_WIN32
        currentZipFilePath = src;

        zzip_plugin_io_handlers pluginIo;
        zzip_init_io( &pluginIo, 0 );

        pluginIo.fd.open = zzip_custom_open;
        pluginIo.fd.close = zzip_custom_close;
        pluginIo.fd.read = zzip_custom_read;
        pluginIo.fd.seeks = zzip_custom_seeks;
        pluginIo.fd.filesize = zzip_custom_filesize;
        pluginIo.fd.write = zzip_custom_write;
        // pluginIo.fd.sys = 1;
        // pluginIo.fd.type = 1;

        std::string srcPath = StringUtil::toStringC( src );

        zzip_error_t zzipError;
        ZZIP_DIR *dir = zzip_dir_open_ext_io( srcPath.c_str(), &zzipError, nullptr, &pluginIo );
        if( dir )
        {
            int fileCount = 0;
            ZZIP_DIRENT dircount;
            while( zzip_dir_read( dir, &dircount ) )
            {
                ++fileCount;
            }

            zzip_rewinddir( dir );

            int fileIdx = 0;

            ZZIP_DIRENT dirent;
            while( zzip_dir_read( dir, &dirent ) )
            {
                // if ( listener )
                //{
                //	listener->setProgress((float)fileIdx / (float)fileCount);
                // }

                /* show info for first file */
                // print("%s %i/%i", dirent.d_name, dirent.d_csize, dirent.st_size);

                ZZIP_FILE *fp = zzip_file_open( dir, dirent.d_name, 0 );
                if( fp )
                {
                    if( dirent.st_size > 0 )
                    {
                        auto buff = new char[dirent.st_size];
                        zzip_file_read( fp, buff, dirent.st_size );

                        StringW newFilePath = dst + StringUtil::toStringW( dirent.d_name );
                        StringW newPath = PathW::getFilePath( newFilePath );
                        PathW::createDirectories( newPath );

                        std::fstream newFile( newFilePath, std::ios::out | std::ios::binary );
                        newFile.write( buff, dirent.st_size );

                        delete[] buff;
                    }
                    else if( dirent.st_size == 0 )
                    {
                        int stop = 0;
                        stop = 0;

                        // StringW newPath = systemSettings->getUserDataFolder() + L"/Temp/" +
                        // std::string(dirent.d_name); FileSystem::createDirectories(newPath);
                    }

                    zzip_file_close( fp );
                }
                else
                {
                    std::string errordesc = zzip_strerror_of( dir );

                    int stop = 0;
                    stop = 0;
                }

                ++fileIdx;
            }

            zzip_dir_close( dir );
        }
        else
        {
            int stop = 0;
            stop = 0;
        }
#        endif
    }

    int ZipUtil::createZipFileFromPath( const StringW &destinationPath, const Array<StringW> &paths )
    {
#        if defined FB_PLATFORM_WIN32
        zipFile zf = zipOpen( std::string( destinationPath.begin(), destinationPath.end() ).c_str(),
                              APPEND_STATUS_CREATE );
        if( zf == nullptr )
            return 1;

        bool _return = true;
        for( size_t i = 0; i < paths.size(); i++ )
        {
            StringW path = paths[i];

            std::fstream file( path, std::ios::binary | std::ios::in );
            if( file.is_open() )
            {
                file.seekg( 0, std::ios::end );
                auto size = file.tellg();
                file.seekg( 0, std::ios::beg );

                Array<char> buffer( size );
                if( size == 0 || file.read( &buffer[0], size ) )
                {
                    zip_fileinfo zfi = { 0 };
                    StringW fileName = path.substr( path.rfind( '\\' ) + 1 );
                    fileName = PathW::getFileName( fileName );

                    if( S_OK == zipOpenNewFileInZip(
                                    zf, std::string( fileName.begin(), fileName.end() ).c_str(), &zfi,
                                    nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_BEST_COMPRESSION ) )
                    {
                        if( zipWriteInFileInZip( zf, size == 0 ? "" : &buffer[0], size ) )
                            _return = false;

                        if( zipCloseFileInZip( zf ) )
                            _return = false;

                        file.close();
                        continue;
                    }
                }
                file.close();
            }
            _return = false;
        }

        if( zipClose( zf, nullptr ) )
            return 3;

        if( !_return )
            return 4;

        return S_OK;
#        else
        return 0;
#        endif
    }

    int ZipUtil::createZipFileFromPath( const StringW &destinationPath, const StringW &zipPath,
                                        const Array<StringW> &paths )
    {
        std::string destinationPathCStr = StringUtil::toStringC( destinationPath );
        zipFile zf = zipOpen( destinationPathCStr.c_str(), APPEND_STATUS_CREATE );
        if( zf == nullptr )
        {
            return 1;
        }

        bool _return = true;
        for( size_t i = 0; i < paths.size(); i++ )
        {
            StringW path = paths[i];

            std::fstream file( path, std::ios::binary | std::ios::in );
            if( file.is_open() )
            {
                file.seekg( 0, std::ios::end );
                auto size = file.tellg();
                file.seekg( 0, std::ios::beg );

                Array<char> buffer( size );
                if( size == 0 || file.read( &buffer[0], size ) )
                {
                    zip_fileinfo zfi = { 0 };
                    StringW fileName = path.substr( path.rfind( '\\' ) + 1 );
                    StringW zipFilePath = PathW::getRelativePath( zipPath, path );
                    std::string zipFilePathCStr = StringUtil::toStringC( zipFilePath );

                    if( S_OK == zipOpenNewFileInZip( zf, zipFilePathCStr.c_str(), &zfi, nullptr, 0,
                                                     nullptr, 0, nullptr, Z_DEFLATED,
                                                     Z_BEST_COMPRESSION ) )
                    {
                        if( zipWriteInFileInZip( zf, size == 0 ? "" : &buffer[0], size ) )
                        {
                            _return = false;
                        }

                        if( zipCloseFileInZip( zf ) )
                        {
                            _return = false;
                        }

                        continue;
                    }
                }
            }

            _return = false;
        }

        if( zipClose( zf, nullptr ) )
        {
            return 3;
        }

        if( !_return )
        {
            return 4;
        }

        return S_OK;
    }

    int ZipUtil::createObfuscatedZipFileFromPath( const StringW &destinationPath,
                                                  const Array<StringW> &paths )
    {
        auto destinationTempPath = destinationPath + L".tempzip";
        createZipFileFromPath( destinationTempPath, paths );

        /* Convert the file */
        FILE *fin = _wfopen( destinationTempPath.c_str(), L"rb" );

        if( !fin )
        {
            fprintf( stderr, "Can't open input file \"%s\"\n", destinationTempPath.c_str() );
            exit( 1 );
        }

        FILE *fout = _wfopen( destinationPath.c_str(), L"wb" );

        if( !fout )
        {
            fprintf( stderr, "Can't open output file \"%s\"\n", destinationPath.c_str() );
            exit( 1 );
        }

        int ch;

        while( ( ch = fgetc( fin ) ) != EOF )
        {
            ch ^= 13;  //ApplicationTypes::XOR_VALUE;
            fputc( ch, fout );
        }

        fclose( fout );
        fclose( fin );

        return 0;
    }

    int ZipUtil::createObfuscatedZipFileFromPath( const StringW &destinationPath, const StringW &zipPath,
                                                  const Array<StringW> &paths )
    {
        StringW destinationTempPath = destinationPath + L".tempzip";
        createZipFileFromPath( destinationTempPath, zipPath, paths );

        /* Convert the file */
        FILE *fin = _wfopen( destinationTempPath.c_str(), L"rb" );

        if( !fin )
        {
            fprintf( stderr, "Can't open input file \"%s\"\n", destinationTempPath.c_str() );
            exit( 1 );
        }

        FILE *fout = _wfopen( destinationPath.c_str(), L"wb" );

        if( !fout )
        {
            fprintf( stderr, "Can't open output file \"%s\"\n", destinationPath.c_str() );
            exit( 1 );
        }

        int ch;

        while( ( ch = fgetc( fin ) ) != EOF )
        {
            ch ^= 13;  //ApplicationTypes::XOR_VALUE;
            fputc( ch, fout );
        }

        fclose( fout );
        fclose( fin );

        return 0;
    }

#        define CHUNK 16384

    /* Compress from file source to file dest until EOF on source.
       def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
       allocated for processing, Z_STREAM_ERROR if an invalid compression
       level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
       version of the library linked do not match, or Z_ERRNO if there is
       an error reading or writing the files. */
    int ZipUtil::def( FILE *source, FILE *dest, int level )
    {
        int ret, flush;
        unsigned have;
        z_stream strm;
        unsigned char in[CHUNK];
        unsigned char out[CHUNK];

        /* allocate deflate state */
        strm.zalloc = nullptr;
        strm.zfree = nullptr;
        strm.opaque = nullptr;
        ret = deflateInit( &strm, level );
        if( ret != Z_OK )
            return ret;

        /* compress until end of file */
        do
        {
            strm.avail_in = fread( in, 1, CHUNK, source );
            if( ferror( source ) )
            {
                (void)deflateEnd( &strm );
                return Z_ERRNO;
            }
            flush = feof( source ) ? Z_FINISH : Z_NO_FLUSH;
            strm.next_in = in;

            /* run deflate() on input until output buffer not full, finish
               compression if all of source has been read in */
            do
            {
                strm.avail_out = CHUNK;
                strm.next_out = out;
                ret = deflate( &strm, flush );   /* no bad return value */
                assert( ret != Z_STREAM_ERROR ); /* state not clobbered */
                have = CHUNK - strm.avail_out;
                if( fwrite( out, 1, have, dest ) != have || ferror( dest ) )
                {
                    (void)deflateEnd( &strm );
                    return Z_ERRNO;
                }
            } while( strm.avail_out == 0 );
            assert( strm.avail_in == 0 ); /* all input will be used */

            /* done when last data in file processed */
        } while( flush != Z_FINISH );
        assert( ret == Z_STREAM_END ); /* stream will be complete */

        /* clean up and return */
        (void)deflateEnd( &strm );
        return Z_OK;
    }

    /* Decompress from file source to file dest until stream ends or EOF.
       inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
       allocated for processing, Z_DATA_ERROR if the deflate data is
       invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
       the version of the library linked do not match, or Z_ERRNO if there
       is an error reading or writing the files. */
    int ZipUtil::inf( FILE *source, FILE *dest )
    {
        int ret;
        unsigned have;
        z_stream strm;
        auto in = new unsigned char[CHUNK];
        auto out = new unsigned char[CHUNK];

        /* allocate inflate state */
        strm.zalloc = nullptr;
        strm.zfree = nullptr;
        strm.opaque = nullptr;
        strm.avail_in = 0;
        strm.next_in = nullptr;
        ret = inflateInit( &strm );
        if( ret != Z_OK )
            return ret;

        /* decompress until deflate stream ends or end of file */
        do
        {
            strm.avail_in = fread( in, 1, CHUNK, source );
            if( ferror( source ) )
            {
                (void)inflateEnd( &strm );
                return Z_ERRNO;
            }
            if( strm.avail_in == 0 )
                break;
            strm.next_in = in;

            /* run inflate() on input until output buffer not full */
            do
            {
                strm.avail_out = CHUNK;
                strm.next_out = out;
                ret = inflate( &strm, Z_NO_FLUSH );
                assert( ret != Z_STREAM_ERROR ); /* state not clobbered */
                switch( ret )
                {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR; /* and fall through */
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd( &strm );
                    return ret;
                }
                have = CHUNK - strm.avail_out;
                if( fwrite( out, 1, have, dest ) != have || ferror( dest ) )
                {
                    (void)inflateEnd( &strm );
                    return Z_ERRNO;
                }
            } while( strm.avail_out == 0 );

            /* done when inflate() says it's done */
        } while( ret != Z_STREAM_END );

        /* clean up and return */
        (void)inflateEnd( &strm );

        delete[] in;
        delete[] out;

        return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
    }
}  // end namespace fb
#    else
namespace fb
{
    void ZipUtil::extractZip( const StringW &src, const StringW &dst )
    {
    }

    s32 ZipUtil::createZipFileFromPath( const StringW &destinationPath, const Array<StringW> &paths )
    {
        return 0;
    }

    s32 ZipUtil::createZipFileFromPath( const StringW &destinationPath, const StringW &zipPath,
                                        const Array<StringW> &paths )
    {
        return 0;
    }

    s32 ZipUtil::createObfuscatedZipFileFromPath( const StringW &destinationPath,
                                                  const Array<StringW> &paths )
    {
        return 0;
    }

    s32 ZipUtil::createObfuscatedZipFileFromPath( const StringW &destinationPath, const StringW &zipPath,
                                                  const Array<StringW> &paths )
    {
        return 0;
    }

    s32 ZipUtil::def( FILE *source, FILE *dest, int level )
    {
        return 0;
    }

    s32 ZipUtil::inf( FILE *source, FILE *dest )
    {
        return 0;
    }
}  // namespace fb

#    endif
#endif
