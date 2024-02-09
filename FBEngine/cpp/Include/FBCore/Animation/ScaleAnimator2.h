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
        ScaleAnimator2();

        ScaleAnimator2( T *object, f32 animationLength, const Vector2<real_Num> &startScale = Vector2<real_Num>::UNIT,
                        const Vector2<real_Num> &endScale = Vector2<real_Num>::UNIT );

        ~ScaleAnimator2();

        void update();

    protected:
        Vector2<real_Num> m_start;
        Vector2<real_Num> m_end;

        T *m_object;
    };

    template <class T>
    ScaleAnimator2<T>::ScaleAnimator2() :
        m_start( Vector2<real_Num>::UNIT ),
        m_end( Vector2<real_Num>::UNIT ),
        m_object( NULL )
    {
    }

    template <class T>
    ScaleAnimator2<T>::ScaleAnimator2( T *object, f32 animationLength,
                                       const Vector2<real_Num> &startScale = Vector2<real_Num>::UNIT,
                                       const Vector2<real_Num> &endScale = Vector2<real_Num>::UNIT ) :
        m_start( startScale ),
        m_end( endScale ),
        m_object( object ),
        m_animationLength( animationLength ),
        m_animationTime( 0.0f )
    {
    }

    template <class T>
    ScaleAnimator2<T>::~ScaleAnimator2()
    {
    }

    template <class T>
    void ScaleAnimator2<T>::update()
    {
        m_animationTime += dt;
        MathF::clamp( m_animationTime, 0.0f, 1.0f );
        Vector2<real_Num> scale = m_start + ( m_end - m_start ) * ( m_animationTime * 1.0f / m_animationLength );
        m_object->setScale( scale );
    }

}  // end namespace fb

#endif
