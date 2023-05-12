#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/StringData.h"
#include "FBCore/Base/StringUtil.h"

extern "C" {
}

namespace fb
{
    StringData MakeSaracenString()
    {
        StringData str;
        memset( str.data, 0, sizeof( str.data ) );
        return str;
    }

    StringData MakeSaracenString( const std::string &str )
    {
        StringData strdata;
        StringUtil::toBuffer( str, strdata.data, StringData::STR_MAX_LENGTH );
        return strdata;
    }
}  // namespace fb
