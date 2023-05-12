#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/ColourUtil.h>

namespace fb
{

    u16 ColourUtil::RGBA16( u32 r, u32 g, u32 b, u32 a )
    {
        return ( ( a & 0x80 ) << 8 | ( r & 0xF8 ) << 7 | ( g & 0xF8 ) << 2 | ( b & 0xF8 ) >> 3 );
    }

    u16 ColourUtil::RGB16( u32 r, u32 g, u32 b )
    {
        return RGBA16( r, g, b, 0xFF );
    }

    u16 ColourUtil::RGB16from16( u16 r, u16 g, u16 b )
    {
        return ( r & 0x1F ) << 10 | ( g & 0x1F ) << 5 | ( b & 0x1F );
    }

    u16 ColourUtil::X8R8G8B8toA1R5G5B5( u32 color )
    {
        return static_cast<u16>( 0x8000 | ( color & 0x00F80000 ) >> 9 | ( color & 0x0000F800 ) >> 6 |
                                 ( color & 0x000000F8 ) >> 3 );
    }

    u16 ColourUtil::A8R8G8B8toA1R5G5B5( u32 color )
    {
        return static_cast<u16>( ( color & 0x80000000 ) >> 16 | ( color & 0x00F80000 ) >> 9 |
                                 ( color & 0x0000F800 ) >> 6 | ( color & 0x000000F8 ) >> 3 );
    }

    u32 ColourUtil::A1R5G5B5toA8R8G8B8( u32 color )
    {
        return ( ( ( -( static_cast<s32>( color ) & 0x00008000 ) >> 31 ) & 0xFF000000 ) |
                 ( ( color & 0x00007C00 ) << 9 ) | ( ( color & 0x00007000 ) << 4 ) |
                 ( ( color & 0x000003E0 ) << 6 ) | ( ( color & 0x00000380 ) << 1 ) |
                 ( ( color & 0x0000001F ) << 3 ) | ( ( color & 0x0000001C ) >> 2 ) );
    }

    u16 ColourUtil::A8R8G8B8toR5G6B5( u32 color )
    {
        return static_cast<u16>( ( color & 0x00F80000 ) >> 8 | ( color & 0x0000FC00 ) >> 5 |
                                 ( color & 0x000000F8 ) >> 3 );
    }

    u32 ColourUtil::R5G6B5toA8R8G8B8( u16 color )
    {
        return 0xFF000000 | ( ( color & 0xF800 ) << 8 ) | ( ( color & 0x07E0 ) << 5 ) |
               ( ( color & 0x001F ) << 3 );
    }

    u16 ColourUtil::R5G6B5toA1R5G5B5( u16 color )
    {
        return 0x8000 | ( ( ( color & 0xFFC0 ) >> 1 ) | ( color & 0x1F ) );
    }

    u16 ColourUtil::A1R5G5B5toR5G6B5( u16 color )
    {
        return ( ( ( color & 0x7FE0 ) << 1 ) | ( color & 0x1F ) );
    }

    u32 ColourUtil::getAlpha( u16 color )
    {
        return ( ( color >> 15 ) & 0x1 );
    }

    u32 ColourUtil::getRed( u16 color )
    {
        return ( ( color >> 10 ) & 0x1F );
    }

    s32 ColourUtil::getAverage( s16 color )
    {
        return ( ( getRed( color ) << 3 ) + ( getGreen( color ) << 3 ) + ( getBlue( color ) << 3 ) ) / 3;
    }

    s32 ColourUtil::getBlueSigned( u16 color )
    {
        return ( color & 0x1F );
    }

    s32 ColourUtil::getGreenSigned( u16 color )
    {
        return ( ( color >> 5 ) & 0x1F );
    }

    s32 ColourUtil::getRedSigned( u16 color )
    {
        return ( ( color >> 10 ) & 0x1F );
    }

    u32 ColourUtil::getBlue( u16 color )
    {
        return ( color & 0x1F );
    }

    u32 ColourUtil::getGreen( u16 color )
    {
        return ( ( color >> 5 ) & 0x1F );
    }
}  // end namespace fb
