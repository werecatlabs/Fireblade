#ifndef ColourF_h__
#define ColourF_h__

#include <FBCore/Core/ColourI.h>

namespace fb
{
    using RGBA = u32;
    using ARGB = u32;
    using ABGR = u32;
    using BGRA = u32;

    //! Class representing a color with four floats.
    class ColourF
    {
    public:
        //! Constructs a color. All values are initialised with 0.0f, resulting
        //! in a black color.
        ColourF() = default;

        //! Constructs a color from three color values: red, green and blue.
        //! \param r: Red color component. Should be a value between 0.0f meaning
        //! no red (=black) and 1.0f, meaning full red.
        //! \param g: Green color component. Should be a value between 0.0f meaning
        //! no green (=black) and 1.0f, meaning full green.
        //! \param b: Blue color component. Should be a value between 0.0f meaning
        //! no blue (=black) and 1.0f, meaning full blue.
        ColourF( f32 r, f32 g, f32 b );

        //! Constructs a color from four color values: red, green, blue, and alpha.
        //! \param r: Red color component. Should be a value between 0.0f meaning
        //! no red (=black) and 1.0f, meaning full red.
        //! \param g: Green color component. Should be a value between 0.0f meaning
        //! no green (=black) and 1.0f, meaning full green.
        //! \param b: Blue color component. Should be a value between 0.0f meaning
        //! no blue (=black) and 1.0f, meaning full blue.
        //! \param a: Alpha color component of the color.
        //! The alpha component defines how transparent a color should be.
        //! Has to be a value between 0.0f and 1.0f,
        //! 0.0f means not transparent (opaque), 1.0f means fully transparent.
        ColourF( f32 r, f32 g, f32 b, f32 a );

        //! Constructs a color from 32 bit Color.
        //! \param c: 32 bit color value from which this Colorf class is
        //! constructed from.
        ColourF( ColourI c );

        //! Converts this color to a SColor without floats.
        ColourI toSColor() const;

        //! Sets three color components to new values at once.
        //! \param rr: Red color component. Should be a value between 0.0f meaning
        //! no red (=black) and 1.0f, meaning full red.
        //! \param gg: Green color component. Should be a value between 0.0f meaning
        //! no green (=black) and 1.0f, meaning full green.
        //! \param bb: Blue color component. Should be a value between 0.0f meaning
        //! no blue (=black) and 1.0f, meaning full blue.
        void set( f32 rr, f32 gg, f32 bb );

        //! Sets all four color components to new values at once.
        //! \param aa: Alpha component.
        //! \param rr: Red color component. Should be a value between 0.0f meaning
        //! no red (=black) and 1.0f, meaning full red.
        //! \param gg: Green color component. Should be a value between 0.0f meaning
        //! no green (=black) and 1.0f, meaning full green.
        //! \param bb: Blue color component. Should be a value between
        //! no blue (=black) and 1.0f, meaning full blue.
        void set( f32 aa, f32 rr, f32 gg, f32 bb );

        //! Interpolates the color with a f32 value to another color
        //! \param other: Other color
        //! \param d: value between 0.0f and 1.0f
        //! \return Returns interpolated color.
        ColourF getInterpolated( const ColourF &other, f32 d ) const;

        //! Returns interpolated color. ( quadratic )
        /** \param c1: first color to interpolate with
        \param c2: second color to interpolate with
        \param d: value between 0.0f and 1.0f. */
        ColourF getInterpolated_quadratic( const ColourF &c1, const ColourF &c2, f32 d ) const;

        //! Sets a color component by index. R=0, G=1, B=2, A=3
        void setColorComponentValue( s32 index, f32 value );

        /// value packed as #PF_R8G8B8A8
        RGBA getAsRGBA( void ) const;

        /// value packed as #PF_A8R8G8B8
        ARGB getAsARGB( void ) const;

        /// value packed as #PF_B8G8R8A8
        BGRA getAsBGRA( void ) const;

        /// value packed as #PF_A8B8G8R8
        ABGR getAsABGR( void ) const;

        /// value packed as #PF_BYTE_RGBA
        RGBA getAsBYTE() const;

        /// Set value from #PF_R8G8B8A8
        void setAsRGBA( RGBA val );

        /// Set value from #PF_A8R8G8B8
        void setAsARGB( ARGB val );

        /// Set value from #PF_B8G8R8A8
        void setAsBGRA( BGRA val );

        /// Set value from #PF_A8B8G8R8
        void setAsABGR( ABGR val );

        /** Checks whether the colour's elements are valid.
         * @return True if the colour is valid, false otherwise.
         */
        bool isValid() const;

        ColourF operator*( f32 fScalar ) const;
        ColourF &operator*=( f32 fScalar );

        bool operator==( const ColourF &other ) const;
        bool operator!=( const ColourF &other ) const;

        static const ColourF ZERO;
        static const ColourF Black;
        static const ColourF White;
        static const ColourF Red;
        static const ColourF Green;
        static const ColourF Blue;

        //! Red component of the color. Should be a value between 0.0f (no red) and 1.0f (full red).
        f32 r = 0.0f;

        //! Green component of the color. Should be a value between 0.0f (no green) and 1.0f (full green).
        f32 g = 0.0f;

        //! Blue component of the color. Should be a value between 0.0f (no blue) and 1.0f (full blue).
        f32 b = 0.0f;

        //! Alpha component of the color. Defines how transparent the color should be.
        //! Should be a value between 0.0f (opaque) and 1.0f (fully transparent).
        f32 a = 0.0f;
    };
}  // namespace fb

#endif  // ColourF_h__
