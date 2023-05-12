#ifndef SaracenString_h__
#define SaracenString_h__

#include <FBCore/FBCorePrerequisites.h>
#include <string>

namespace fb
{
    class StringData
    {
    public:
        static constexpr int STR_MAX_LENGTH = 4096;
        char data[STR_MAX_LENGTH];
    };
}  // namespace fb

namespace fb
{
    StringData MakeSaracenString();
    StringData MakeSaracenString( const std::string &str );
}  // namespace fb

#endif  // SaracenString_h__
