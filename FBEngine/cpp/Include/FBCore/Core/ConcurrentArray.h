#ifndef ConcurrentArray_h__
#define ConcurrentArray_h__

#include <FBCore/FBCoreTypes.h>

#if FB_USE_FIREBLADE_CONTAINERS
#    include <FBCore/Core/ConcurrentArrayBase.h>
#else

#    if FB_USE_ONETBB
#        include <tbb/concurrent_vector.h>
#    elif FB_USE_TBB
#        include <tbb/concurrent_vector.h>
#    else
#        include <FBCore/Core/ConcurrentArrayBase.h>
#    endif
#endif

namespace fb
{
#if FB_USE_FIREBLADE_CONTAINERS
    template <class _Ty, class _Alloc = std::allocator<_Ty> >
    using ConcurrentArray = ConcurrentArrayBase<_Ty, _Alloc>;
#else
#    if FB_USE_TBB | FB_USE_TBB
    template <class _Ty, class _Alloc = std::allocator<_Ty> >
    using ConcurrentArray = tbb::concurrent_vector<_Ty, _Alloc>;
#    else
    template <class _Ty, class _Alloc = std::allocator<_Ty> >
    using ConcurrentArray = ConcurrentArrayBase<_Ty, _Alloc>;
#    endif
#endif
}  // end namespace fb

#endif  // ConcurrentArray_h__
