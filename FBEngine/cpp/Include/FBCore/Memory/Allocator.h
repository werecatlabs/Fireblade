#ifndef __FB_Allocator_h__
#define __FB_Allocator_h__

#include <FBCore/FBCoreTypes.h>
#include <new>
#include <memory.h>

namespace fb
{
    //!	Very simple allocator implementation, containers using it are able to
    //! be used it across dll boundaries
    template <typename T>
    class Allocator
    {
    public:
        //! destructor
        virtual ~Allocator()
        {
        }

        //! allocate memory for an Array of objects
        T *allocate( u32 cnt )
        {
            return static_cast<T *>( internal_new( cnt * sizeof( T ) ) );
        }

        //! deallocate memory for an Array of objects
        void deallocate( T *ptr, u32 )
        {
            internal_delete( ptr );
        }

        //! construct an element
        void construct( T *ptr, const T &e )
        {
            new( static_cast<void *>( ptr ) ) T( e );
        }

        //! destruct an element
        void destroy( T *ptr )
        {
            ptr->~T();
        }

    protected:
        virtual void *internal_new( u32 cnt )
        {
            return operator new( cnt );
        }

        virtual void internal_delete( void *ptr )
        {
            operator delete( ptr );
        }
    };
}  // end namespace fb

#endif
