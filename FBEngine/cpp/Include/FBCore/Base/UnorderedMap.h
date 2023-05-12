#ifndef UnorderedMap_h__
#define UnorderedMap_h__

#include <FBCore/FBCoreTypes.h>
#include <unordered_map>

namespace fb
{
    template <class T, class B>
    using UnorderedMap = std::unordered_map<T, B>;
}  // end namespace fb

#endif  // UnorderedMap_h__
