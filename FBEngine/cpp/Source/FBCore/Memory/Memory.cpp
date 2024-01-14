#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/Memory.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <cstring>

#if FB_USE_ONETBB | FB_USE_TBB
#    include <tbb/scalable_allocator.h>
#endif

#if defined FB_PLATFORM_APPLE
#    include <malloc/_malloc.h>
#endif

namespace fb
{

    auto memcpy_kaetemi_sse2( [[maybe_unused]] void *dst, [[maybe_unused]] const void *src,
                              [[maybe_unused]] int nBytes ) -> void *
    {
#if FB_ARCH_TYPE == FB_ARCHITECTURE_32
        __asm {
            // Copyright (C) 2009  Jan Boon (Kaetemi)
            // optimized on Intel Core 2 Duo T7500

			mov         ecx, nBytes
			mov         edi, dst
			mov         esi, src
			add         ecx, edi

			prefetchnta[esi]
			prefetchnta[esi + 32]
			prefetchnta[esi + 64]
			prefetchnta[esi + 96]

            // handle nBytes lower than 128
			cmp         nBytes, 512
			jge         fast
			slow :
			mov         bl, [esi]
				mov[edi], bl
				inc         edi
				inc         esi
				cmp         ecx, edi
				jnz         slow
				jmp         end

				fast :
                // align dstEnd to 128 bytes
			and ecx, 0xFFFFFF80

            // get srcEnd aligned to dstEnd aligned to 128 bytes
				mov         ebx, esi
				sub         ebx, edi
				add         ebx, ecx

                // skip unaligned copy if dst is aligned
				mov         eax, edi
				and edi, 0xFFFFFF80
				cmp         eax, edi
				jne         first
				jmp         more

				first :
                // copy the first 128 bytes unaligned
			movdqu      xmm0, [esi]
				movdqu      xmm1, [esi + 16]
				movdqu      xmm2, [esi + 32]
				movdqu      xmm3, [esi + 48]

				movdqu      xmm4, [esi + 64]
				movdqu      xmm5, [esi + 80]
				movdqu      xmm6, [esi + 96]
				movdqu      xmm7, [esi + 112]

				movdqu[eax], xmm0
				movdqu[eax + 16], xmm1
				movdqu[eax + 32], xmm2
				movdqu[eax + 48], xmm3

				movdqu[eax + 64], xmm4
				movdqu[eax + 80], xmm5
				movdqu[eax + 96], xmm6
				movdqu[eax + 112], xmm7

                // add 128 bytes to edi aligned earlier
				add         edi, 128

            // offset esi by the same value
				sub         eax, edi
				sub         esi, eax

                // last bytes if dst at dstEnd
				cmp         ecx, edi
				jnz         more
				jmp         last

				more :
                // handle equally aligned arrays
			mov         eax, esi
				and eax, 0xFFFFFF80
				cmp         eax, esi
				jne         unaligned4k

				aligned4k :
			mov         eax, esi
				add         eax, 4096
				cmp         eax, ebx
				jle         aligned4kin
				cmp         ecx, edi
				jne         alignedlast
				jmp         last

				aligned4kin :
			prefetchnta[esi]
				prefetchnta[esi + 32]
				prefetchnta[esi + 64]
				prefetchnta[esi + 96]

				add         esi, 128

				cmp         eax, esi
				jne         aligned4kin

				sub         esi, 4096

				alinged4kout:
			movdqa      xmm0, [esi]
				movdqa      xmm1, [esi + 16]
				movdqa      xmm2, [esi + 32]
				movdqa      xmm3, [esi + 48]

				movdqa      xmm4, [esi + 64]
				movdqa      xmm5, [esi + 80]
				movdqa      xmm6, [esi + 96]
				movdqa      xmm7, [esi + 112]

				movntdq[edi], xmm0
				movntdq[edi + 16], xmm1
				movntdq[edi + 32], xmm2
				movntdq[edi + 48], xmm3

				movntdq[edi + 64], xmm4
				movntdq[edi + 80], xmm5
				movntdq[edi + 96], xmm6
				movntdq[edi + 112], xmm7

				add         esi, 128
				add         edi, 128

				cmp         eax, esi
				jne         alinged4kout
				jmp         aligned4k

				alignedlast :
			mov         eax, esi

				alignedlastin :
			prefetchnta[esi]
				prefetchnta[esi + 32]
				prefetchnta[esi + 64]
				prefetchnta[esi + 96]

				add         esi, 128

				cmp         ebx, esi
				jne         alignedlastin

				mov         esi, eax

				alignedlastout :
			movdqa      xmm0, [esi]
				movdqa      xmm1, [esi + 16]
				movdqa      xmm2, [esi + 32]
				movdqa      xmm3, [esi + 48]

				movdqa      xmm4, [esi + 64]
				movdqa      xmm5, [esi + 80]
				movdqa      xmm6, [esi + 96]
				movdqa      xmm7, [esi + 112]

				movntdq[edi], xmm0
				movntdq[edi + 16], xmm1
				movntdq[edi + 32], xmm2
				movntdq[edi + 48], xmm3

				movntdq[edi + 64], xmm4
				movntdq[edi + 80], xmm5
				movntdq[edi + 96], xmm6
				movntdq[edi + 112], xmm7

				add         esi, 128
				add         edi, 128

				cmp         ecx, edi
				jne         alignedlastout
				jmp         last

				unaligned4k :
			mov         eax, esi
				add         eax, 4096
				cmp         eax, ebx
				jle         unaligned4kin
				cmp         ecx, edi
				jne         unalignedlast
				jmp         last

				unaligned4kin :
			prefetchnta[esi]
				prefetchnta[esi + 32]
				prefetchnta[esi + 64]
				prefetchnta[esi + 96]

				add         esi, 128

				cmp         eax, esi
				jne         unaligned4kin

				sub         esi, 4096

				unalinged4kout:
			movdqu      xmm0, [esi]
				movdqu      xmm1, [esi + 16]
				movdqu      xmm2, [esi + 32]
				movdqu      xmm3, [esi + 48]

				movdqu      xmm4, [esi + 64]
				movdqu      xmm5, [esi + 80]
				movdqu      xmm6, [esi + 96]
				movdqu      xmm7, [esi + 112]

				movntdq[edi], xmm0
				movntdq[edi + 16], xmm1
				movntdq[edi + 32], xmm2
				movntdq[edi + 48], xmm3

				movntdq[edi + 64], xmm4
				movntdq[edi + 80], xmm5
				movntdq[edi + 96], xmm6
				movntdq[edi + 112], xmm7

				add         esi, 128
				add         edi, 128

				cmp         eax, esi
				jne         unalinged4kout
				jmp         unaligned4k

				unalignedlast :
			mov         eax, esi

				unalignedlastin :
			prefetchnta[esi]
				prefetchnta[esi + 32]
				prefetchnta[esi + 64]
				prefetchnta[esi + 96]

				add         esi, 128

				cmp         ebx, esi
				jne         unalignedlastin

				mov         esi, eax

				unalignedlastout :
			movdqu      xmm0, [esi]
				movdqu      xmm1, [esi + 16]
				movdqu      xmm2, [esi + 32]
				movdqu      xmm3, [esi + 48]

				movdqu      xmm4, [esi + 64]
				movdqu      xmm5, [esi + 80]
				movdqu      xmm6, [esi + 96]
				movdqu      xmm7, [esi + 112]

				movntdq[edi], xmm0
				movntdq[edi + 16], xmm1
				movntdq[edi + 32], xmm2
				movntdq[edi + 48], xmm3

				movntdq[edi + 64], xmm4
				movntdq[edi + 80], xmm5
				movntdq[edi + 96], xmm6
				movntdq[edi + 112], xmm7

				add         esi, 128
				add         edi, 128

				cmp         ecx, edi
				jne         unalignedlastout
				jmp         last

				last :
                // get the last 128 bytes
			mov         ecx, nBytes
				mov         edi, dst
				mov         esi, src
				add         edi, ecx
				add         esi, ecx
				sub         edi, 128
				sub         esi, 128

            // copy the last 128 bytes unaligned
				movdqu      xmm0, [esi]
				movdqu      xmm1, [esi + 16]
				movdqu      xmm2, [esi + 32]
				movdqu      xmm3, [esi + 48]

				movdqu      xmm4, [esi + 64]
				movdqu      xmm5, [esi + 80]
				movdqu      xmm6, [esi + 96]
				movdqu      xmm7, [esi + 112]

				movdqu[edi], xmm0
				movdqu[edi + 16], xmm1
				movdqu[edi + 32], xmm2
				movdqu[edi + 48], xmm3

				movdqu[edi + 64], xmm4
				movdqu[edi + 80], xmm5
				movdqu[edi + 96], xmm6
				movdqu[edi + 112], xmm7

				end :
        }

        return dst;
#endif

        return nullptr;
    }

