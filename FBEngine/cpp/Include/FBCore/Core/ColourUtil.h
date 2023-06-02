#ifndef ColourUtil_h__
#define ColourUtil_h__

#include <FBCore/FBCoreTypes.h>

namespace fb
{
    class ColourUtil
    {
    public:
        //! Creates a 16 bit A1R5G5B5 color
        static u16 RGBA16( u32 r, u32 g, u32 b, u32 a );

        //! Creates a 16 bit A1R5G5B5 color
        static u16 RGB16( u32 r, u32 g, u32 b );

        //! Creates a 16 bit A1R5G5B5 color, based on 16 bit input values
        static u16 RGB16from16( u16 r, u16 g, u16 b );

        //! Converts a 32 bit (X8R8G8B8) color to a 16 A1R5G5B5 color
        static u16 X8R8G8B8toA1R5G5B5( u32 color );

        //! Converts a 32 bit (A8R8G8B8) color to a 16 A1R5G5B5 color
        static u16 A8R8G8B8toA1R5G5B5( u32 color );

        //! Converts a 32 bit (A8R8G8B8) color to a 16 R5G6B5 color
        static u16 A8R8G8B8toR5G6B5( u32 color );

        //! Returns A8R8G8B8 Color from A1R5G5B5 color
        //! build a nicer 32 Bit Color by extending dest lower bits with source high bits
        static u32 A1R5G5B5toA8R8G8B8( u32 color );

        //! Returns A8R8G8B8 Color from R5G6B5 color
        static u32 R5G6B5toA8R8G8B8( u16 color );

        //! Returns A1R5G5B5 Color from R5G6B5 color
        static u16 R5G6B5toA1R5G5B5( u16 color );

        //! Returns R5G6B5 Color from A1R5G5B5 color
        static u16 A1R5G5B5toR5G6B5( u16 color );

        //! Returns the alpha component from A1R5G5B5 color
        static u32 getAlpha( u16 color );

        //! Returns the red component from A1R5G5B5 color.
        //! Shift left by 3 to get 8 bit value.
        static u32 getRed( u16 color );

        //! Returns the green component from A1R5G5B5 color
        //! Shift left by 3 to get 8 bit value.
        static u32 getGreen( u16 color );

        //! Returns the blue component from A1R5G5B5 color
        //! Shift left by 3 to get 8 bit value.
        static u32 getBlue( u16 color );

        //! Returns the red component from A1R5G5B5 color.
        //! Shift left by 3 to get 8 bit value.
        static s32 getRedSigned( u16 color );

        //! Returns the green component from A1R5G5B5 color
        //! Shift left by 3 to get 8 bit value.
        static s32 getGreenSigned( u16 color );

        //! Returns the blue component from A1R5G5B5 color
        //! Shift left by 3 to get 8 bit value.
        static s32 getBlueSigned( u16 color );

        //! Returns the average from a 16 bit A1R5G5B5 color
        static s32 getAverage( s16 color );
    };
}  // end namespace fb

#endif  // ColourUtil_h__
