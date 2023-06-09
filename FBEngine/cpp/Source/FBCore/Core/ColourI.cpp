#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/ColourI.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Core/ColourUtil.h>

namespace fb
{

    ColourI::ColourI( u32 a, u32 r, u32 g, u32 b ) :
        m_color( ( ( a & 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff ) )
    {
    }

    ColourI::ColourI( u32 clr ) : m_color( clr )
    {
    }

    u32 ColourI::getAlpha() const
    {
        return m_color >> 24;
    }

    u32 ColourI::getRed() const
    {
        return ( m_color >> 16 ) & 0xff;
    }

    u32 ColourI::getGreen() const
    {
        return ( m_color >> 8 ) & 0xff;
    }

    ColourI ColourI::operator+( const ColourI &other ) const
    {
        s32 a = getAlpha() + other.getAlpha();
        if( a > 255 )
            a = 255;

        s32 r = getRed() + other.getRed();
        if( r > 255 )
            r = 255;

        s32 g = getGreen() + other.getGreen();
        if( g > 255 )
            g = 255;

        s32 b = getBlue() + other.getBlue();
        if( b > 255 )
            b = 255;

        return ColourI( a, r, g, b );
    }

    ColourI ColourI::getInterpolated( const ColourI &other, f32 d ) const
    {
        const auto inv = 1.0f - d;
        return ColourI( static_cast<u32>( other.getAlpha() * inv + getAlpha() * d ),
                        static_cast<u32>( other.getRed() * inv + getRed() * d ),
                        static_cast<u32>( other.getGreen() * inv + getGreen() * d ),
                        static_cast<u32>( other.getBlue() * inv + getBlue() * d ) );
    }

    bool ColourI::operator!=( const ColourI &other ) const
    {
        return other.m_color != m_color;
    }

    bool ColourI::operator==( const ColourI &other ) const
    {
        return other.m_color == m_color;
    }

    void ColourI::set( u32 col )
    {
        m_color = col;
    }

    void ColourI::set( u32 a, u32 r, u32 g, u32 b )
    {
        m_color =
            ( ( ( a & 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff ) );
    }

    void ColourI::toOpenGLColor( u8 *dest ) const
    {
        *dest = static_cast<u8>( getRed() );
        *++dest = static_cast<u8>( getGreen() );
        *++dest = static_cast<u8>( getBlue() );
        *++dest = static_cast<u8>( getAlpha() );
    }

    u16 ColourI::toA1R5G5B5() const
    {
        return ColourUtil::A8R8G8B8toA1R5G5B5( m_color );
    }

    void ColourI::setBlue( u32 b )
    {
        m_color = ( b & 0xff ) | ( m_color & 0xffffff00 );
    }

    void ColourI::setGreen( u32 g )
    {
        m_color = ( ( g & 0xff ) << 8 ) | ( m_color & 0xffff00ff );
    }

    void ColourI::setRed( u32 r )
    {
        m_color = ( ( r & 0xff ) << 16 ) | ( m_color & 0xff00ffff );
    }

    void ColourI::setAlpha( u32 a )
    {
        m_color = ( ( a & 0xff ) << 24 ) | ( m_color & 0x00ffffff );
    }

    u32 ColourI::getAverage() const
    {
        return ( getRed() + getGreen() + getBlue() ) / 3;
    }

    f32 ColourI::getLuminance() const
    {
        return 0.3f * getRed() + 0.59f * getGreen() + 0.11f * getBlue();
    }

    u32 ColourI::getBlue() const
    {
        return m_color & 0xff;
    }

    ColourI ColourI::getInterpolated_quadratic( const ColourI &c1, const ColourI &c2, const f32 d ) const
    {
        // this*(1-d)*(1-d) + 2 * c1 * (1-d) + c2 * d * d;
        const f32 inv = 1.f - d;
        const f32 mul0 = inv * inv;
        const f32 mul1 = 2.f * d * inv;
        const f32 mul2 = d * d;

        return ColourI(
            static_cast<u32>( MathI::clamp(
                MathI::Floor( static_cast<s32>( (f32)getAlpha() * mul0 + (f32)c1.getAlpha() * mul1 +
                                                (f32)c2.getAlpha() * mul2 ) ),
                0, 255 ) ),
            static_cast<u32>( MathI::clamp(
                MathI::Floor( static_cast<s32>( (f32)getRed() * mul0 + (f32)c1.getRed() * mul1 +
                                                (f32)c2.getRed() * mul2 ) ),
                0, 255 ) ),
            static_cast<u32>( MathI::clamp(
                MathI::Floor( static_cast<s32>( (f32)getGreen() * mul0 + (f32)c1.getGreen() * mul1 +
                                                (f32)c2.getGreen() * mul2 ) ),
                0, 255 ) ),
            static_cast<u32>( MathI::clamp(
                MathI::Floor( static_cast<s32>( (f32)getBlue() * mul0 + (f32)c1.getBlue() * mul1 +
                                                (f32)c2.getBlue() * mul2 ) ),
                0, 255 ) ) );
    }

}  // end namespace fb
