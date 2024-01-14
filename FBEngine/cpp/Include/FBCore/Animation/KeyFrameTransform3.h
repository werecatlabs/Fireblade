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

        KeyFrameTransform3( f32 time, const Vector3F &position, const QuaternionF &orientation,
                            const Vector3F &scale );

        ~KeyFrameTransform3();

        Vector3F getPosition() const;

        void setPosition( const Vector3F &position );

        QuaternionF getOrientation() const;

        void setOrientation( const QuaternionF &orientation );

        Vector3F getScale() const;

        void setScale( const Vector3F &scale );

    private:
        QuaternionF m_orientation;
        Vector3F m_position;
        Vector3F m_scale;
    };

}  // end namespace fb

#endif  // KeyFrameTransform3_h__
