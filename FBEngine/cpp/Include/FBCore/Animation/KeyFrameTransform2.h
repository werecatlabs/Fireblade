#ifndef KeyFrameTransform2_h__
#define KeyFrameTransform2_h__

#include "KeyFrame.h"
#include "FBCore/Math/Vector2.h"

namespace fb
{

    class KeyFrameTransform2 : public KeyFrame
    {
    public:
        KeyFrameTransform2()
        {
        }
        ~KeyFrameTransform2()
        {
        }

        Vector2F getPosition() const
        {
            return m_position;
        }
        void setPosition( fb::Vector2F val )
        {
            m_position = val;
        }

        Vector2F getScale() const
        {
            return m_scale;
        }
        void setScale( fb::Vector2F val )
        {
            m_scale = val;
        }

        f32 getRotation() const
        {
            return m_rotation;
        }
        void setRotation( f32 val )
        {
            m_rotation = val;
        }

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
