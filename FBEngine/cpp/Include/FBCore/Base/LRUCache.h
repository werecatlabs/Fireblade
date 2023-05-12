#ifndef LRUCache_h__
#define LRUCache_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Base/Array.h>
#include <iostream>

namespace fb
{

    class LRUCache
    {
    public:
        LRUCache() : m_capcity( 2 )
        {
            m_values.reserve( 2 );
        }

        LRUCache( int capacity ) : m_capcity( capacity )
        {
            m_values.reserve( capacity );
        }

        void removeLRU()
        {
            auto it = std::find( m_values.begin(), m_values.end(), m_lru );
            if( it != m_values.end() )
            {
                m_values.erase( it );
            }
        }

        int get( int key )
        {
            std::pair<int, int> val;
            auto it = std::find_if( m_values.begin(), m_values.end(),
                                    [&]( const auto &val ) { return val.first == 1; } );

            if( it != m_values.end() )
            {
                val = *it;
            }

            auto d = std::distance( m_values.begin(), it );
            if( d++ > m_capcity )
            {
                d = m_capcity - d;
            }

            for( auto &value : m_values )
            {
                if( value.first == key )
                {
                    val = value;
                }
                else
                {
                    m_lru = val;
                }
            }

            print();
            return -1;
        }

        void put( int key, int value )
        {
            if( key >= m_values.size() )
            {
                removeLRU();
            }

            //assert(key < m_values.size());

            //auto bucket = key % m_values.size();
            m_values.push_back( std::make_pair( key, value ) );
            print();
        }

        void print()
        {
            for( auto &value : m_values )
            {
                std::cout << value.first << " " << value.second << std::endl;
            }
        }

        int m_capcity = 0;
        std::pair<int, int> m_lru;
        Array<std::pair<int, int>> m_values;
    };
}  // namespace fb

#endif  // LRUCache_h__
