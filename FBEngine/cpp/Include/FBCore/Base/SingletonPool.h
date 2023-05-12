#ifndef SingletonPool_h__
#define SingletonPool_h__

#include <FBCore/FBCoreTypes.h>

#ifdef FB_USE_BOOST

#    include <boost/pool/singleton_pool.hpp>

namespace fb
{
    template <typename Tag, unsigned RequestedSize,
              typename UserAllocator = boost::default_user_allocator_new_delete,
              typename Mutex = boost::details::pool::default_mutex, unsigned NextSize = 32,
              unsigned MaxSize = 0>
    class SingletonPool
        : public boost::singleton_pool<Tag, RequestedSize, UserAllocator, Mutex, NextSize, MaxSize>
    {
    public:
        SingletonPool() :
            boost::singleton_pool<Tag, RequestedSize, UserAllocator, Mutex, NextSize, MaxSize>()
        {
        }

        ~SingletonPool()
        {
        }
    };
}  // namespace fb

#else

namespace fb
{
    struct SingletonPoolDummy
    {
    };

    template <typename Tag, unsigned RequestedSize, typename UserAllocator = SingletonPoolDummy,
              typename Mutex = SingletonPoolDummy, unsigned NextSize = 32, unsigned MaxSize = 0>
    class SingletonPool
    {
    public:
        SingletonPool()
        {
        }

        ~SingletonPool()
        {
        }

        static void *malloc()
        {
            return new u8[RequestedSize];
        }

        static void free( void *const ptr )
        {
            delete[] ptr;
        }
    };
}  // namespace fb

#endif

#endif  // SingletonPool_h__