    // Courtesy of William Chan and Google. 30-70% faster than memcpy in Microsoft Visual Studio 2005.
    void X_aligned_memcpy_sse2( [[maybe_unused]] void *dest, [[maybe_unused]] const void *src,
                                [[maybe_unused]] const unsigned long u32 )
    {
#if FB_ARCH_TYPE == FB_ARCHITECTURE_32
        __asm {
			mov esi, src;  // src pointer
			mov edi, dest;  // dest pointer

			mov ebx, u32;  // ebx is our counter 
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
                            //loop_copy_end:
        }
#endif
    }

    void Memory::Memcpy( void *dest0, const void *src0, const int count0 )
    {
#if FB_ARCH_TYPE == FB_ARCHITECTURE_32
        memcpy_kaetemi_sse2( dest0, src0, count0 );
#else
        memcpy( dest0, src0, count0 );
#endif
    }

    void Memory::AlignedMemcpy( void *dest0, const void *src0, const int count0 )
    {
#if FB_ARCH_TYPE == FB_ARCHITECTURE_32
        X_aligned_memcpy_sse2( dest0, src0, count0 );
#else
        memcpy( dest0, src0, count0 );
#endif
    }

    void Memory::Memset( void *dest0, const int val, const int count0 )
    {
        memset( dest0, val, count0 );
    }

