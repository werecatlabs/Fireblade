#include "Types.h"
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    const hash_type Types::INPUT_UP = StringUtil::getHashMakeLower( "up" );
    const hash_type Types::INPUT_DOWN = StringUtil::getHashMakeLower( "down" );
    const hash_type Types::INPUT_RIGHT = StringUtil::getHashMakeLower( "right" );
    const hash_type Types::INPUT_LEFT = StringUtil::getHashMakeLower( "left" );
} // namespace fb
