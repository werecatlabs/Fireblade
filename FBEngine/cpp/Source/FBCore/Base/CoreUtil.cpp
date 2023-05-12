#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/CoreUtil.h>
#include <FBCore/Thread/Threading.h>

#if defined FB_PLATFORM_WIN32
#    include <windows.h>
#endif

namespace fb
{
    const s32 CoreUtil::compact_range = std::numeric_limits<short>::max();

    //------------------------------------------------
    // Courtesy of William Chan and Google. 30-70% faster than memcpy in Microsoft Visual Studio 2005.
    void CoreUtil::X_aligned_memcpy_sse2( void *dest, const void *src, const size_t size )
    {
        memcpy( dest, src, size );

#ifdef FB_PLATFORM_WIN32
#    if FB_ARCH_TYPE == FB_ARCHITECTURE_32
#        if 0
        __asm {
			mov esi, src;  // src pointer
			mov edi, dest;  // dest pointer

			mov ebx, size;  // ebx is our counter
			shr ebx, 7;  // divide by 128 (8 * 128bit registers)


		loop_copy:
			prefetchnta 128[ESI];  // SSE2 prefetch
			prefetchnta 160[ESI];
			prefetchnta 192[ESI];
			prefetchnta 224[ESI];

			movdqa xmm0, 0[ESI];  // move data from src to registers
			movdqa xmm1, 16[ESI];
			movdqa xmm2, 32[ESI];
			movdqa xmm3, 48[ESI];
			movdqa xmm4, 64[ESI];
			movdqa xmm5, 80[ESI];
			movdqa xmm6, 96[ESI];
			movdqa xmm7, 112[ESI];

			movntdq 0[EDI], xmm0;  // move data from registers to dest
			movntdq 16[EDI], xmm1;
			movntdq 32[EDI], xmm2;
			movntdq 48[EDI], xmm3;
			movntdq 64[EDI], xmm4;
			movntdq 80[EDI], xmm5;
			movntdq 96[EDI], xmm6;
			movntdq 112[EDI], xmm7;

			add esi, 128;
			add edi, 128;
			dec ebx;

			jnz loop_copy;  // loop please
		loop_copy_end:
        }
#        endif
#    else
        memcpy( dest, src, size );
#    endif
#else
        memcpy( dest, src, size );
#endif
    }

    //------------------------------------------------
    void CoreUtil::accurateSleep( [[maybe_unused]] f64 seconds )
    {
#if defined FB_PLATFORM_WIN32
        if( seconds == 0.0 )
        {
            return;
        }

        static LARGE_INTEGER s_freq = { { 0, 0 } };
        if( s_freq.QuadPart == 0 )
        {
            QueryPerformanceFrequency( &s_freq );
        }

        LARGE_INTEGER from, now;
        QueryPerformanceCounter( &from );
        auto ticks_to_wait = static_cast<s32>( static_cast<f64>( s_freq.QuadPart ) / ( 1.0 / seconds ) );
        auto done = false;
        auto ticks_passed = 0;
        auto ticks_left = 0;

        do
        {
            QueryPerformanceCounter( &now );
            ticks_passed = static_cast<s32>( now.QuadPart - from.QuadPart );
            ticks_left = ticks_to_wait - ticks_passed;

            // time wrap
            if( now.QuadPart < from.QuadPart )
            {
                done = true;
            }

            if( ticks_passed >= ticks_to_wait )
            {
                done = true;
            }

            if( !done )
            {
                Thread::yield();
            }
        } while( !done );
#endif
    }

    //------------------------------------------------
    void CoreUtil::getDesktopResolution( [[maybe_unused]] s32 &horizontal,
                                         [[maybe_unused]] s32 &vertical )
    {
#if defined FB_PLATFORM_WIN32
        RECT desktop;
        // Get a handle to the desktop window
        const HWND hDesktop = GetDesktopWindow();
        // Get the size of screen to the variable desktop
        GetWindowRect( hDesktop, &desktop );
        // The top left corner will have coordinates (0,0)
        // and the bottom right corner will have coordinates
        // (horizontal, vertical)
        horizontal = desktop.right;
        vertical = desktop.bottom;
#endif
    }

    //------------------------------------------------
    __m128 CoreUtil::crossProduct( __m128 a, __m128 b )
    {
        return _mm_sub_ps( _mm_mul_ps( _mm_shuffle_ps( a, a, _MM_SHUFFLE( 3, 0, 2, 1 ) ),
                                       _mm_shuffle_ps( b, b, _MM_SHUFFLE( 3, 1, 0, 2 ) ) ),
                           _mm_mul_ps( _mm_shuffle_ps( a, a, _MM_SHUFFLE( 3, 1, 0, 2 ) ),
                                       _mm_shuffle_ps( b, b, _MM_SHUFFLE( 3, 0, 2, 1 ) ) ) );
    }

    //------------------------------------------------
    s16 CoreUtil::compactFloat( f64 input, s32 range )
    {
        return static_cast<s16>( Math<f64>::round( input * compact_range / range ) );
    }

    //------------------------------------------------
    f64 CoreUtil::expandToFloat( s16 input, s32 range )
    {
        return static_cast<f64>( input ) * range / compact_range;
    }

    //------------------------------------------------
    void CoreUtil::convertBetweenBGRAandRGBA( u8 *input, s32 pixel_width, s32 pixel_height, u8 *output )
    {
        auto offset = 0;

        for( s32 y = 0; y < pixel_height; y++ )
        {
            for( s32 x = 0; x < pixel_width; x++ )
            {
                output[offset] = input[offset + 2];
                output[offset + 1] = input[offset + 1];
                output[offset + 2] = input[offset];
                output[offset + 3] = input[offset + 3];

                offset += 4;
            }
        }
    }
}  // end namespace fb
