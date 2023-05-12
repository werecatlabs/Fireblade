#ifndef __AtomicNumber_h__
#define __AtomicNumber_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <atomic>

namespace fb
{
    /** Atomic number to wrap a generic value. */
    template <class T>
    class AtomicNumber
    {
    public:
        /** Default constructor. */
        AtomicNumber();

        /** Constructor that copies a value. */
        AtomicNumber( const T &other );

        /** Copy constructor. */
        AtomicNumber( const AtomicNumber &other );

        /** Destructor. */
        ~AtomicNumber();

        /** Assignment operator. */
        T operator=( const AtomicNumber<T> &other );

        /** Assignment operator. */
        T operator=( const T &other );

        /** Assignment operator. */
        operator T() const;

        /** Gets the value of this atomic type. */
        T get() const;

        /** Operator. */
        T operator+( const T &other ) const;

        /** Operator. */
        T operator+=( const T &other );

        /** Operator. */
        T operator-( const T &other ) const;

        /** Operator. */
        T operator-=( const T &other );

        /** Comparison operator. */
        bool operator==( const T &other ) const;

        /** Comparison operator. */
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
        /// The value
        std::atomic<T> m_value;
    };

    template <class T>
    AtomicNumber<T>::AtomicNumber()
    {
        m_value = T( 0 );
    }

    template <class T>
    AtomicNumber<T>::AtomicNumber( const T &other )
    {
        m_value = other;
    }

    template <class T>
    AtomicNumber<T>::AtomicNumber( const AtomicNumber &other )
    {
        auto value = other.get();
        m_value = value;
    }

    template <class T>
    AtomicNumber<T>::~AtomicNumber()
    {
    }

    template <class T>
    T AtomicNumber<T>::operator=( const AtomicNumber<T> &other )
    {
        m_value = other;
        return m_value;
    }

    template <class T>
    T AtomicNumber<T>::operator=( const T &other )
    {
        m_value = other;
        return m_value;
    }

    template <class T>
    AtomicNumber<T>::operator T() const
    {
        return m_value;
    }

    template <class T>
    T AtomicNumber<T>::get() const
    {
        return m_value;
    }

    template <class T>
    T AtomicNumber<T>::operator+( const T &other ) const
    {
        return m_value + other;
    }

    template <class T>
    T AtomicNumber<T>::operator+=( const T &other )
    {
        m_value += other;
        return m_value;
    }

    template <class T>
    T AtomicNumber<T>::operator-( const T &other ) const
    {
        return m_value - other;
    }

    template <class T>
    T AtomicNumber<T>::operator-=( const T &other )
    {
        m_value -= other;
        return m_value;
    }

    template <class T>
    bool AtomicNumber<T>::operator==( const T &other ) const
    {
        return m_value == other;
    }

    template <class T>
    bool AtomicNumber<T>::operator!=( const T &other ) const
    {
        return m_value != other;
    }

    template <class T>
    T AtomicNumber<T>::operator++( void )
    {
        return ++m_value;
    }

    template <class T>
    T AtomicNumber<T>::operator--( void )
    {
        return --m_value;
    }

    template <class T>
    T AtomicNumber<T>::operator++( int )
    {
        return m_value++;
    }

    template <class T>
    T AtomicNumber<T>::operator--( int )
    {
        return m_value--;
    }

    template <class T>
    T AtomicNumber<T>::fetch_or( T arg, std::memory_order order ) noexcept
    {
        return m_value.fetch_or( arg, order );
    }

    template <class T>
    T AtomicNumber<T>::fetch_and( T arg, std::memory_order order ) noexcept
    {
        return m_value.fetch_and( arg, order );
    }

    template <class T>
    void AtomicNumber<T>::store( T v, std::memory_order m )
    {
        m_value.store( v, m );
    }

    template <class T>
    T AtomicNumber<T>::load( std::memory_order m ) const
    {
        return m_value.load( m );
    }

}  // end namespace fb

#endif  // __AtomicNumber_h__
