#ifndef __ZipUtil_h__
#define __ZipUtil_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>

#if defined( MSDOS ) || defined( OS2 ) || defined( WIN32 ) || defined( __CYGWIN__ )
#    include <fcntl.h>
#    include <io.h>
#    define SET_BINARY_MODE( file ) setmode( fileno( file ), O_BINARY )
#else
#    define SET_BINARY_MODE( file )
#endif

namespace fb
{
    class ZipUtil
    {
    public:
        static void extractZip( const StringW &src, const StringW &dst );

        static s32 createZipFileFromPath( const StringW &destinationPath, const Array<StringW> &paths );
        static s32 createZipFileFromPath( const StringW &destinationPath, const StringW &zipPath,
                                          const Array<StringW> &paths );

        static s32 createObfuscatedZipFileFromPath( const StringW &destinationPath,
                                                    const Array<StringW> &paths );
        static s32 createObfuscatedZipFileFromPath( const StringW &destinationPath,
                                                    const StringW &zipPath,
                                                    const Array<StringW> &paths );
        static s32 def( FILE *source, FILE *dest, int level );
        static s32 inf( FILE *source, FILE *dest );
    };
}  // namespace fb

#endif  // ZipUtil_h__
