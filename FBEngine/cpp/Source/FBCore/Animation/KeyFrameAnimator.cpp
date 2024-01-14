#include <FBCore/FBCorePCH.h>
#include <FBCore/Animation/KeyFrameAnimator3.h>
#include <FBCore/Math/Math.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/ITimer.h>

namespace fb
{

    KeyFrameAnimator3::KeyFrameAnimator3()
    {
        setAnimationTime( 0.f );
        setAnimationLength( 0.f );

        m_currentKeyFrame = SmartPtr<KeyFrameTransform3>( new KeyFrameTransform3 );
        m_animationLength = 1.0f / 60.0f;
    }

    KeyFrameAnimator3::~KeyFrameAnimator3() = default;

    void KeyFrameAnimator3::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        m_animationTime = m_animationTime + ( static_cast<f32>( dt ) * m_animationLength );
        m_animationTime = MathF::Mod( m_animationTime, 1.0f );

        //m_currentKeyFrame->setPosition(m_positionSpline.interpolate(m_animationTime));
        m_currentKeyFrame->setOrientation( m_orientationSpline.interpolate( m_animationTime, false ) );
    }

    void KeyFrameAnimator3::addKeyFrame( const SmartPtr<KeyFrameTransform3> &keyFrame )
    {
        m_keyFrames.push_back( keyFrame );

        //m_positionSpline.addPoint(keyFrame->getPosition());
        m_orientationSpline.addPoint( keyFrame->getOrientation() );
    }

    void KeyFrameAnimator3::addKeyFrames( const Array<SmartPtr<KeyFrameTransform3>> &keyFrames )
    {
        m_keyFrames = keyFrames;

        //m_positionSpline.setAutoCalculate(false);
        m_orientationSpline.setAutoCalculate( false );

        for( const auto &keyFrame : m_keyFrames )
        {
            //m_positionSpline.addPoint(keyFrame->getPosition());
            m_orientationSpline.addPoint( keyFrame->getOrientation() );
        }

        //m_positionSpline.recalcTangents();
        m_orientationSpline.recalcTangents();
    }

    void KeyFrameAnimator3::setAnimationLength( f32 animationLength )
    {
        m_animationLength = 1.0f / animationLength;
    }

}  // end namespace fb
