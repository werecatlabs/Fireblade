#ifndef _ScaleAnimator3_H
#define _ScaleAnimator3_H

#include "FBCore/Animation/BaseAnimator.h"
#include <FBCore/Math/Vector3.h>

namespace fb
{

    template <class T>
    class ScaleAnimator3 : public BaseAnimator
    {
    public:
        ScaleAnimator3();

        ScaleAnimator3( T *object, f32 animationLength, const Vector3<real_Num> &startScale = Vector3<real_Num>::unit(),
                        const Vector3<real_Num> &endScale = Vector3<real_Num>::unit() );

        ~ScaleAnimator3();

        void update();

        void start();

        bool isFinished() const;

    protected:
        Vector3<real_Num> m_start;
        Vector3<real_Num> m_end;

        T *m_object;
    };

    template <class T>
    ScaleAnimator3<T>::ScaleAnimator3() :
        m_start( Vector3<real_Num>::unit() ),
        m_end( Vector3<real_Num>::unit() ),
        m_object( NULL )
    {
    }

    template <class T>
    ScaleAnimator3<T>::ScaleAnimator3( T *object, f32 animationLength,
                                       const Vector3<real_Num> &startScale = Vector3<real_Num>::unit(),
                                       const Vector3<real_Num> &endScale = Vector3<real_Num>::unit() ) :
        m_start( startScale ),
        m_end( endScale ),
        m_object( object )
    {
        setAnimationLength( animationLength );
        setAnimationTime( 0.0f );
    }

    template <class T>
    ScaleAnimator3<T>::~ScaleAnimator3()
    {
    }

    template <class T>
    void ScaleAnimator3<T>::update()
    {
        if( m_isPlaying )
        {
            m_animationTime += dt * ( 1.0f / m_animationLength );
            m_animationTime = MathF::clamp( m_animationTime, 0.0f, 1.0f );
            Vector3<real_Num> scale = m_start + ( m_end - m_start ) * m_animationTime;
            m_object->setScale( scale );
        }
    }

    template <class T>
    void ScaleAnimator3<T>::start()
    {
        BaseAnimator::start();
        m_animationTime = 0.0f;
    }

    template <class T>
    bool ScaleAnimator3<T>::isFinished() const
    {
        return m_animationTime >= ( 1.0 - MathF::epsilon() );
    }

}  // end namespace fb

#endif
