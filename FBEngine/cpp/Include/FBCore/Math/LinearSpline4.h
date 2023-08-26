#ifndef LinearSpline4_h__
#define LinearSpline4_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector4.h>

namespace fb
{
    
    template <class T>
    class LinearSpline4 : public ISharedObject
    {
    public:
        LinearSpline4()
        {
        }

        ~LinearSpline4() override
        {
        }

        u32 getNumPoints() const
        {
        	return m_points.size();
        }

    protected:
        Array<Vector4<T>> m_points;
    };

    using LinearSpline4F = LinearSpline4<f32>;
    using LinearSpline4D = LinearSpline4<f64>;
}  // namespace fb

#endif  // LinearSpline4_h__
