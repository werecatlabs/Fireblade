#pragma once

#include <FBCore/FBCoreTypes.h>
#include <new>
#include <memory.h>

namespace fb
{
    //! fast allocator, only to be used in containers inside the same memory heap.
    /** Containers using it are NOT able to be used it across dll boundaries. Use this
    when using in an internal class or function or when compiled into a static lib */
    template <typename T>
    class AllocatorFast
    {
    public:
        //! allocate memory for an Array of objects
        T *allocate( u32 cnt )
        {
            return static_cast<T *>( operator new( cnt * sizeof( T ) ) );
        }

        //! deallocate memory for an Array of objects
        void deallocate( T *ptr )
        {
            operator delete( ptr );
        }

        //! construct an element
        void construct( T *ptr, const T &e )
        {
            new( static_cast<void *>( ptr ) ) T( e );
        }

        //! destruct an element
        void destruct( T *ptr )
        {
            ptr->~T();
        }
    };
}  // end namespace fb
