#ifndef ConcurrentQueue_h__
#define ConcurrentQueue_h__

#include <FBCore/FBCoreTypes.h>

#if FB_USE_ONETBB | FB_USE_TBB
#    include <tbb/concurrent_queue.h>
#else
#    include "FBCore/Containers/ConcurrentQueueBase.h"
#endif

namespace fb
{
#if FB_USE_ONETBB | FB_USE_TBB
    template <class T>
    using ConcurrentQueue = tbb::concurrent_queue<T>;
#else
    template <class T>
    using ConcurrentQueue = ConcurrentQueueBase<T>;
#endif
}  // end namespace fb

#endif  // ConcurrentQueue_h__
