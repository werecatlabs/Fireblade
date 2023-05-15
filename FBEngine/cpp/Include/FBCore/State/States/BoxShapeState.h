#ifndef BoxShapeState_h__
#define BoxShapeState_h__

#include <FBCore/State/States/ShapeState.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    class BoxShapeState : public ShapeState
    {
    public:
        BoxShapeState();
        ~BoxShapeState() override;

        Vector3<real_Num> getExtents() const;
        void setExtents( const Vector3<real_Num> &extents );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector3<real_Num> m_extents = Vector3<real_Num>::unit();
    };
}  // end namespace fb

#endif  // BoxShapeState_h__
