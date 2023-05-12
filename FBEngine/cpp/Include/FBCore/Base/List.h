#ifndef List_h__
#define List_h__

#include <FBCore/FBCoreTypes.h>
#include <list>

namespace fb
{
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using List = std::list<_Ty, _Alloc>;
} // end namespace fb

#endif  // List_h__
