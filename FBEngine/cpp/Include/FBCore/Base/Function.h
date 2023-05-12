#ifndef _FB_Function_h__
#define _FB_Function_h__

#include <FBCore/FBCoreTypes.h>
#include <functional>

namespace fb
{
    template <class T>
    using Function = std::function<T>;
}  // end namespace fb

#endif  // _FB_Function_h__
