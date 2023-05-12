#ifndef __Deque_h__
#define __Deque_h__

#include <deque>

namespace fb
{
    template <class T, class B>
    using Deque = std::deque<T, B>;
} // end namespace fb

#endif  // __Deque_h__
