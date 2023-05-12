#ifndef LinearSpline4_h__
#define LinearSpline4_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    //------------------------------------------------
    template <class T>
    class LinearSpline4 : public CSharedObject<ISharedObject>
    {
    public:
        LinearSpline4()
        {
        }

        ~LinearSpline4() override
        {
        }

        u32 getNumPoints() const;

    protected:
    };

    using LinearSpline4F = LinearSpline4<f32>;
    using LinearSpline4D = LinearSpline4<f64>;
}  // namespace fb

#endif  // LinearSpline4_h__
