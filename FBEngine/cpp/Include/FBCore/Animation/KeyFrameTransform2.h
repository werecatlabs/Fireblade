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

        Vector2F getPosition() const;

        void setPosition( const Vector2F& position );

        Vector2F getScale() const;

        void setScale( const Vector2F& scale );

        f32 getRotation() const;

        void setRotation( f32 rotation );

    private:
        ///
        Vector2F m_position;

        ///
        Vector2F m_scale;

        ///
        f32 m_rotation;
    };

}  // end namespace fb

#endif  // KeyFrameTransform2_h__
