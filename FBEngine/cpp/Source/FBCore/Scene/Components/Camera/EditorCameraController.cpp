#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/EditorCameraController.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Scene/CameraManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>
#include <FBCore/Interface/Input/IInputEvent.h>
#include <FBCore/Interface/Input/IKeyboardState.h>
#include <FBCore/Interface/Input/IMouseState.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, EditorCameraController, CameraController );

    EditorCameraController::EditorCameraController() :
        m_moveSpeed( 1.0f ),
        m_rotationSpeed( 1.0f ),
        m_bInvert( false )
    {
        m_mouseKeys.resize( 3 );
        m_cursorKeys.resize( 6 );
        m_translationSpeed = 100.0f;

        for( auto &&m_mouseKey : m_mouseKeys )
        {
            m_mouseKey = false;
        }

        // create default key map
        m_keyMap.emplace_back( 0, static_cast<u32>( KeyCodes::KEY_UP ),
                               static_cast<u32>( KeyCodes::KEY_KEY_W ) );
        m_keyMap.emplace_back( 1, static_cast<u32>( KeyCodes::KEY_DOWN ),
                               static_cast<u32>( KeyCodes::KEY_KEY_S ) );
        m_keyMap.emplace_back( 2, static_cast<u32>( KeyCodes::KEY_LEFT ),
                               static_cast<u32>( KeyCodes::KEY_KEY_A ) );
        m_keyMap.emplace_back( 3, static_cast<u32>( KeyCodes::KEY_RIGHT ),
                               static_cast<u32>( KeyCodes::KEY_KEY_D ) );

        allKeysUp();

        auto applicationManager = core::ApplicationManager::instance();
        auto inputManager = applicationManager->getInputDeviceManager();
        //inputManager->addListener( SmartPtr<IEventListener>( new FPSCameraInputListener( this ) ) );
    }

    EditorCameraController::~EditorCameraController() = default;

    void EditorCameraController::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto timer = applicationManager->getTimer();

        auto task = Thread::getCurrentTask();
        auto t = timer->getTime();
        auto dt = timer->getDeltaTime();

        // auto cameraManager = applicationManager->getCameraManager();
        // if (cameraManager)
        //{
        //	if (cameraManager->getCurrentCamera() != SmartPtr<scene::CameraController>(this))
        //		return;
        // }

        Vector3F position = m_position;
        QuaternionF orientation = getOrientation();

        Vector3F targetDirection = orientation * -Vector3F::UNIT_Z;

        if( m_cursorKeys[0] )
        {
            position += targetDirection * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;
        }

        if( m_cursorKeys[1] )
        {
            position -= targetDirection * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;
        }

        // strafing
        Vector3F upVector = orientation * Vector3F::UNIT_Y;
        Vector3F strafevect = targetDirection.crossProduct( upVector );
        strafevect.normalise();
        if( m_cursorKeys[2] )
        {
            position -= strafevect * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;
        }

        if( m_cursorKeys[3] )
        {
            position += strafevect * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;
        }

        m_position = position;

        for( auto camera : m_cameras )
        {
            if( camera )
            {
                //actor->setPosition( m_position );
            }
        }

        if( m_relativeMouse != Vector2F::ZERO )
        {
            if( m_bInvert )
            {
                m_rotation.X() -= m_relativeMouse.Y() * m_rotationSpeed * static_cast<f32>( dt );
                m_rotation.Y() -= m_relativeMouse.X() * m_rotationSpeed * static_cast<f32>( dt );
            }
            else
            {
                m_rotation.X() += m_relativeMouse.Y() * m_rotationSpeed * static_cast<f32>( dt );
                m_rotation.Y() += m_relativeMouse.X() * m_rotationSpeed * static_cast<f32>( dt );
            }

            m_relativeMouse = Vector2F::ZERO;
        }

        for( auto camera : m_cameras )
        {
            if( camera )
            {
                //actor->setOrientation( orientation );
            }
        }
    }

    auto EditorCameraController::handleInputEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto cameraManager = applicationManager->getCameraManager();
        // if (cameraManager)
        //{
        //	if (cameraManager->getCurrentCamera() != SmartPtr<scene::CameraController>(this))
        //		return false;
        // }

        bool eventHandled = false;

        switch( event->getEventType() )
        {
        case IInputEvent::EventType::Key:
        {
            for( auto &i : m_keyMap )
            {
                if( i.keycode == event->getKeyboardState()->getKeyCode() ||
                    i.keycode1 == event->getKeyboardState()->getKeyCode() )
                {
                    m_cursorKeys[i.action] = event->getKeyboardState()->isPressedDown();
                }
            }
        }
        break;
        case IInputEvent::EventType::Mouse:
        {
            switch( event->getMouseState()->getEventType() )
            {
            case IMouseState::Event::LeftPressed:
                m_mouseKeys[0] = true;
                break;
            case IMouseState::Event::RightPressed:
                m_prevCursorPos = m_cursorPos;
                m_mouseKeys[2] = true;
                break;
            case IMouseState::Event::MiddlePressed:
                m_mouseKeys[1] = true;
                break;
            case IMouseState::Event::LeftReleased:
                m_mouseKeys[0] = false;
                break;
            case IMouseState::Event::RightReleased:
                m_mouseKeys[2] = false;
                break;
            case IMouseState::Event::MiddleReleased:
                m_mouseKeys[1] = false;
                break;
            case IMouseState::Event::Moved:
            {
                // Update rotation
                if( isMouseKeyDown( 2 ) )
                {
                    m_relativeMouse = event->getMouseState()->getRelativePosition();
                    eventHandled = true;
                }
            }
            break;
            case IMouseState::Event::Wheel:
            case IMouseState::Event::Count:
            {
            }
            }
        }
        break;
        default:
            break;
        }

        return eventHandled;
    }

    void EditorCameraController::setPosition( const Vector3F &position )
    {
        m_position = position;
    }

    auto EditorCameraController::getPosition() const -> Vector3F
    {
        return m_position;
    }

    void EditorCameraController::setTargetPosition( const Vector3F &position )
    {
        Vector3F direction = ( position - m_position ).normaliseCopy();

        Matrix3F currentRot;
        currentRot.fromEulerAnglesXYZ( m_rotation.X(), m_rotation.Y(), m_rotation.Z() );

        QuaternionF orientation;
        orientation.toRotationMatrix( currentRot );

        Vector3F currentDirection = orientation * -Vector3F::UNIT_Z;
        QuaternionF orientationDiff;  // = MathUtilF::getRotationTo(currentDirection, direction);

        QuaternionF newRot = orientationDiff * orientation;

        Vector3F newDirection_ = newRot * -Vector3F::UNIT_Z;
        if( newDirection_ != direction )
        {
            int halt = 0;
            halt = 0;
        }

        Matrix3F rot;
        newRot.toRotationMatrix( rot );
        rot.toEulerAnglesXYZ( m_rotation.X(), m_rotation.Y(), m_rotation.Z() );
        // m_rotation *= MathF::RAD_TO_DEG;
    }

    auto EditorCameraController::getTargetPosition() const -> Vector3F
    {
        return m_position + getOrientation() * -Vector3F::UNIT_Z;
    }

    void EditorCameraController::setOrientation( const QuaternionF &orientation )
    {
    }

    auto EditorCameraController::getOrientation() const -> QuaternionF
    {
        QuaternionF yaw;
        yaw.fromAngleAxis( MathF::DegToRad( m_rotation.Y() ), Vector3F::UNIT_Y );

        QuaternionF pitch;
        pitch.fromAngleAxis( MathF::DegToRad( m_rotation.X() ), Vector3F::UNIT_X );

        // the orientation
        return yaw * pitch;
    }

    void EditorCameraController::setDirection( const Vector3F &direction )
    {
    }

    auto EditorCameraController::getDirection() const -> Vector3F
    {
        return getOrientation() * -Vector3F::UNIT_Z;
    }

    auto EditorCameraController::getUp() const -> Vector3F
    {
        return Vector3F::zero();
    }

    auto EditorCameraController::isMouseKeyDown( s32 key ) -> bool
    {
        return m_mouseKeys[key];
    }

    void EditorCameraController::allKeysUp()
    {
        for( u32 i = 0; i < 6; ++i )
        {
            m_cursorKeys[i] = false;
        }
    }

    auto EditorCameraController::getRight() const -> Vector3F
    {
        return Vector3F::zero();
    }

    void EditorCameraController::addCamera( SmartPtr<render::ICamera> camera )
    {
        m_cameras.push_back( camera );
    }

    auto EditorCameraController::removeCamera( SmartPtr<render::ICamera> camera ) -> bool
    {
        for( const auto &m_camera : m_cameras )
        {
            if( m_camera == camera )
            {
                // m_cameras.erase_element_index(i);
                return true;
            }
        }

        return false;
    }

    auto EditorCameraController::getRotationSpeed() const -> f32
    {
        return m_rotationSpeed;
    }

    void EditorCameraController::setRotationSpeed( f32 val )
    {
        m_rotationSpeed = val;
    }

    auto EditorCameraController::getInvert() const -> bool
    {
        return m_bInvert;
    }

    void EditorCameraController::setInvert( bool val )
    {
        m_bInvert = val;
    }

    EditorCameraController::FPSCameraInputListener::FPSCameraInputListener(
        EditorCameraController *fpsCameraCtrl ) :
        m_fpsCameraCtrl( fpsCameraCtrl )
    {
    }

    auto EditorCameraController::FPSCameraInputListener::inputEvent( SmartPtr<IInputEvent> event )
        -> bool
    {
        return m_fpsCameraCtrl->handleInputEvent( event );
    }

    auto EditorCameraController::FPSCameraInputListener::updateEvent(
        const SmartPtr<IInputEvent> &event ) -> bool
    {
        return false;
    }

    auto EditorCameraController::FPSCameraInputListener::getPriority() const -> s32
    {
        return 0;
    }

    void EditorCameraController::FPSCameraInputListener::setPriority( s32 priority )
    {
    }

    EditorCameraController::SCamKeyMap::SCamKeyMap( s32 a, s32 k, s32 k1 ) :
        action( a ),
        keycode( k ),
        keycode1( k1 )
    {
    }

    EditorCameraController::SCamKeyMap::SCamKeyMap() = default;
}  // namespace fb::scene
