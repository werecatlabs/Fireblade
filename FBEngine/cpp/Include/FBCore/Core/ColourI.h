#ifndef ColourI_h__
#define ColourI_h__

#include <FBCore/FBCoreTypes.h>

namespace fb
{
    /** Integer colour value. */
    class FBCore_API ColourI
    {
    public:
        /** Default constructor. */
        ColourI();

        /** Constructor to pass in colour values. */
        ColourI( u32 a, u32 r, u32 g, u32 b );

        /** Constructor to pass in colour value. */
        explicit ColourI( u32 clr );

        /** Returns the alpha component of the color. The alpha component
        defines how transparent a color should be. */
        u32 getAlpha() const;

        /** Returns the red component of the color.
        @return Specifying how red the color is. */
        u32 getRed() const;

        /** Returns the green component of the color.
        @return Specifying how green the color is. */
        u32 getGreen() const;

        /** Returns the value of the blue component.
        @return Specifying how blue the color is. */
        u32 getBlue() const;

        /** Returns the luminance. */
        f32 getLuminance() const;

        /** Returns the average intensity. */
        u32 getAverage() const;

        /** Sets the alpha component. The alpha component
        defines how transparent a color should be.
        @param a: Has to be a value. */
        void setAlpha( u32 a );

        /** Sets the red component.
        @param r: Has to be a value. */
        void setRed( u32 r );

        /** Sets the green component.
        @param g: Has to be a value. */
        void setGreen( u32 g );

        /** Sets the blue component.
        @param b: Has to be a value. */
        void setBlue( u32 b );

        /** Calculates a 16 bit A1R5G5B5 value of this color.
        @return Returns the 16 bit A1R5G5B5 value of this color. */
        u16 toA1R5G5B5() const;

        /** Converts color to OpenGL color format,
        from ARGB to RGBA in 4 byte components for endian aware
        passing to OpenGL.
        @param dest: address where the 4x8 bit OpenGL color is stored. */
        void toOpenGLColor( u8 *dest ) const;

        /** Sets all four components of the color at once. */
        void set( u32 a, u32 r, u32 g, u32 b );
        void set( u32 col );

        /** Compares the color to another color.
        @return Returns true if the colors are the same. */
        bool operator==( const ColourI &other ) const;

        /** Compares the color to another color.
        @return Returns true if the colors are different. */
        bool operator!=( const ColourI &other ) const;

        /** Adds two lights. */
        ColourI operator+( const ColourI &other ) const;

        /** Interpolates the color. */
        ColourI getInterpolated( const ColourI &other, f32 d ) const;

        /** Returns interpolated color. ( quadratic )
        @param c1: first color to interpolate with
        @param c2: second color to interpolate with
        @param d: value between 0.0f and 1.0f. */
        ColourI getInterpolated_quadratic( const ColourI &c1, const ColourI &c2, f32 d ) const;

    private:
        /** color in A8R8G8B8 format */
        u32 m_color = 0;
    };
}  // end namespace fb

#endif  // ColourI_h__
