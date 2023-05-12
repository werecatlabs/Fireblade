#ifndef AtomicObject_h__
#define AtomicObject_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <atomic>

namespace fb
{

    /** An atomic wrapper for objects. */
    template <class T>
    class AtomicObject
    {
    public:
        /** Default constructor. */
        AtomicObject();

        /** Constructor that copies a value. */
        AtomicObject( T other );

        /** Copy constructor. */
        AtomicObject( const AtomicObject &other );

        /** Assignment operator.
        @remarks
        Assumes type has assignment operator to perform atomic operations.
        */
        void operator=( const T &value );

        /** Assignment operator.
        @remarks
        Assumes type has assignment operator to perform atomic operations.
        */
        void operator=( const AtomicObject &other );

        /** Comparison operator. */
        bool operator==( const T &value ) const;

        /** Comparison operator. */
        bool operator==( const AtomicObject &other ) const;

        /** Comparison operator. */
        bool operator!=( const T &value ) const;

        /** Comparison operator. */
        bool operator!=( const AtomicObject &other ) const;

        /** Gets the value of this atomic type. */
        operator T() const;

        /** Exchange the desired value.
        @param desired The desired value.
        @param order The memory order.
        @return The object's value.
        */
        T exchange( T desired, std::memory_order order = std::memory_order_seq_cst ) noexcept;

        /** Fetches the value.
        @param arg The value.
        @param order The memory order.
        @return The object's value.
        */
        T fetch_or( T arg, std::memory_order order = std::memory_order_seq_cst ) noexcept;

        /** Fetches the value.
        @param arg The value.
        @param order The memory order.
        @return The object's value.
        */
        T fetch_and( T arg, std::memory_order order = std::memory_order_seq_cst ) noexcept;

        /** To know if the object is lock free.
        @return A boolean indicating whether the object is lock free.
        */
        bool is_lock_free() const;

        /** Store's a value.
        @param v The value.
        @param m The memory order.
        */
        void store( T v, std::memory_order m = std::memory_order_seq_cst );

        /** Load's a value.
        @param m The memory order.
        @return The return value.
        */
        T load( std::memory_order m = std::memory_order_seq_cst ) const;

    protected:
#if !FB_USE_LOCK_FREE_ATOMIC
        /// The mutex
        mutable SpinRWMutex m_mutex;

        /// The object
        T m_value;
#else
        /// The value
        std::atomic<T> m_value;
#endif
    };

        template <class T>
    AtomicObject<T>::AtomicObject()
    {
        m_value = T();
    }

    template <class T>
    AtomicObject<T>::AtomicObject( T other )
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        m_value = other;
#else
        m_value.store( other );
#endif
    }

    template <class T>
    AtomicObject<T>::AtomicObject( const AtomicObject &other )
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        m_value = other.m_value;
#else
        m_value.store( other.m_value.load() );
#endif
    }

    template <class T>
    void AtomicObject<T>::operator=( const T &value )
    {
        m_value = value;
    }

    template <class T>
    void AtomicObject<T>::operator=( const AtomicObject &other )
    {
        m_value = other.m_value;
    }

    template <class T>
    bool AtomicObject<T>::operator==( const T &value ) const
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return m_value == value;
#else
        return m_value.load() == value;
#endif
    }

    template <class T>
    bool AtomicObject<T>::operator==( const AtomicObject &other ) const
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return m_value == other.m_value;
#else
        return m_value.load() == other.m_value.load();
#endif
    }

    template <class T>
    bool AtomicObject<T>::operator!=( const T &value ) const
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return m_value != value;
#else
        return m_value.load() != value;
#endif
    }

    template <class T>
    bool AtomicObject<T>::operator!=( const AtomicObject &other ) const
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return m_value != other.m_value;
#else
        return m_value.load() != other.m_value.load();
#endif
    }

    template <class T>
    AtomicObject<T>::operator T() const
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return m_value;
#else
        return m_value.load();
#endif
    }

    template <class T>
    T AtomicObject<T>::exchange( T desired, std::memory_order order ) noexcept
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        auto old = m_value;
        m_value = desired;
        return old;
#else
        return m_value.exchange( desired, order );
#endif
    }

    template <class T>
    T AtomicObject<T>::fetch_or( T arg, std::memory_order order ) noexcept
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return m_value;
#else
        return m_value.fetch_or( arg, order );
#endif
    }

    template <class T>
    T AtomicObject<T>::fetch_and( T arg, std::memory_order order ) noexcept
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return m_value.fetch_and( arg );
#else
        return m_value.fetch_and( arg, order );
#endif
    }

    template <class T>
    bool AtomicObject<T>::is_lock_free() const
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        return false;
#else
        return m_value.is_lock_free();
#endif
    }

    template <class T>
    void AtomicObject<T>::store( T v, std::memory_order m )
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_value = v;
#else
        m_value.store( v, m );
#endif
    }

    template <class T>
    T AtomicObject<T>::load( std::memory_order m ) const
    {
#if !FB_USE_LOCK_FREE_ATOMIC
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_value;
#else
        return m_value.load( m );
#endif
    }

}  // end namespace fb

#endif  // AtomicObject_h__
