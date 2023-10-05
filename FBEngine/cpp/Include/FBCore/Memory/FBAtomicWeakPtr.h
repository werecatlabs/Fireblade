#ifndef FBAtomicWeakPtr_h__
#define FBAtomicWeakPtr_h__

#include <FBCore/Memory/FBWeakPtr.h>
#include <FBCore/Thread/SpinRWMutex.h>

namespace fb
{

    /**
     * A thread-safe version of FBWeakPtr. Allows multiple threads to access and modify the
     * pointer held by a weak pointer atomically.
     *
     * @tparam T The type of the object that the pointer points to.
     */
    template <class T>
    class FBAtomicWeakPtr
    {
    public:
        using this_type = FBAtomicWeakPtr<T>;
        using element_type = T;
        using value_type = T;
        using pointer = T *;

        /**
         * Constructs a new FBAtomicWeakPtr object.
         */
        FBAtomicWeakPtr() = default;

        /**
         * Constructs a new FBAtomicWeakPtr object by copying the value of another FBAtomicWeakPtr object.
         *
         * @param other The FBAtomicWeakPtr object to copy.
         */
        FBAtomicWeakPtr( const FBAtomicWeakPtr<T> &other );

        /**
         * Constructs a new FBAtomicWeakPtr object by copying the value of an FBWeakPtr object.
         *
         * @param other The FBWeakPtr object to copy.
         */
        FBAtomicWeakPtr( const FBWeakPtr<T> &other );

        /**
         * Destroys the FBAtomicWeakPtr object.
         */
        ~FBAtomicWeakPtr() = default;

        /**
         * Determines whether or not the underlying pointer is lock-free.
         *
         * @return True if the pointer is lock-free, false otherwise.
         */
        bool is_lock_free() const;

        /**
         * Atomically sets the value of the underlying pointer to the value of a given FBWeakPtr object.
         *
         * @param ptr The FBWeakPtr object whose value to set.
         */
        void store( const FBWeakPtr<T> &ptr );

        /**
         * Atomically gets the value of the underlying pointer as an FBWeakPtr object.
         *
         * @return An FBWeakPtr object representing the value of the pointer.
         */
        FBWeakPtr<T> load() const;

        /**
         * Copies the value of another FBAtomicWeakPtr object to this one.
         *
         * @param ptr The FBAtomicWeakPtr object to copy.
         */
        void operator=( const FBAtomicWeakPtr<T> &ptr ) noexcept;

        /**
         * Copies the value of an FBWeakPtr object to this FBAtomicWeakPtr object.
         *
         * @param ptr The FBWeakPtr object to copy.
         */
        void operator=( const FBWeakPtr<T> &ptr ) noexcept;

        /**
         * Converts the FBAtomicWeakPtr object to an FBWeakPtr object.
         *
         * @return An FBWeakPtr object representing the value of the pointer.
         */
        operator FBWeakPtr<T>() const noexcept;

        /**
         * Atomically exchanges the value of the underlying pointer with the value of a given FBWeakPtr object.
         *
         * @param ptr The FBWeakPtr object whose value to exchange.
         * @return An FBWeakPtr object representing the previous value of the pointer.
         */
        FBWeakPtr<T> exchange( const FBWeakPtr<T> &ptr );

    protected:
        mutable SpinRWMutex m_mutex;
        FBWeakPtr<T> m_pointer;
    };

    template <class T>
    FBAtomicWeakPtr<T>::FBAtomicWeakPtr( const FBAtomicWeakPtr<T> &other )
    {
        *this = other;
    }

    template <class T>
    FBAtomicWeakPtr<T>::FBAtomicWeakPtr( const FBWeakPtr<T> &other )
    {
        *this = other;
    }

    template <class T>
    bool FBAtomicWeakPtr<T>::is_lock_free() const
    {
        return false;
    }

    template <class T>
    void FBAtomicWeakPtr<T>::store( const FBWeakPtr<T> &ptr )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_pointer = ptr;
    }

    template <class T>
    FBWeakPtr<T> FBAtomicWeakPtr<T>::load() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_pointer;
    }

    template <class T>
    void FBAtomicWeakPtr<T>::operator=( const FBAtomicWeakPtr<T> &ptr ) noexcept
    {
        auto p = ptr.load();

        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_pointer = p;
    }

    template <class T>
    void FBAtomicWeakPtr<T>::operator=( const FBWeakPtr<T> &ptr ) noexcept
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_pointer = ptr;
    }

    template <class T>
    FBAtomicWeakPtr<T>::operator FBWeakPtr<T>() const noexcept
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_pointer;
    }

    template <class T>
    FBWeakPtr<T> FBAtomicWeakPtr<T>::exchange( const FBWeakPtr<T> &ptr )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        auto old = m_pointer;
        m_pointer = ptr;
        return old;
    }

}  // end namespace fb

#endif  // FBAtomicWeakPtr_h__
