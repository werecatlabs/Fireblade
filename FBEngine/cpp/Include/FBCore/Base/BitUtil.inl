namespace fb
{

    template <typename T>
    bool BitUtil::isPO2( T n )
    {
        return ( n & ( n - 1 ) ) == 0;
    }

    template <class T>
    T BitUtil::bit_and( T val0, T val1 )
    {
        return val0 & val1;
    }

    template <class T>
    T BitUtil::bit_or( T val0, T val1 )
    {
        return val0 | val1;
    }

    template <class T>
    T BitUtil::bit_or( T val0, T val1, T val2 )
    {
        return val0 | val1 | val2;
    }

    template <class T>
    T BitUtil::bit_or( T val0, T val1, T val2, T val3 )
    {
        return val0 | val1 | val2 | val3;
    }

    template <class T>
    T BitUtil::setFlagValue( T flags, T flag, bool value )
    {
        if( value )
        {
            flags |= flag;
        }
        else
        {
            flags &= ~flag;
        }

        return flags;
    }

    template <class T>
    bool BitUtil::getFlagValue( T flags, T flag )
    {
        return ( flags & flag ) != 0;
    }

    inline f32 BitUtil::fixedToFloat( u32 value, u32 bits )
    {
        return static_cast<f32>( value ) / static_cast<f32>( ( 1 << bits ) - 1 );
    }

    inline u32 BitUtil::floatToFixed( f32 value, u32 bits )
    {
        return static_cast<u32>( value * ( 1 << bits ) );
    }

    inline u16 BitUtil::floatToHalf( float i )
    {
        union
        {
            float f;
            u32 i;
        } v;
        v.f = i;
        return floatToHalfI( v.i );
    }

    inline u16 BitUtil::floatToHalfI( u32 i )
    {
        int s = ( i >> 16 ) & 0x00008000;
        int e = ( ( i >> 23 ) & 0x000000ff ) - ( 127 - 15 );
        int m = i & 0x007fffff;

        if( e <= 0 )
        {
            if( e < -10 )
            {
                return 0;
            }
            m = ( m | 0x00800000 ) >> ( 1 - e );

            return static_cast<u16>( s | ( m >> 13 ) );
        }
        if( e == 0xff - ( 127 - 15 ) )
        {
            if( m == 0 )  // Inf
            {
                return static_cast<u16>( s | 0x7c00 );
            }
            // NAN
            m >>= 13;
            return static_cast<u16>( s | 0x7c00 | m | ( m == 0 ) );
        }
        if( e > 30 )  // Overflow
        {
            return static_cast<u16>( s | 0x7c00 );
        }

        return static_cast<u16>( s | ( e << 10 ) | ( m >> 13 ) );
    }

    inline f32 BitUtil::halfToFloat( u16 y )
    {
        union
        {
            f32 f;
            u32 i;
        } v;
        v.i = halfToFloatI( y );
        return v.f;
    }

    inline u32 BitUtil::halfToFloatI( u16 y )
    {
        int s = ( y >> 15 ) & 0x00000001;
        int e = ( y >> 10 ) & 0x0000001f;
        int m = y & 0x000003ff;

        if( e == 0 )
        {
            if( m == 0 )  // Plus or minus zero
            {
                return s << 31;
            }
            // Denormalized number -- renormalize it
            while( !( m & 0x00000400 ) )
            {
                m <<= 1;
                e -= 1;
            }

            e += 1;
            m &= ~0x00000400;
        }
        else if( e == 31 )
        {
            if( m == 0 )  // Inf
            {
                return ( s << 31 ) | 0x7f800000;
            }
            // NaN
            return ( s << 31 ) | 0x7f800000 | ( m << 13 );
        }

        e = e + ( 127 - 15 );
        m = m << 13;

        return ( s << 31 ) | ( e << 23 ) | m;
    }
}  // end namespace fb
