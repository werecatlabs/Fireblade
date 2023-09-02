#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/LogStream.h>
#include <FBCore/FBCore.h>

namespace fb
{

    void LogStream::write( const char *message )
    {
        String msg( message );
        StringUtil::trim( msg );
        FB_LOG( "Assimp: " + msg );
    }

}  // namespace fb
