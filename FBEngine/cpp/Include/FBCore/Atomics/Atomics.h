#ifndef _FBAtomics_H_
#define _FBAtomics_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Thread/SpinRWMutex.h>
#include <FBCore/Atomics/AtomicNumber.h>
#include <FBCore/Atomics/AtomicObject.h>
#include <FBCore/Atomics/AtomicValue.h>
#include <atomic>

namespace fb
{
#if FB_USE_STD_ATOMIC
    typedef std::atomic_uchar atomic_u8;
    typedef std::atomic_char atomic_s8;
    typedef std::atomic_ushort atomic_u16;
    typedef std::atomic_short atomic_s16;
    typedef std::atomic_bool atomic_bool;
    typedef std::atomic_uint atomic_u32;
    typedef std::atomic_int atomic_s32;
    typedef std::atomic_int64_t atomic_s64;
    typedef std::atomic_uint64_t atomic_u64;

    template <class T>
    using Atomic = std::atomic<T>;
#else
    /// A type definition for an atomic unsigned integer
    using atomic_u8 = AtomicNumber<u8>;

    /// A type definition for an atomic integer
    using atomic_s8 = AtomicNumber<c8>;

    /// A type definition for an atomic unsigned integer
    using atomic_u16 = AtomicNumber<u16>;

    /// A type definition for an atomic integer
    using atomic_s16 = AtomicNumber<s16>;

    /// A type definition for an atomic boolean
    using atomic_bool = AtomicNumber<bool>;

    /// A type definition for an atomic unsigned integer
    using atomic_u32 = AtomicNumber<u32>;

    /// A type definition for an atomic integer
    using atomic_s32 = AtomicNumber<s32>;

    /// A type definition for an atomic integer
    using atomic_s64 = AtomicNumber<int64_t>;

    /// A type definition for an atomic integer
    using atomic_u64 = AtomicNumber<uint64_t>;

    /// A type definition for an atomic object
    template <class T>
    using Atomic = AtomicObject<T>;
#endif
}  // end namespace fb

#endif
