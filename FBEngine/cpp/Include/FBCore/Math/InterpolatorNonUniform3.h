#ifndef InterpolatorNonUniform3_h__
#define InterpolatorNonUniform3_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    //------------------------------------------------
    template <class T>
    class InterpolatorNonUniform3
    {
    public:
        InterpolatorNonUniform3();
        ~InterpolatorNonUniform3();

        void setValues( const Array<std::pair<T, Vector3<T>>> &values );

        const Array<std::pair<T, Vector3<T>>> &getValues() const;

        Vector3<T> interpolate( const T &t );

    protected:
        Array<std::pair<T, Vector3<T>>> m_values;
    };

    using InterpolatorNonUniform4F = InterpolatorNonUniform3<f32>;
    using InterpolatorNonUniform4D = InterpolatorNonUniform3<f64>;
}  // end namespace fb

#endif  // InterpolatorNonUniform3_h__
