#ifndef ConcurrentHashMap_h__
#define ConcurrentHashMap_h__

#include <FBCore/FBCoreTypes.h>

#if FB_USE_TBB
#    include <tbb/concurrent_hash_map.h>
#elif FB_USE_ONETBB
#    include <tbb/concurrent_hash_map.h>
#else
#    include <FBCore/Core/ConcurrentHashmapBase.h>
#endif

namespace fb
{
#if FB_USE_TBB | FB_USE_ONETBB
    template <typename Key, typename T>
    using ConcurrentHashMap = tbb::concurrent_hash_map<Key, T>;
#else
    template <typename Key, typename T>
    using ConcurrentHashMap = ConcurrentHashmapBase<Key, T>;
#endif
}  // end namespace fb

#endif  // ConcurrentQueue_h__
