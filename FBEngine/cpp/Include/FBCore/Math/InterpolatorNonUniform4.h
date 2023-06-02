#ifndef InterpolatorNonUniform4_h__
#define InterpolatorNonUniform4_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector4.h>

namespace fb
{
    //------------------------------------------------
    template <class T>
    class InterpolatorNonUniform4
    {
    public:
        InterpolatorNonUniform4();
        ~InterpolatorNonUniform4();

        void setValues( const Array<std::pair<T, Vector4<T>>> &values )
        {
            m_values = values;
        }

        const Array<std::pair<T, Vector4<T>>> &getValues() const
        {
            return m_values;
        }

        Vector4<T> interpolate( const T &t );

    protected:
        Array<std::pair<T, Vector4<T>>> m_values;
    };

    using InterpolatorNonUniform4F = InterpolatorNonUniform4<f32>;
    using InterpolatorNonUniform4D = InterpolatorNonUniform4<f64>;
}  // end namespace fb

#endif  // InterpolatorNonUniform4_h__
