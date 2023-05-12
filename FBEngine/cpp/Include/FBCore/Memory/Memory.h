#ifndef __FB_Memory_h__
#define __FB_Memory_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>

namespace fb
{

    template <class T>
    static void FB_SAFE_DELETE( T *&data )
    {
        if( data )
        {
            T *ptr = data;
            delete ptr;

            data = nullptr;
        }
    }

    template <class T>
    static void FB_SAFE_DELETE_ARRAY( T *&data )
    {
        if( data )
        {
            T *ptr = data;
            delete[] ptr;

            data = nullptr;
        }
    }

    class FBCore_API Memory
    {
    public:
        static void Memcpy( void *dest, const void *src, s32 count );
        static void AlignedMemcpy( void *dest0, const void *src0, s32 count0 );
        static void Memset( void *dst, s32 val, s32 count );
        static s32 Memcmp( const void *ptr1, const void *ptr2, s32 num );
        static bool CheckHeap();

        static void *ScalableAlignedMalloc( size_t size, size_t alignment );
        static void ScalableAlignedFree( void *ptr );

        static void *ScalableMalloc( size_t size );
        static void ScalableFree( void *ptr );
    };
}  // end namespace fb

#endif  // Memory_h__
