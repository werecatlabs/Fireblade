#ifndef __FBAtomicSmartPtr_h__
#define __FBAtomicSmartPtr_h__

#include <FBCore/Memory/FBSmartPtr.h>
#include <FBCore/Thread/SpinRWMutex.h>
#include <atomic>

namespace fb
{

    /**
     * An atomic smart pointer that provides atomic store, load, and exchange
     * operations.
     *
     * @tparam T The type of the element pointed to by this smart pointer.
     */
    template <class T>
    class FBAtomicSmartPtr
    {
    public:
        using this_type = FBAtomicSmartPtr<T>;  ///< The type of this smart pointer.
        using element_type = T;                 ///< The type of the pointed-to element.
        using value_type = T;                   ///< The value type of the pointed-to element.
        using pointer = T *;                    ///< The type of the pointer to the pointed-to element.

        /**
         * Constructs an atomic smart pointer.
         */
        FBAtomicSmartPtr() = default;

        FBAtomicSmartPtr( const FBAtomicSmartPtr<T> &other );

        /** Destroys the atomic smart pointer. */
        ~FBAtomicSmartPtr();

        /**
         * Returns true if the object is lock-free.
         *
         * @return True if the object is lock-free.
         */
        bool is_lock_free() const;

        /**
         * Atomically stores the given smart pointer into the atomic smart pointer.
         *
         * @param ptr The smart pointer to store.
         */
        void store( const FBSmartPtr<T> ptr );

        /**
         * Atomically loads the smart pointer stored in this atomic smart pointer.
         *
         * @return The smart pointer stored in this atomic smart pointer.
         */
        FBSmartPtr<T> load() const;

        /**
         * Atomically assigns the given smart pointer to this atomic smart pointer.
         *
         * @param ptr The smart pointer to assign.
         */
        void operator=( const FBSmartPtr<T> ptr ) noexcept;

        /**
         * Converts this atomic smart pointer to a smart pointer.
         *
         * @return The smart pointer that this atomic smart pointer holds.
         */
        operator FBSmartPtr<T>() const noexcept;

        /**
         * Atomically exchanges the given smart pointer with the smart pointer stored in this atomic smart pointer.
         *
         * @param ptr The smart pointer to exchange.
         *
         * @return The smart pointer that was stored in this atomic smart pointer before the exchange.
         */
        FBSmartPtr<T> exchange( const FBSmartPtr<T> ptr );

        /**
         * @brief Accesses the members of the object that the smart pointer is managing.
         *
         * @return A pointer to the object that the smart pointer is managing.
         */
        T *operator->();

        /**
         * @brief Accesses the members of the object that the smart pointer is managing.
         *
         * @return A pointer to the object that the smart pointer is managing.
         */
        const T *operator->() const;

        /**
         * @brief Checks whether the smart pointer is null.
         *
         * @return `true` if the smart pointer is null, `false` otherwise.
         */
        bool operator!() const;

        /**
         * @brief Implicitly converts the smart pointer to a `bool` value.
         *
         * @return `true` if the smart pointer is not null, `false` otherwise.
         */
        operator bool() const;

        T *get();

    protected:
        ///< The smart pointer that this atomic smart pointer holds.
        std::atomic<T *> m_pointer = nullptr;
    };

    template <class T>
    FBAtomicSmartPtr<T>::FBAtomicSmartPtr( const FBAtomicSmartPtr<T> &other )
    {
        *this = other.load();
    }

    template <class T>
    FBAtomicSmartPtr<T>::~FBAtomicSmartPtr()
    {
        if( auto pointer = get() )
        {
            pointer->removeReference();
            m_pointer = nullptr;
        }
    }

    template <class T>
    bool FBAtomicSmartPtr<T>::is_lock_free() const
    {
        return true;
    }

    template <class T>
    void FBAtomicSmartPtr<T>::store( const FBSmartPtr<T> ptr )
    {
        if( auto pointer = get() )
        {
            pointer->removeReference();
        }

        m_pointer.store( ptr.get() );

        if( auto pointer = get() )
        {
            pointer->addReference();
        }
    }

    template <class T>
    FBSmartPtr<T> FBAtomicSmartPtr<T>::load() const
    {
        return m_pointer.load();
    }

    template <class T>
    void FBAtomicSmartPtr<T>::operator=( const FBSmartPtr<T> ptr ) noexcept
    {
        store( ptr );
    }

    template <class T>
    FBAtomicSmartPtr<T>::operator FBSmartPtr<T>() const noexcept
    {
        return m_pointer.load();
    }

    template <class T>
    FBSmartPtr<T> FBAtomicSmartPtr<T>::exchange( const FBSmartPtr<T> ptr )
    {
        auto old = m_pointer.load();
        m_pointer = ptr.get();
        return old;
    }

    template <class T>
    T *FBAtomicSmartPtr<T>::get()
    {
        return m_pointer;
    }

    template <class T>
    FBForceInline T *FBAtomicSmartPtr<T>::operator->()
    {
        FB_ASSERT( m_pointer );

#if FB_ENABLE_PTR_EXCEPTIONS
        if( !m_pointer )
        {
            throw Exception( "Null pointer exception." );
        }
#endif

        return m_pointer;
    }

    template <class T>
    FBForceInline const T *FBAtomicSmartPtr<T>::operator->() const
    {
        FB_ASSERT( m_pointer );

#if FB_ENABLE_PTR_EXCEPTIONS
        if( !m_pointer )
        {
            throw Exception( "Null pointer exception." );
        }
#endif

        return m_pointer;
    }

    template <class T>
    FBForceInline bool FBAtomicSmartPtr<T>::operator!() const
    {
        return m_pointer == nullptr;
    }

    template <class T>
    FBForceInline FBAtomicSmartPtr<T>::operator bool() const
    {
        return m_pointer != nullptr;
    }

}  // end namespace fb

#endif  // __FBAtomicSmartPtr_h__
