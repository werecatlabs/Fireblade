#ifndef FB_ByteSwap_h__
#define FB_ByteSwap_h__

#include <FBCore/FBCoreTypes.h>

namespace fb
{
    class Byteswap
    {
    public:
        static u16 byteswap( u16 num );
        static s16 byteswap( s16 num );
        static u32 byteswap( u32 num );
        static s32 byteswap( s32 num );
        static f32 byteswap( f32 num );

        // prevent accidental swapping of chars
        static u8 byteswap( u8 num );
        static c8 byteswap( c8 num );
    };
}  // end namespace fb

#endif  // ByteSwap_h__
