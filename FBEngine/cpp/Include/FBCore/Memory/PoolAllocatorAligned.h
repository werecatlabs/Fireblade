#ifndef __PoolAllocatorAligned_h__
#define __PoolAllocatorAligned_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>

#if FB_USE_ONETBB | FB_USE_TBB
#    include <tbb/scalable_allocator.h>
#endif

namespace fb
{
    //---------------------------------------------
    struct PoolAllocatorAligned
    {
        // types
        using size_type = u32;  // An unsigned integral type that can represent the size of the largest
        // object to be allocated.
        using difference_type = int;  // A signed integral type that can represent the difference of any
                                      // two pointers.

        // public static functions
        static char *malloc( const size_type size )
        {
#if FB_USE_ONETBB | FB_USE_TBB
            return (char *)scalable_aligned_malloc( size, 16 );
#else
            return (char *)malloc( size );
#endif
        }

        static void free( char *const ptr )
        {
#if FB_USE_ONETBB | FB_USE_TBB
            scalable_aligned_free( ptr );
#else
            free( ptr );
#endif
        }
    };
}  // end namespace fb

#endif  // PoolAllocator_h__
