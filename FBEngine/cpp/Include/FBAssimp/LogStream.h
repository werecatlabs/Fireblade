#ifndef LogStream_h__
#define LogStream_h__

#include <FBAssimp/FBAssimpPrerequisites.h>
#include <assimp/DefaultLogger.hpp>

namespace fb
{

    class LogStream : public Assimp::LogStream
    {
    public:
        LogStream() = default;

        void write( const char *message );
    };

}  // namespace fb

#endif  // LogStream_h__