    auto Memory::Memcmp( const void *ptr1, const void *ptr2, s32 num ) -> s32
    {
        return memcmp( ptr1, ptr2, num );
    }

    auto Memory::CheckHeap() -> bool
    {
#ifdef WIN32
        /* Check heap status */
        auto heapstatus = _heapchk();
        switch( heapstatus )
        {
        case _HEAPOK:
            return true;
        case _HEAPEMPTY:
            return false;
        case _HEAPBADBEGIN:
            return false;
        case _HEAPBADNODE:
            return false;
        }
#endif

        return true;
    }

    auto Memory::ScalableAlignedMalloc( size_t size, size_t alignment ) -> void *
    {
#if FB_ENABLE_HEAP_DEBUG
        FB_ASSERT( CheckHeap() );
        auto ptr = scalable_aligned_malloc( size, alignment );
        if( !ptr )
        {
            std::cout << "Out of memory." << std::endl;
            std::terminate();
        }

        FB_ASSERT( CheckHeap() );
        return ptr;
#elif FB_USE_ONETBB | FB_USE_TBB
        auto ptr = scalable_aligned_malloc( size, alignment );
        if( !ptr )
        {
            std::cout << "Out of memory." << std::endl;
            std::terminate();
        }

        return ptr;
#else

#    if defined FB_PLATFORM_APPLE
        auto ptr = malloc( size );
        if( !ptr )
        {
            std::cout << "Out of memory." << std::endl;
            std::terminate();
        }

        return ptr;
#    else
        auto ptr = _aligned_malloc( size, alignment );
        if( !ptr )
        {
            std::cout << "Out of memory." << std::endl;
            std::terminate();
        }

        return ptr;
#    endif
#endif
    }

    void Memory::ScalableAlignedFree( void *ptr )
    {
#if FB_ENABLE_HEAP_DEBUG
        FB_ASSERT( CheckHeap() );
        scalable_aligned_free( ptr );
        FB_ASSERT( CheckHeap() );
#elif FB_USE_ONETBB | FB_USE_TBB
        scalable_aligned_free( ptr );
#else
#    if defined FB_PLATFORM_APPLE
        free( ptr );
#    else
        _aligned_free( ptr );
#    endif
#endif
    }

    auto Memory::ScalableMalloc( size_t size ) -> void *
    {
#if FB_ENABLE_HEAP_DEBUG
        FB_ASSERT( CheckHeap() );
        return malloc( size );
#elif FB_USE_ONETBB | FB_USE_TBB
        return scalable_malloc( size );
#else
        return malloc( size );
#endif
    }

    void Memory::ScalableFree( void *ptr )
    {
#if FB_ENABLE_HEAP_DEBUG
        FB_ASSERT( CheckHeap() );
        free( ptr );
#elif FB_USE_ONETBB | FB_USE_TBB
        scalable_free( ptr );
#else
        free( ptr );
#endif
    }
}  // end namespace fb
