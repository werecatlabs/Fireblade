namespace fb
{

    template <class T>
    ColourHSL<T>::ColourHSL( T h, T s, T l ) : m_hue( h ), m_saturation( s ), m_luminance( l )
    {
    }

    template <class T>
    u32 ColourHSL<T>::toRGB1( T rm1, T rm2, T rh ) const
    {
        while( rh > T( 2.0 ) * Math<T>::pi() )
        {
            rh -= T( 2.0 ) * Math<T>::pi();
        }

        while( rh < T( 0.0 ) )
        {
            rh += T( 2.0 ) * Math<T>::pi();
        }

        if( rh < T( 60.0 ) * Math<T>::deg_to_rad() )
        {
            rm1 = rm1 + ( rm2 - rm1 ) * rh / ( T( 60.0 ) * Math<T>::deg_to_rad() );
        }
        else if( rh < T( 180.0 ) * Math<T>::deg_to_rad() )
        {
            rm1 = rm2;
        }
        else if( rh < T( 240.0 ) * Math<T>::deg_to_rad() )
        {
            rm1 = rm1 + ( rm2 - rm1 ) * ( ( T( 240.0 ) * Math<T>::deg_to_rad() ) - rh ) /
                            ( T( 60.0 ) * Math<T>::deg_to_rad() );
        }

        return static_cast<u32>( rm1 * T( 255.0 ) );
    }

    template <class T>
    void ColourHSL<T>::settoRGB( ColourI &color ) const
    {
        if( m_saturation == T( 0.0 ) )  // grey
        {
            auto c = static_cast<u8>( m_luminance * T( 255.0 ) );
            color.setRed( c );
            color.setGreen( c );
            color.setBlue( c );
            return;
        }

        T rm1, rm2;

        if( m_luminance <= T( 0.5 ) )
        {
            rm2 = m_luminance + m_luminance * m_saturation;
        }
        else
        {
            rm2 = m_luminance + m_saturation - m_luminance * m_saturation;
        }

        rm1 = T( 2.0 ) * m_luminance - rm2;

        color.setRed( toRGB1( rm1, rm2, m_hue + ( T( 120.0 ) * Math<T>::deg_to_rad() ) ) );
        color.setGreen( toRGB1( rm1, rm2, m_hue ) );
        color.setBlue( toRGB1( rm1, rm2, m_hue - ( T( 120.0 ) * Math<T>::deg_to_rad() ) ) );
    }

    template <class T>
    T ColourHSL<T>::getHue() const
    {
        return m_hue;
    }

    template <class T>
    void ColourHSL<T>::setHue( T hue )
    {
        m_hue = hue;
    }

    template <class T>
    T ColourHSL<T>::getSaturation() const
    {
        return m_saturation;
    }

    template <class T>
    void ColourHSL<T>::setSaturation( T saturation )
    {
        m_saturation = saturation;
    }

    template <class T>
    T ColourHSL<T>::getLuminance() const
    {
        return m_luminance;
    }

    template <class T>
    void ColourHSL<T>::setLuminance( T luminance )
    {
        m_luminance = luminance;
    }

    template <class T>
    bool ColourHSL<T>::isValid() const
    {
        return true;
    }

}  // end namespace fb
