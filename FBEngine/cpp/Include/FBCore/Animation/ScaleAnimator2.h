#ifndef _ScaleAnimator2_H
#define _ScaleAnimator2_H

#include "FBCore/Animation/BaseAnimator.h"
#include "FBCore/Math/Vector2.h"

namespace fb
{

    template <class T>
    class ScaleAnimator2 : public BaseAnimator
    {
    public:
        ScaleAnimator2() : m_start( Vector2F::UNIT ), m_end( Vector2F::UNIT ), m_object( NULL )
        {
        }

        ScaleAnimator2( T *object, f32 animationLength, const Vector2F &startScale = Vector2F::UNIT,
                        const Vector2F &endScale = Vector2F::UNIT ) :
            m_start( startScale ),
            m_end( endScale ),
            m_object( object ),
            m_animationLength( animationLength ),
            m_animationTime( 0.0f )
        {
            setAnimationTime( 0.f );
            setAnimationLength( animationLength );
        }

        ~ScaleAnimator2()
        {
        }

        void update()
        {
            m_animationTime += dt;
            MathF::clamp( m_animationTime, 0.0f, 1.0f );
            Vector2F scale =
                m_start + ( m_end - m_start ) * ( m_animationTime * 1.0f / m_animationLength );
            m_object->setScale( scale );
        }

    protected:
        Vector2F m_start;
        Vector2F m_end;

        T *m_object;
    };

}  // end namespace fb

#endif