#ifndef HashMap_h__
#define HashMap_h__

#include <unordered_map>

namespace fb
{
    template <class T, class B>
    using HashMap = std::unordered_map<T, B>;

    template <class T, class B>
    using UnorderedMap = std::unordered_map<T, B>;
}  // end namespace fb

#endif  // HashMap_h__
