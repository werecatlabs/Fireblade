#ifndef AtomicValue_h__
#define AtomicValue_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Thread/SpinRWMutex.h>
#include <atomic>

namespace fb
{
    /** Atomic value to wrap a generic value. */
    template <class T>
    class AtomicValue
    {
    public:
        /** Default constructor. */
        AtomicValue();

        /** Constructor that copies a value. */
        AtomicValue( const T &other );

        /** Copy constructor. */
        AtomicValue( const AtomicValue &other );

        /** Destructor. */
        ~AtomicValue();

        /** Assignment operator. */
        T operator=( const AtomicValue<T> &other );

        /** Assignment operator. */
        T operator=( const T &other );

        /** Value operator. */
        operator T() const;

        /** Returns a value. */
        T get() const;

        /** Operator. */
        T operator+( const T &other ) const;

        /** Operator. */
        T operator+=( const T &other );

        /** Operator. */
        T operator-( const T &other ) const;

        /** Operator. */
        T operator-=( const T &other );

        /** Assignment operator. */
        bool operator==( const T &other ) const;

        /** Assignment operator. */
        bool operator!=( const T &other ) const;

        /** Prefix operator. */
        T operator++( void );

        /** Postfix operator. */
        T operator--( void );

        /** Prefix operator. */
        T operator++( int );

        /** Postfix operator. */
        T operator--( int );

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

    private:
        /// The mutex
        mutable SpinRWMutex m_mutex;

        /// The value
        T m_value;
    };

    template <class T>
    AtomicValue<T>::AtomicValue()
    {
    }

    template <class T>
    AtomicValue<T>::AtomicValue( const T &other )
    {
        m_value = other;
    }

    template <class T>
    AtomicValue<T>::AtomicValue( const AtomicValue &other )
    {
        auto value = other.get();
        store( value );
    }

    template <class T>
    AtomicValue<T>::~AtomicValue()
    {
    }

    template <class T>
    T AtomicValue<T>::operator=( const AtomicValue<T> &value )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_value = value;
        return m_value;
    }

    template <class T>
    T AtomicValue<T>::operator=( const T &value )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_value = value;
        return m_value;
    }

    template <class T>
    AtomicValue<T>::operator T() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value;
    }

    template <class T>
    T AtomicValue<T>::get() const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value;
    }

    template <class T>
    T AtomicValue<T>::operator+( const T &other ) const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value + other;
    }

    template <class T>
    T AtomicValue<T>::operator+=( const T &other )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_value += other;
        return m_value;
    }

    template <class T>
    T AtomicValue<T>::operator-( const T &other ) const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value - other;
    }

    template <class T>
    T AtomicValue<T>::operator-=( const T &other )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_value -= other;
        return m_value;
    }

    template <class T>
    bool AtomicValue<T>::operator==( const T &other ) const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value == other;
    }

    template <class T>
    bool AtomicValue<T>::operator!=( const T &other ) const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value != other;
    }

    template <class T>
    T AtomicValue<T>::operator++( void )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return ++m_value;
    }

    template <class T>
    T AtomicValue<T>::operator--( void )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return --m_value;
    }

    template <class T>
    T AtomicValue<T>::operator++( int )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value++;
    }

    template <class T>
    T AtomicValue<T>::operator--( int )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        return m_value--;
    }

    template <class T>
    T AtomicValue<T>::fetch_or( T arg, std::memory_order order ) noexcept
    {
        return m_value;
    }

    template <class T>
    T AtomicValue<T>::fetch_and( T arg, std::memory_order order ) noexcept
    {
        return m_value;
    }

    template <class T>
    void AtomicValue<T>::store( T v, std::memory_order m )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_value = v;
    }

    template <class T>
    T AtomicValue<T>::load( std::memory_order m ) const
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_value;
    }

}  // end namespace fb

#endif  // AtomicValue_h__
