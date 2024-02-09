#ifndef KeyFrameTransform3_h__
#define KeyFrameTransform3_h__

#include "KeyFrame.h"
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Math/Vector3.h"

namespace fb
{

    class KeyFrameTransform3 : public KeyFrame
    {
    public:
        KeyFrameTransform3();

        KeyFrameTransform3( f32 time, const Vector3<real_Num> &position, const Quaternion<real_Num> &orientation,
                            const Vector3<real_Num> &scale );

        ~KeyFrameTransform3();

        Vector3<real_Num> getPosition() const;

        void setPosition( const Vector3<real_Num> &position );

        Quaternion<real_Num> getOrientation() const;

        void setOrientation( const Quaternion<real_Num> &orientation );

        Vector3<real_Num> getScale() const;

        void setScale( const Vector3<real_Num> &scale );

    private:
        Quaternion<real_Num> m_orientation;
        Vector3<real_Num> m_position;
        Vector3<real_Num> m_scale;
    };

}  // end namespace fb

#endif  // KeyFrameTransform3_h__
