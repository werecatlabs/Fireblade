#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/SplineCameraCtrl.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {

        SplineCameraCtrl::SplineCameraCtrl() : m_depthOffset( 0.0f )
        {
            // m_fsm = SmartPtr<IFSM>(new FSMStandard, true);
        }

        SplineCameraCtrl::~SplineCameraCtrl()
        {
        }

        void SplineCameraCtrl::initialise( SmartPtr<render::ICamera> camera,
                                           SmartPtr<render::ISceneNode> cameraSceneNode )
        {
            if( !camera )
            {
                FB_LOG_MESSAGE( "Camera", "Warning: Camera null." );
            }

            if( !cameraSceneNode )
            {
                FB_LOG_MESSAGE( "Camera", "Warning: Camera node null." );
            }

            m_cameraSceneNode = cameraSceneNode;

            setCamera( camera );

            m_depthOffset = 0.0f;
            m_offsetPosition = Vector2F( 0.5f, 0.5f );

            m_animators.resize( static_cast<size_t>( SplineCameraAnimatorId::SCAI_COUNT ) );
            m_animators[static_cast<int>( SplineCameraAnimatorId::SCAI_DEPTH_VALUE )] =
                SmartPtr<IAnimator>( new InterpolateFloat( 1.0f, m_depthOffset, m_depthOffset ) );
            m_animators[static_cast<int>( SplineCameraAnimatorId::SCAI_OFFSET_POSITION )] =
                SmartPtr<IAnimator>(
                    new InterpolateVector2f( 1.0f, m_offsetPosition, m_offsetPosition ) );

            m_fsm->setState( States::SCS_IN_GAME );
        }

        void SplineCameraCtrl::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            m_fsm->update();

            if( m_fsm->getState<States>() != States::SCS_IN_GAME )
            {
                return;
            }

            switch( task )
            {
            case Thread::Task::Render:
            {
            }
            break;
            default:
            {
            }
            }

            /*
            if(m_proceduralSpline)
            {
                m_position = m_proceduralSpline->getPosition() + m_offset;
                m_orientation.fromDegrees(m_proceduralSpline->getRotation());
            }
            */

            InterpolateFloatPtr depthAnimator;  // = m_animators[SCAI_DEPTH_VALUE];
            if( depthAnimator->isPlaying() )
            {
                depthAnimator->update();
                m_depthOffset = depthAnimator->getValue();

                if( depthAnimator->isPlaying() && depthAnimator->isReverse() &&
                    depthAnimator->isFinished() )
                {
                    depthAnimator->stop();
                }

                if( depthAnimator->isPlaying() && depthAnimator->isFinished() )
                {
                    depthAnimator->setReverse( true );
                }
            }

            InterpolateVector2fPtr positionAnimator;  // = m_animators[SCAI_OFFSET_POSITION];
            if( positionAnimator->isPlaying() )
            {
                positionAnimator->update();
                m_offsetPosition = positionAnimator->getValue();

                if( positionAnimator->isPlaying() && positionAnimator->isReverse() &&
                    positionAnimator->isFinished() )
                {
                    positionAnimator->stop();
                }

                if( positionAnimator->isPlaying() && positionAnimator->isFinished() )
                {
                    positionAnimator->setReverse( true );
                }
            }

            m_up = m_orientation * Vector3F::UNIT_Y;
            m_right = m_orientation * Vector3F::UNIT_X;
            m_direction = m_orientation * -Vector3F::UNIT_Z;

            Vector3F cameraPosition = getPosition() + getDirection() * m_depthOffset;
            cameraPosition += -m_up * ( m_offsetPosition.Y() - 0.5f ) * 115.f;
            cameraPosition += m_right * ( m_offsetPosition.X() - 0.5f ) * 115.0f;

            m_cameraSceneNode->setPosition( cameraPosition );
            m_cameraSceneNode->setOrientation( m_orientation );
        }

        void SplineCameraCtrl::setPosition( const Vector3F &position )
        {
            m_position = position;

            // if(m_proceduralSpline)
            //	m_proceduralSpline->setPosition(position);
        }

        Vector3F SplineCameraCtrl::getPosition() const
        {
            return m_position;
        }

        void SplineCameraCtrl::setRotation( const Vector3F &rotation )
        {
            // m_proceduralSpline->setRotation(rotation);
        }

        Vector3F SplineCameraCtrl::getRotation() const
        {
            return Vector3F::zero();  // m_proceduralSpline->getRotation();
        }

        void SplineCameraCtrl::setTargetPosition( const Vector3F &position )
        {
        }

        Vector3F SplineCameraCtrl::getTargetPosition() const
        {
            return Vector3F::zero();
        }

        void SplineCameraCtrl::setOrientation( const QuaternionF &orientation )
        {
            m_cameraSceneNode->setOrientation( orientation );
        }

        QuaternionF SplineCameraCtrl::getOrientation() const
        {
            return m_orientation;
        }

        void SplineCameraCtrl::setDirection( const Vector3F &direction )
        {
        }

        Vector3F SplineCameraCtrl::getDirection() const
        {
            return m_direction;
        }

        Vector3F SplineCameraCtrl::getUp() const
        {
            return m_up;
        }

        Vector3F SplineCameraCtrl::getRight() const
        {
            return m_right;
        }

        void SplineCameraCtrl::setPropertyValue( const String &name, const String &value )
        {
        }

        void SplineCameraCtrl::setProperties( const Properties &propertyGroup )
        {
        }

        void SplineCameraCtrl::getProperties( Properties &propertyGroup ) const
        {
        }

        /*

        void SplineCameraCtrl::setProceduralSpline(const ProceduralSpline3fPtr& spline)
        {
            m_proceduralSpline = spline;
        }




        ProceduralSpline3fPtr& SplineCameraCtrl::getProceduralSpline() const
        {
            return m_proceduralSpline.removeConst();
        }
        */

        void SplineCameraCtrl::setSplineTime( f32 splineTime )
        {
        }

        f32 SplineCameraCtrl::getSplineTime() const
        {
            return 0.f;
        }

        void SplineCameraCtrl::_OnsetActive( bool active )
        {
        }

        void SplineCameraCtrl::setSplineSpeed( f32 speed )
        {
        }

        f32 SplineCameraCtrl::getSplineSpeed() const
        {
            return 0.f;
        }

        void SplineCameraCtrl::setTarget( Vector3F vTargetPos )
        {
        }

        void SplineCameraCtrl::setUpdate( bool shouldUpdate )
        {
        }

        bool SplineCameraCtrl::shouldUpdate()
        {
            return false;
        }

        void SplineCameraCtrl::setVelocity( const Vector3F &velocity )
        {
            m_velocity = velocity;
        }

        const Vector3F &SplineCameraCtrl::getVelocity() const
        {
            return m_velocity;
        }

        f32 SplineCameraCtrl::getDistanceTravelled() const
        {
            return 0.f;
        }

        void SplineCameraCtrl::setLinearTarget( const Vector3F &linearTarget )
        {
        }

        const Vector3F &SplineCameraCtrl::getLinearTarget() const
        {
            static Vector3F v = Vector3F::zero();
            return v;
        }

        void SplineCameraCtrl::setAngularTarget( const Vector3F &angularTarget )
        {
        }

        const Vector3F &SplineCameraCtrl::getAngularTarget() const
        {
            static Vector3F v = Vector3F::zero();
            return v;
        }

        void SplineCameraCtrl::setLinearSpeed( f32 speed )
        {
        }

        f32 SplineCameraCtrl::getLinearSpeed() const
        {
            return 0.f;
        }

        void SplineCameraCtrl::setAngularSpeed( f32 speed )
        {
        }

        f32 SplineCameraCtrl::getAngularSpeed() const
        {
            return 0.f;
        }

        void SplineCameraCtrl::selectNextNode()
        {
        }

        void SplineCameraCtrl::selectPrevNode()
        {
        }

        void SplineCameraCtrl::nextNode()
        {
        }

        void SplineCameraCtrl::prevNode()
        {
        }

        void SplineCameraCtrl::setShouldUpdateCameraPosition( bool status )
        {
        }

        bool SplineCameraCtrl::getShouldUpdateCameraPosition() const
        {
            return false;
        }

        void SplineCameraCtrl::setTotalSplineFollowTime( f32 totalTime )
        {
        }

        f32 SplineCameraCtrl::getTotalSplineFollowTime() const
        {
            return 0.f;
        }

        f32 SplineCameraCtrl::getCurSplineTime() const
        {
            return 0.f;
        }

        f32 SplineCameraCtrl::getCurSplineDistance() const
        {
            return 0.f;
        }

        f32 SplineCameraCtrl::getTotalSplineDistance() const
        {
            return 0.f;
        }

        void SplineCameraCtrl::setSplineOffset( const Vector3F &offset )
        {
            m_offset = offset;
        }

        const Vector3F &SplineCameraCtrl::getSplineOffset() const
        {
            return m_offset;
        }

        void SplineCameraCtrl::setSplineName( const String &splineName )
        {
        }

        void SplineCameraCtrl::setDepthOffset( f32 offset )
        {
            m_depthOffset = offset;

            InterpolateFloatPtr depthAnimator;  // = m_animators[SCAI_DEPTH_VALUE];
            depthAnimator->setEndValue( m_depthOffset );
            depthAnimator->setReverse( false );
            depthAnimator->start();
        }

        f32 SplineCameraCtrl::getDepthOffset() const
        {
            return m_depthOffset;
        }

        void SplineCameraCtrl::setOffsetPosition( const Vector2F &offsetPosition )
        {
            m_offsetPosition = offsetPosition;

            InterpolateVector2fPtr positionAnimator;  // = m_animators[SCAI_OFFSET_POSITION];
            positionAnimator->setEndValue( m_offsetPosition );
            positionAnimator->setReverse( false );
            positionAnimator->start();
        }

        Vector2F SplineCameraCtrl::getOffsetPosition() const
        {
            return m_offsetPosition;
        }
    }  // namespace scene
}  // end namespace fb
