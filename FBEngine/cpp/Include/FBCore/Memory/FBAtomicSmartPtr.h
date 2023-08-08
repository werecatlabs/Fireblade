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

    protected:
        T *get()
        {
            return m_pointer;
        }

        ///< The smart pointer that this atomic smart pointer holds.
        std::atomic<T *> m_pointer =
            nullptr;  
    };

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

}  // end namespace fb

#endif  // __FBAtomicSmartPtr_h__
