#ifndef KeyFrameTransform2_h__
#define KeyFrameTransform2_h__

#include "KeyFrame.h"
#include "FBCore/Math/Vector2.h"

namespace fb
{

    class KeyFrameTransform2 : public KeyFrame
    {
    public:
        KeyFrameTransform2();

        ~KeyFrameTransform2();

        Vector2<real_Num> getPosition() const;

        void setPosition( const Vector2<real_Num>& position );

        Vector2<real_Num> getScale() const;

        void setScale( const Vector2<real_Num>& scale );

        f32 getRotation() const;

        void setRotation( f32 rotation );

    private:
        ///
        Vector2<real_Num> m_position;

        ///
        Vector2<real_Num> m_scale;

        ///
        f32 m_rotation;
    };

}  // end namespace fb

#endif  // KeyFrameTransform2_h__
