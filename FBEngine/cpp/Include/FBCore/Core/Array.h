#ifndef _FB_Array_h__
#define _FB_Array_h__

#include <FBCore/FBCoreTypes.h>

#if FB_USE_FIREBLADE_CONTAINERS
#    include <FBCore/Core/ArrayBase.h>
#else
#    include <vector>
#endif

namespace fb
{

#if FB_USE_FIREBLADE_CONTAINERS
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using Array = ArrayBase<_Ty, _Alloc>;
#else
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using Array = std::vector<_Ty, _Alloc>;
#endif

}  // end namespace fb

#endif  // Array_h__
