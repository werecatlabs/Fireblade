#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Spline/InterpolatorNonUniform4.h>

namespace fb
{
    template <class T>
    InterpolatorNonUniform4<T>::InterpolatorNonUniform4()
    {
    }

    template <class T>
    InterpolatorNonUniform4<T>::~InterpolatorNonUniform4()
    {
    }

    template <class T>
    Vector4<T> InterpolatorNonUniform4<T>::interpolate( const T &t )
    {
        if( m_values.empty() )
        {
            return Vector4<T>::ZERO;
        }

        if( t < T( 0.0 ) )
        {
            if( !m_values.empty() )
            {
                return m_values[0].second;
            }

            return Vector4<T>::ZERO;
        }

        if( t > T( 1.0 ) )
        {
            if( !m_values.empty() )
            {
                return m_values.back().second;
            }

            return Vector4<T>::ZERO;
        }

        std::pair<T, Vector4<T>> pointA;
        std::pair<T, Vector4<T>> pointB;

        bool bInterpolate = true;

        for( u32 i = 0; i < m_values.size(); ++i )
        {
            const std::pair<T, Vector4<T>> &value = m_values[i];
            if( value.first >= t )
            {
                pointA = value;

                u32 nextIndex = i + 1;
                if( nextIndex < m_values.size() )
                {
                    pointB = m_values[nextIndex];
                }
                else
                {
                    bInterpolate = false;
                }

                break;
            }
        }

        if( bInterpolate )
        {
            T time = t - pointA.first;
            return pointA.second + ( ( pointB.second - pointA.second ) * time );
        }

        return m_values.back().second;
    }

    // explicit instantiation
    template class InterpolatorNonUniform4<f32>;

    template class InterpolatorNonUniform4<f64>;
}  // end namespace fb
