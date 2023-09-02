#include <FBCore/FBCorePCH.h>
#include "FBCore/Scene/Components/Camera/EditorCameraController.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {

        EditorCameraController::EditorCameraController() :
            m_moveSpeed( 1.0f ),
            m_rotationSpeed( 1.0f ),
            m_bInvert( false )
        {
            m_mouseKeys.resize( 3 );
            m_cursorKeys.resize( 6 );
            m_translationSpeed = 100.0f;

            for( u32 i = 0; i < m_mouseKeys.size(); ++i )
            {
                m_mouseKeys[i] = false;
            }

            // create default key map
            m_keyMap.push_back( SCamKeyMap( 0, static_cast<u32>( KeyCodes::KEY_UP ),
                                            static_cast<u32>( KeyCodes::KEY_KEY_W ) ) );
            m_keyMap.push_back( SCamKeyMap( 1, static_cast<u32>( KeyCodes::KEY_DOWN ),
                                            static_cast<u32>( KeyCodes::KEY_KEY_S ) ) );
            m_keyMap.push_back( SCamKeyMap( 2, static_cast<u32>( KeyCodes::KEY_LEFT ),
                                            static_cast<u32>( KeyCodes::KEY_KEY_A ) ) );
            m_keyMap.push_back( SCamKeyMap( 3, static_cast<u32>( KeyCodes::KEY_RIGHT ),
                                            static_cast<u32>( KeyCodes::KEY_KEY_D ) ) );

            allKeysUp();

            auto applicationManager = core::IApplicationManager::instance();
            auto inputManager = applicationManager->getInputDeviceManager();
            //inputManager->addListener( SmartPtr<IEventListener>( new FPSCameraInputListener( this ) ) );
        }

        EditorCameraController::~EditorCameraController()
        {
        }

        void EditorCameraController::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
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
                position += targetDirection * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;

            if( m_cursorKeys[1] )
                position -= targetDirection * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;

            // strafing
            Vector3F upVector = orientation * Vector3F::UNIT_Y;
            Vector3F strafevect = targetDirection.crossProduct( upVector );
            strafevect.normalise();
            if( m_cursorKeys[2] )
                position -= strafevect * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;

            if( m_cursorKeys[3] )
                position += strafevect * static_cast<f32>( dt ) * m_moveSpeed * m_translationSpeed;

            m_position = position;

            for( auto camera : m_cameras )
            {
                if( camera )
                {
                    camera->getOwner()->setPosition( m_position );
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
                    camera->getOwner()->setOrientation( orientation );
                }
            }
        }

        bool EditorCameraController::OnEvent( const SmartPtr<IInputEvent> &event )
        {
            auto applicationManager = core::IApplicationManager::instance();
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
                for( u32 i = 0; i < m_keyMap.size(); ++i )
                {
                    if( m_keyMap[i].keycode == event->getKeyboardState()->getKeyCode() ||
                        m_keyMap[i].keycode1 == event->getKeyboardState()->getKeyCode() )
                    {
                        m_cursorKeys[m_keyMap[i].action] = event->getKeyboardState()->isPressedDown();
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

        Vector3F EditorCameraController::getPosition() const
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

        Vector3F EditorCameraController::getTargetPosition() const
        {
            return m_position + getOrientation() * -Vector3F::UNIT_Z;
        }

        void EditorCameraController::setOrientation( const QuaternionF &orientation )
        {
        }

        QuaternionF EditorCameraController::getOrientation() const
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

        Vector3F EditorCameraController::getDirection() const
        {
            return getOrientation() * -Vector3F::UNIT_Z;
        }

        Vector3F EditorCameraController::getUp() const
        {
            return Vector3F::zero();
        }

        void EditorCameraController::setPropertyValue( const String &name, const String &value )
        {
            if( name == ( "MoveSpeed" ) )
            {
                m_translationSpeed = StringUtil::parseFloat( value );
            }
            else if( name == ( "FarDistance" ) )
            {
            }
        }

        void EditorCameraController::setProperties( const Properties &propertyGroup )
        {
            Vector3F position;

            Array<Property> properties = propertyGroup.getPropertiesAsArray();

            // loop through the keys and set the variables
            for( u32 i = 0; i < properties.size(); ++i )
            {
                const Property &key = properties[i];
                if( key.getName() == ( "Position" ) )
                {
                    position = StringUtil::parseVector3<f32>( key.getValue() );
                }
            }

            setPosition( position );
            setTargetPosition( position + Vector3F( 0, 0, 1 ) );

            m_propertyGroup = propertyGroup;
        }

        void EditorCameraController::getProperties( Properties &propertyGroup ) const
        {
            propertyGroup = m_propertyGroup;
            propertyGroup.setProperty( "Position", StringUtil::toString( m_position ) );
        }

        bool EditorCameraController::isMouseKeyDown( s32 key )
        {
            return m_mouseKeys[key];
        }

        void EditorCameraController::allKeysUp()
        {
            for( u32 i = 0; i < 6; ++i )
                m_cursorKeys[i] = false;
        }

        Vector3F EditorCameraController::getRight() const
        {
            return Vector3F::zero();
        }

        Ray3F EditorCameraController::getCameraToViewportRay( const Vector2F &screenPosition ) const
        {
            Ray3F ray;
            /*m_selectedCamera->getCameraToViewportRay()

            if(m_selectedCamera)
            {
                Viewport* viewport = m_selectedCamera->getViewport();
                f32 width = viewport->getActualWidth();
                f32 height = viewport->getActualHeight();

                Vector2F normalizedScreenCoords;
                normalizedScreenCoords.X() = screenPosition.X() / width;
                normalizedScreenCoords.Y() = screenPosition.Y() / height;

                Ogre::Ray ogreRay = m_selectedCamera->getCameraToViewportRay(normalizedScreenCoords.X(),
            normalizedScreenCoords.Y());

                Ogre::Vector3 ogreCameraPosition = m_selectedCamera->getDerivedPosition();
                Ogre::Vector3 rayOrigin = ogreRay.getOrigin();
                Ogre::Vector3 rayDirection = ogreRay.getDirection();
                ray.Start = Vector3F(&rayOrigin[0]);
                ray.Direction = Vector3F(&rayDirection[0]);

                ray.Start.Z() = -ray.Start.Z();
                ray.Direction.Z() = -ray.Direction.Z();
            }*/

            return ray;
        }

        bool EditorCameraController::isInFrustum( const AABB3F &box ) const
        {
            return true;
        }

        void EditorCameraController::addCamera( SmartPtr<render::ICamera> camera )
        {
            m_cameras.push_back( camera );
        }

        bool EditorCameraController::removeCamera( SmartPtr<render::ICamera> camera )
        {
            for( u32 i = 0; i < m_cameras.size(); ++i )
            {
                if( m_cameras[i] == camera )
                {
                    // m_cameras.erase_element_index(i);
                    return true;
                }
            }

            return false;
        }

        f32 EditorCameraController::getRotationSpeed() const
        {
            return m_rotationSpeed;
        }

        void EditorCameraController::setRotationSpeed( f32 val )
        {
            m_rotationSpeed = val;
        }

        bool EditorCameraController::getInvert() const
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

        bool EditorCameraController::FPSCameraInputListener::inputEvent( SmartPtr<IInputEvent> event )
        {
            return m_fpsCameraCtrl->OnEvent( event );
        }

        bool EditorCameraController::FPSCameraInputListener::updateEvent(
            const SmartPtr<IInputEvent> &event )
        {
            return false;
        }

        s32 EditorCameraController::FPSCameraInputListener::getPriority() const
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

        EditorCameraController::SCamKeyMap::SCamKeyMap()
        {
        }
    }  // namespace scene
}  // end namespace fb
