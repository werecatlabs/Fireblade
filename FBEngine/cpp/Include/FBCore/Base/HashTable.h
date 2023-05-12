#ifndef HashTable_h__
#define HashTable_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Math/Math.h>

namespace fb
{

    template <typename Key, typename Value, typename TAlloc>
    class HashTable
    {
    public:
        HashTable( u32 size );
        ~HashTable();

        void insert( const Key &key, const Value &value );

        void remove( const Key &key );

        Value &get( const Key &key );

        const Value &get( const Key &key ) const;

        u32 size() const;
        Value *data() const;

    private:
        /** Default hash function. */
        u32 hash( const Key &key ) const;

        u32 m_size;
        Value *m_data;
    };

    template <typename Key, typename Value, typename TAlloc>
    HashTable<Key, Value, TAlloc>::HashTable( u32 size ) : m_size( size ), m_data( nullptr )
    {
        if( m_size > 0 )
        {
            m_data = new Value[size];
        }
    }

    template <typename Key, typename Value, typename TAlloc>
    HashTable<Key, Value, TAlloc>::~HashTable()
    {
        if( m_data )
        {
            delete[] m_data;
            m_data = nullptr;
        }
    }

    template <typename Key, typename Value, typename TAlloc>
    void HashTable<Key, Value, TAlloc>::insert( const Key &key, const Value &value )
    {
        u32 index = hash( key );
        m_data[index] = value;
    }

    template <typename Key, typename Value, typename TAlloc>
    void HashTable<Key, Value, TAlloc>::remove( const Key &key )
    {
        u32 index = hash( key );
        m_data[index] = Value();
    }

    template <typename Key, typename Value, typename TAlloc>
    Value &HashTable<Key, Value, TAlloc>::get( const Key &key )
    {
        u32 index = hash( key );
        return m_data[index];
    }

    template <typename Key, typename Value, typename TAlloc>
    const Value &HashTable<Key, Value, TAlloc>::get( const Key &key ) const
    {
        u32 index = hash( key );
        return m_data[index];
    }

    template <typename Key, typename Value, typename TAlloc>
    u32 HashTable<Key, Value, TAlloc>::size() const
    {
        return m_size;
    }

    template <typename Key, typename Value, typename TAlloc>
    Value *HashTable<Key, Value, TAlloc>::data() const
    {
        return m_data;
    }

    template <typename Key, typename Value, typename TAlloc>
    u32 HashTable<Key, Value, TAlloc>::hash( const Key &key ) const
    {
        static double s_dHashMultiplier = 0.5 * ( MathD::Sqrt( 5.0 ) - 1.0 );
        unsigned int uiKey;
        memcpy( &uiKey, &key, sizeof( unsigned int ) );
        uiKey %= m_size;
        double dFraction = MathD::Mod( s_dHashMultiplier * uiKey, 1.0 );
        return static_cast<u32>( MathD::Floor( m_size * dFraction ) );
    }

}  // end namespace fb

#endif  // HashTable_h__
