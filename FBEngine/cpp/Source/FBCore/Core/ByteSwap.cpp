#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/ByteSwap.h>

#ifdef FB_PLATFORM_IOS
#    define bswap_16( X ) X
#    define bswap_32( X ) X
#else
#    define bswap_16( X ) X
#    define bswap_32( X ) X
#endif

namespace fb
{
    //-------------------------------------------------
    u16 Byteswap::byteswap( u16 num )
    {
        return bswap_16( num );
    }

    //-------------------------------------------------
    s16 Byteswap::byteswap( s16 num )
    {
        return bswap_16( num );
    }

    //-------------------------------------------------
    u32 Byteswap::byteswap( u32 num )
    {
        return bswap_32( num );
    }

    //-------------------------------------------------
    s32 Byteswap::byteswap( s32 num )
    {
        return bswap_32( num );
    }

    //-------------------------------------------------
    f32 Byteswap::byteswap( f32 num )
    {
        float retVal;
        auto floatToConvert = (char *)&num;
        auto returnFloat = (char *)&retVal;

        // swap the bytes into a temporary buffer
        returnFloat[0] = floatToConvert[3];
        returnFloat[1] = floatToConvert[2];
        returnFloat[2] = floatToConvert[1];
        returnFloat[3] = floatToConvert[0];

        return retVal;
    }

    //-------------------------------------------------
    u8 Byteswap::byteswap( u8 num )
    {
        return num;
    }

    //-------------------------------------------------
    c8 Byteswap::byteswap( c8 num )
    {
        return num;
    }
}  // end namespace fb
