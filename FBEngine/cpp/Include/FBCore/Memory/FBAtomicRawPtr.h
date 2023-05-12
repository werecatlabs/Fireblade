#ifndef FBAtomicRawPtr_h__
#define FBAtomicRawPtr_h__

#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Thread/SpinRWMutex.h>

namespace fb
{

    /**
     * A thread-safe class representing an atomic raw pointer.
     *
     * @tparam T the type of the pointer.
     */
    template <class T>
    class FBAtomicRawPtr
    {
    public:
        using this_type = FBAtomicRawPtr<T>;  ///< The type of the atomic raw pointer.
        using element_type = T;               ///< The type of the element.
        using value_type = T;                 ///< The type of the value.
        using pointer = T *;                  ///< The type of the pointer.

        /**
         * Determines whether the atomic raw pointer is lock-free.
         *
         * @return true if the atomic raw pointer is lock-free, false otherwise.
         */
        bool is_lock_free() const;

        /**
         * Stores a raw pointer in the atomic raw pointer.
         *
         * @param ptr the raw pointer to store.
         */
        void store( const RawPtr<T> &ptr );

        /**
         * Loads the raw pointer from the atomic raw pointer.
         *
         * @return the raw pointer.
         */
        RawPtr<T> load() const;

        /**
         * Assigns a raw pointer to the atomic raw pointer.
         *
         * @param ptr the raw pointer to assign.
         */
        void operator=( const RawPtr<T> &ptr ) noexcept;

        /**
         * Implicitly converts the atomic raw pointer to a raw pointer.
         *
         * @return the raw pointer.
         */
        operator RawPtr<T>() const noexcept;

        /**
         * Atomically exchanges the value of the atomic raw pointer with the given raw pointer.
         *
         * @param ptr the raw pointer to exchange.
         * @return the previous raw pointer.
         */
        RawPtr<T> exchange( const RawPtr<T> &ptr );

    protected:
        ///< The mutex used to synchronize access to the atomic raw pointer.
        mutable SpinRWMutex m_mutex;

        ///< The raw pointer stored in the atomic raw pointer.
        RawPtr<T> m_pointer;
    };

    template <class T>
    bool FBAtomicRawPtr<T>::is_lock_free() const
    {
        return false;
    }

    template <class T>
    void FBAtomicRawPtr<T>::store( const RawPtr<T> &ptr )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_pointer = ptr;
    }

    template <class T>
    RawPtr<T> FBAtomicRawPtr<T>::load() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_pointer;
    }

    template <class T>
    void FBAtomicRawPtr<T>::operator=( const RawPtr<T> &ptr ) noexcept
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_pointer = ptr;
    }

    template <class T>
    FBAtomicRawPtr<T>::operator RawPtr<T>() const noexcept
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_pointer;
    }

    template <class T>
    RawPtr<T> FBAtomicRawPtr<T>::exchange( const RawPtr<T> &ptr )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        auto old = m_pointer;
        m_pointer = ptr;
        return old;
    }

}  // end namespace fb

#endif  // FBAtomicWeakPtr_h__
