#ifndef _FB_UtilTypes_h__
#define _FB_UtilTypes_h__

#include <FBCore/FBCoreTypes.h>
#include <utility>

namespace fb
{
    template <class T, class B>
    using Pair = std::pair<T, B>;
}  // end namespace fb

#endif  // UtilTypes_h__
