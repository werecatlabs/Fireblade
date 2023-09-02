#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/FPSCameraController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {

        FpsCameraController::FpsCameraController() :
            m_selectedCamera( nullptr ),
            MoveSpeed( 1.0f ),
            m_rotationSpeed( 10.01f )
        {
            MouseKeys.resize( 3 );
            CursorKeys.resize( 6 );
            m_translationSpeed = 100.0f;

            for( u32 i = 0; i < MouseKeys.size(); ++i )
            {
                MouseKeys[i] = false;
            }

            // create default key map
            KeyMap.push_back( SCamKeyMap( 0, static_cast<u32>( KeyCodes::KEY_UP ),
                                          static_cast<u32>( KeyCodes::KEY_KEY_W ) ) );
            KeyMap.push_back( SCamKeyMap( 1, static_cast<u32>( KeyCodes::KEY_DOWN ),
                                          static_cast<u32>( KeyCodes::KEY_KEY_S ) ) );
            KeyMap.push_back( SCamKeyMap( 2, static_cast<u32>( KeyCodes::KEY_LEFT ),
                                          static_cast<u32>( KeyCodes::KEY_KEY_A ) ) );
            KeyMap.push_back( SCamKeyMap( 3, static_cast<u32>( KeyCodes::KEY_RIGHT ),
                                          static_cast<u32>( KeyCodes::KEY_KEY_D ) ) );

            allKeysUp();

            m_targetDirection = Vector3F::UNIT_Z;
        }

        FpsCameraController::~FpsCameraController()
        {
        }

        void FpsCameraController::initialise()
        {
            // String cameraName = "FPSCameraController";
            // setName( cameraName );
            // m_propertyGroup.setProperty( "Name", cameraName );
        }

        void FpsCameraController::update()
        {
            /*if(!m_selectedCamera)
            return;

            if(IApplicationManager::instance()->getCameraManager()->getCurrentCamera() !=
            SmartPtr<scene::CameraController>(this)) return;

            Vector3F position = m_position;

            if (CursorKeys[0])
            position += m_targetDirection * dt * MoveSpeed * m_translationSpeed;

            if (CursorKeys[1])
            position -= m_targetDirection * dt * MoveSpeed * m_translationSpeed;

            // strafing
            Ogre::Vector3 ogreCamUp = m_selectedCamera->getDerivedUp();
            Vector3F upVector(&ogreCamUp[0]);
            upVector.Z() = -upVector.Z();

            Vector3F strafevect = m_targetDirection.crossProduct(upVector);
            strafevect.normalise();
            if (CursorKeys[2])
            position += strafevect * dt * MoveSpeed * m_translationSpeed;

            if (CursorKeys[3])
            position -= strafevect * dt * MoveSpeed * m_translationSpeed;

            m_position = position;

            Vector3F ogreCameraPosition = m_position;
            ogreCameraPosition.Z() = -ogreCameraPosition.Z();
            m_selectedCamera->setPosition(Ogre::Vector3(ogreCameraPosition));	*/
        }

        bool FpsCameraController::OnEvent( const SmartPtr<IInputEvent> &event )
        {
            bool eventHandled = false;

            /*if(event.UserData)
            {
            m_selectedCamera = (SmartPtr<ICamera>)event.UserData;
            }

            switch(event->getEventType())
            {
            case EET_KEY_INPUT_EVENT:
            for (u32 i=0; i<KeyMap.size(); ++i)
            {
            if (KeyMap[i].keycode == event.KeyInput.Key || KeyMap[i].keycode1 == event.KeyInput.Key)
            {
            CursorKeys[KeyMap[i].action] = event.KeyInput.PressedDown;
            }
            }
            break;
            case EET_MOUSE_INPUT_EVENT:
            {
            switch(event.MouseInput.Event)
            {
            case EMIE_LMOUSE_PRESSED_DOWN:
            MouseKeys[0] = true;
            break;
            case EMIE_RMOUSE_PRESSED_DOWN:
            PrevCursor = CursorPos;
            MouseKeys[2] = true;
            break;
            case EMIE_MMOUSE_PRESSED_DOWN:
            MouseKeys[1] = true;
            break;
            case EMIE_LMOUSE_LEFT_UP:
            MouseKeys[0] = false;
            break;
            case EMIE_RMOUSE_LEFT_UP:
            MouseKeys[2] = false;
            break;
            case EMIE_MMOUSE_LEFT_UP:
            MouseKeys[1] = false;
            break;
            case EMIE_MOUSE_MOVED:
            MousePos.X() = (f32)event.MouseInput.X;
            MousePos.Y() = (f32)event.MouseInput.Y;
            break;
            case EMIE_MOUSE_WHEEL:
            case EMIE_COUNT:
            {
            }
            };
            }
            break;
            default:
            break;
            };

            // update position
            Ogre::Vector3 ogreCameraPos = m_selectedCamera->getPosition();
            Vector3F cameraPos(&ogreCameraPos[0]);

            CursorPos.set(MousePos.X(),MousePos.Y());

            Vector3F cameraTarget = cameraPos + Vector3F::UNIT_Z;

            // Update rotation
            Ogre::Vector3 ogreTargetDir = m_selectedCamera->getDerivedDirection();
            Vector3F targetDir(&ogreTargetDir[0]);
            //targetDir = m_targetDirection;
            targetDir.Z() = -targetDir.Z();
            Vector3F relativeRotation = targetDir.getHorizontalAngle();

            if(isMouseKeyDown(2))
            {
            static f32 ratio = 10.01;
            Vector2F relativeMouse = (PrevCursor - CursorPos) * ratio;
            if(relativeMouse.squaredLength() > 0.0f)
            {
            relativeRotation.X() -= relativeMouse.Y() * RotateSpeed;
            relativeRotation.Y() -= relativeMouse.X() * RotateSpeed;

            PrevCursor = CursorPos;

            targetDir.set(0,0,1);

            Matrix4F mat;
            mat.setRotationDegrees(Vector3F(relativeRotation.X(), relativeRotation.Y(), 0));
            mat.transformVect(targetDir);
            targetDir.normalise();

            eventHandled = true;

            m_targetDirection = targetDir;

            targetDir.Z() = -targetDir.Z();
            m_selectedCamera->setDirection(Ogre::Vector3(targetDir));
            }
            }
            else
            {
            PrevCursor = CursorPos;
            }*/

            return false;
        }

        void FpsCameraController::setPosition( const Vector3F &position )
        {
            m_position = position;
        }

        Vector3F FpsCameraController::getPosition() const
        {
            return m_position;
        }

        void FpsCameraController::setTargetPosition( const Vector3F &position )
        {
        }

        Vector3F FpsCameraController::getTargetPosition() const
        {
            m_targetPosition = m_position + m_targetDirection;
            return m_targetPosition;
        }

        void FpsCameraController::setOrientation( const QuaternionF &orientation )
        {
            m_orientation = orientation;
        }

        QuaternionF FpsCameraController::getOrientation() const
        {
            /*Ogre::Quaternion ogreOrientation = m_selectedCamera->getOrientation();
            Ogre::Matrix3 ogreMatrix;
            ogreOrientation.ToRotationMatrix(ogreMatrix);

            /*f32 xAngle;
            f32 yAngle;
            f32 zAngle;
            ogreMatrix.ToEulerAnglesXYZ(xAngle, yAngle, zAngle);
            m_orientation = Quaternion(&ogreOrientation[0]);*/
            return m_orientation;
        }

        Vector3F FpsCameraController::getDirection() const
        {
            // Ogre::Vector3 dir = m_selectedCamera->getRealDirection();
            // return Vector3F(&dir[0]);

            return Vector3F::zero();
        }

        void FpsCameraController::setPropertyValue( const String &name, const String &value )
        {
            if( name == ( "MoveSpeed" ) )
            {
                m_translationSpeed = StringUtil::parseFloat( value );
            }
            else if( name == ( "FarDistance" ) )
            {
                if( m_selectedCamera )
                {
                    f32 farDistance = StringUtil::parseFloat( value );
                    m_selectedCamera->setFarClipDistance( farDistance );
                }
            }
        }

        void FpsCameraController::setProperties( const Properties &propertyGroup )
        {
            Vector3F position;

            const Array<Property> &properties = propertyGroup.getPropertiesAsArray();

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

        void FpsCameraController::getProperties( Properties &propertyGroup ) const
        {
            propertyGroup = m_propertyGroup;
            propertyGroup.setProperty( "Position", StringUtil::toString( m_position ) );
        }

        bool FpsCameraController::isMouseKeyDown( s32 key )
        {
            return MouseKeys[key];
        }

        void FpsCameraController::allKeysUp()
        {
            for( u32 i = 0; i < 6; ++i )
                CursorKeys[i] = false;
        }

        Ray3F FpsCameraController::getCameraToViewportRay( const Vector2F &screenPosition ) const
        {
            Ray3F ray;

            /*if(m_selectedCamera)
            {
            Ogre::Viewport* viewport = m_selectedCamera->getViewport();
            f32 width = viewport->getActualWidth();
            f32 height = viewport->getActualHeight();

            Vector2F normalisedScreenCoords;
            normalisedScreenCoords.X() = screenPosition.X() / width;
            normalisedScreenCoords.Y() = screenPosition.Y() / height;

            Ogre::Ray ogreRay = m_selectedCamera->getCameraToViewportRay(normalisedScreenCoords.X(),
            normalisedScreenCoords.Y());

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

        bool FpsCameraController::isInFrustum( const AABB3F &box ) const
        {
            return true;
        }

        void FpsCameraController::addCamera( SmartPtr<render::ICamera> camera )
        {
            m_cameras.push_back( camera );
        }

        bool FpsCameraController::removeCamera( SmartPtr<render::ICamera> camera )
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

        FpsCameraController::SCamKeyMap::SCamKeyMap( s32 a, s32 k, s32 k1 ) :
            action( a ),
            keycode( k ),
            keycode1( k1 )
        {
        }

        FpsCameraController::SCamKeyMap::SCamKeyMap()
        {
        }
    }  // namespace scene
}  // end namespace fb
