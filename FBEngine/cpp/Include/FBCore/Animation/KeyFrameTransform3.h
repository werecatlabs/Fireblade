#ifndef KeyFrameTransform3_h__
#define KeyFrameTransform3_h__

#include "KeyFrame.h"

namespace fb
{

    //---------------------------------------------
    class KeyFrameTransform3 : public KeyFrame
    {
    public:
        KeyFrameTransform3()
        {
        }

        KeyFrameTransform3( f32 time, const Vector3F &position, const QuaternionF &orientation,
                            const Vector3F &scale ) :
            Orientation( orientation ),
            Position( position ),
            Scale( scale )
        {
            m_time = time;
        }

        ~KeyFrameTransform3()
        {
        }

        Vector3F getPosition() const
        {
            return Position;
        }
        void setPosition( Vector3F val )
        {
            Position = val;
        }

        QuaternionF getOrientation() const
        {
            return Orientation;
        }
        void setOrientation( QuaternionF val )
        {
            Orientation = val;
        }

        Vector3F getScale() const
        {
            return Scale;
        }
        void setScale( Vector3F val )
        {
            Scale = val;
        }

    private:
        QuaternionF Orientation;
        Vector3F Position;
        Vector3F Scale;
    };

    typedef SmartPtr<KeyFrameTransform3> KeyFrameTransform3Ptr;

}  // end namespace fb

#endif  // KeyFrameTransform3_h__
