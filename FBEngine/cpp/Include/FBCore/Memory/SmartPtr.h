#ifndef _FB_SmartPtr_H_
#define _FB_SmartPtr_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/FBSmartPtr.h>
#include <FBCore/Memory/FBWeakPtr.h>
#include <FBCore/Memory/FBAtomicSmartPtr.h>
#include <FBCore/Memory/FBAtomicWeakPtr.h>
#include <FBCore/Memory/FBAtomicRawPtr.h>

#if FB_USE_BOOST
#    include <boost/smart_ptr/shared_ptr.hpp>
#    include <boost/smart_ptr/atomic_shared_ptr.hpp>
#else
#    include <memory>
#endif

namespace fb
{
    template <class T>
    using SmartPtr = FBSmartPtr<T>;

    template <class T>
    using WeakPtr = FBWeakPtr<T>;

    template <class T>
    using AtomicSmartPtr = FBAtomicSmartPtr<T>;

    template <class T>
    using AtomicWeakPtr = FBAtomicWeakPtr<T>;

    template <class T>
    using AtomicRawPtr = FBAtomicRawPtr<T>;

#if FB_USE_BOOST
    template <class T>
    using SharedPtr = boost::shared_ptr<T>;

    template <class T>
    using AtomicSharedPtr = boost::atomic_shared_ptr<T>;
#else
    template <class T>
    using SharedPtr = std::shared_ptr<T>;

    template <class T>
    using AtomicSharedPtr = std::atomic<std::shared_ptr<T> >;
#endif

}  // end namespace fb

#endif
