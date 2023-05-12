#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Spline/InterpolatorNonUniform3.h>

namespace fb
{
    template <class T>
    InterpolatorNonUniform3<T>::InterpolatorNonUniform3()
    {
    }

    template <class T>
    InterpolatorNonUniform3<T>::~InterpolatorNonUniform3()
    {
    }

    template <class T>
    Vector3<T> InterpolatorNonUniform3<T>::interpolate( const T &t )
    {
        if( m_values.empty() )
        {
            return Vector3<T>::ZERO;
        }

        if( t < T( 0.0 ) )
        {
            if( !m_values.empty() )
            {
                return m_values[0].second;
            }

            return Vector3<T>::ZERO;
        }

        if( t > T( 1.0 ) )
        {
            if( !m_values.empty() )
            {
                return m_values.getLast().second;
            }

            return Vector3<T>::ZERO;
        }

        std::pair<T, Vector3<T>> pointA;
        std::pair<T, Vector3<T>> pointB;

        bool bInterpolate = true;

        for( u32 i = 0; i < m_values.size(); ++i )
        {
            const std::pair<T, Vector3<T>> &value = m_values[i];
            if( value.first >= t )
            {
                pointA = value;

                u32 nextIndex = i + 1;
                if( nextIndex <= m_values.size() )
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

        return m_values.getLast().second;
    }

    template <class T>
    const Array<std::pair<T, Vector3<T>>> &InterpolatorNonUniform3<T>::getValues() const
    {
        return m_values;
    }

    template <class T>
    void InterpolatorNonUniform3<T>::setValues( const Array<std::pair<T, Vector3<T>>> &values )
    {
        m_values = values;
    }
}  // end namespace fb
