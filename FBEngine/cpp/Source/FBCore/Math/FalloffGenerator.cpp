#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/FalloffGenerator.h>
#include <FBCore/Math/Math.h>

namespace fb
{
    template <class T>
    Array<Array<T>> FalloffGenerator<T>::generate()
    {
        Array<Array<T>> map;
        map.resize( m_size );

        for( int i = 0; i < m_size; i++ )
        {
            map[i].resize( m_size );
        }

        for( int i = 0; i < m_size; i++ )
        {
            for( int j = 0; j < m_size; j++ )
            {
                T x = i / static_cast<T>( m_size ) * 2 - 1;
                T y = j / static_cast<T>( m_size ) * 2 - 1;

                T value = Math<T>::max( Math<T>::Abs( x ), Math<T>::Abs( y ) );
                map[i][j] = evaluate( value );
            }
        }

        return map;
    }

    template <class T>
    T FalloffGenerator<T>::evaluate( T value )
    {
        return Math<T>::Pow( value, m_falloffDirection ) /
               ( Math<T>::Pow( value, m_falloffDirection ) +
                 Math<T>::Pow( m_falloffRange - m_falloffRange * value, m_falloffDirection ) );
    }

    template <class T>
    T FalloffGenerator<T>::getFalloffDirection() const
    {
        return m_falloffDirection;
    }

    template <class T>
    T FalloffGenerator<T>::getFalloffRange() const
    {
        return m_falloffRange;
    }

    template <class T>
    s32 FalloffGenerator<T>::getSize() const
    {
        return m_size;
    }

    template <class T>
    void FalloffGenerator<T>::setFalloffDirection( T falloffDirection )
    {
        m_falloffDirection = falloffDirection;
    }

    template <class T>
    void FalloffGenerator<T>::setFalloffRange( T falloffRange )
    {
        m_falloffRange = falloffRange;
    }

    template <class T>
    void FalloffGenerator<T>::setSize( s32 size )
    {
        m_size = size;
    }

    template class FalloffGenerator<f32>;
    template class FalloffGenerator<f64>;
}  // namespace fb
