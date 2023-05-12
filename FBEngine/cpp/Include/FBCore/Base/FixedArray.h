#ifndef FBFixedSizeArray_h__
#define FBFixedSizeArray_h__

#include <FBCore/FBCoreTypes.h>
#include <array>

namespace fb
{
    template <class T, std::size_t N>
    using FixedArray = std::array<T, N>;
}  // end namespace fb

#endif  // FBFixedSizeArray_h__
