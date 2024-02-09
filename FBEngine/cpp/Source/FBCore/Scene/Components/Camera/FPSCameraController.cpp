#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/FPSCameraController.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Input/IInputEvent.h>
#include <FBCore/Interface/Input/IKeyboardState.h>
#include <FBCore/Interface/Input/IMouseState.h>
#include <FBCore/Interface/System/ISelectionManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Scene/Components/Camera.h>

namespace fb::scene
{

    FB_CLASS_REGISTER_DERIVED( fb, FpsCameraController, CameraController );

    FpsCameraController::FpsCameraController() :
        m_selectedCamera( nullptr ),
        MoveSpeed( 1.0f ),
        m_rotationSpeed( 10.01f )
    {
        MouseKeys.resize( 3 );
        CursorKeys.resize( 6 );
        m_translationSpeed = 100.0f;

        for( auto &&MouseKey : MouseKeys )
        {
            MouseKey = false;
        }

        // create default key map
        KeyMap.emplace_back( 0, static_cast<u32>( KeyCodes::KEY_UP ),
                             static_cast<u32>( KeyCodes::KEY_KEY_W ) );
        KeyMap.emplace_back( 1, static_cast<u32>( KeyCodes::KEY_DOWN ),
                             static_cast<u32>( KeyCodes::KEY_KEY_S ) );
        KeyMap.emplace_back( 2, static_cast<u32>( KeyCodes::KEY_LEFT ),
                             static_cast<u32>( KeyCodes::KEY_KEY_A ) );
        KeyMap.emplace_back( 3, static_cast<u32>( KeyCodes::KEY_RIGHT ),
                             static_cast<u32>( KeyCodes::KEY_KEY_D ) );

        allKeysUp();

        m_targetDirection = Vector3<real_Num>::UNIT_Z;
    }

    FpsCameraController::~FpsCameraController() = default;

    void FpsCameraController::update()
    {
        /*if(!m_selectedCamera)
        return;

        if(IApplicationManager::instance()->getCameraManager()->getCurrentCamera() !=
        SmartPtr<scene::CameraController>(this)) return;

        Vector3<real_Num> position = m_position;

        if (CursorKeys[0])
        position += m_targetDirection * dt * MoveSpeed * m_translationSpeed;

        if (CursorKeys[1])
        position -= m_targetDirection * dt * MoveSpeed * m_translationSpeed;

        // strafing
        Ogre::Vector3 ogreCamUp = m_selectedCamera->getDerivedUp();
        Vector3<real_Num> upVector(&ogreCamUp[0]);
        upVector.Z() = -upVector.Z();

        Vector3<real_Num> strafevect = m_targetDirection.crossProduct(upVector);
        strafevect.normalise();
        if (CursorKeys[2])
        position += strafevect * dt * MoveSpeed * m_translationSpeed;

        if (CursorKeys[3])
        position -= strafevect * dt * MoveSpeed * m_translationSpeed;

        m_position = position;

        Vector3<real_Num> ogreCameraPosition = m_position;
        ogreCameraPosition.Z() = -ogreCameraPosition.Z();
        m_selectedCamera->setPosition(Ogre::Vector3(ogreCameraPosition));	*/
    }

    auto FpsCameraController::handleInputEvent( const SmartPtr<IInputEvent> &event ) -> bool
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
        Vector3<real_Num> cameraPos(&ogreCameraPos[0]);

        CursorPos.set(MousePos.X(),MousePos.Y());

        Vector3<real_Num> cameraTarget = cameraPos + Vector3<real_Num>::UNIT_Z;

        // Update rotation
        Ogre::Vector3 ogreTargetDir = m_selectedCamera->getDerivedDirection();
        Vector3<real_Num> targetDir(&ogreTargetDir[0]);
        //targetDir = m_targetDirection;
        targetDir.Z() = -targetDir.Z();
        Vector3<real_Num> relativeRotation = targetDir.getHorizontalAngle();

        if(isMouseKeyDown(2))
        {
        static f32 ratio = 10.01;
        Vector2<real_Num> relativeMouse = (PrevCursor - CursorPos) * ratio;
        if(relativeMouse.squaredLength() > 0.0f)
        {
        relativeRotation.X() -= relativeMouse.Y() * RotateSpeed;
        relativeRotation.Y() -= relativeMouse.X() * RotateSpeed;

        PrevCursor = CursorPos;

        targetDir.set(0,0,1);

        Matrix4F mat;
        mat.setRotationDegrees(Vector3<real_Num>(relativeRotation.X(), relativeRotation.Y(), 0));
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

    void FpsCameraController::setPosition( const Vector3<real_Num> &position )
    {
        m_position = position;
    }

    auto FpsCameraController::getPosition() const -> Vector3<real_Num>
    {
        return m_position;
    }

    void FpsCameraController::setTargetPosition( const Vector3<real_Num> &position )
    {
    }

    auto FpsCameraController::getTargetPosition() const -> Vector3<real_Num>
    {
        m_targetPosition = m_position + m_targetDirection;
        return m_targetPosition;
    }

    void FpsCameraController::setOrientation( const Quaternion<real_Num> &orientation )
    {
        m_orientation = orientation;
    }

    auto FpsCameraController::getOrientation() const -> Quaternion<real_Num>
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

    auto FpsCameraController::getDirection() const -> Vector3<real_Num>
    {
        // Ogre::Vector3 dir = m_selectedCamera->getRealDirection();
        // return Vector3<real_Num>(&dir[0]);

        return Vector3<real_Num>::zero();
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
        Vector3<real_Num> position;

        const Array<Property> &properties = propertyGroup.getPropertiesAsArray();

        // loop through the keys and set the variables
        for( const auto &key : properties )
        {
            if( key.getName() == ( "Position" ) )
            {
                position = StringUtil::parseVector3<real_Num>( key.getValue() );
            }
        }

        setPosition( position );
        setTargetPosition( position + Vector3<real_Num>( 0, 0, 1 ) );

        m_propertyGroup = propertyGroup;
    }

    void FpsCameraController::getProperties( Properties &propertyGroup ) const
    {
        propertyGroup = m_propertyGroup;
        propertyGroup.setProperty( "Position", StringUtil::toString( m_position ) );
    }

    auto FpsCameraController::isMouseKeyDown( s32 key ) -> bool
    {
        return MouseKeys[key];
    }

    void FpsCameraController::allKeysUp()
    {
        for( u32 i = 0; i < 6; ++i )
        {
            CursorKeys[i] = false;
        }
    }

    auto FpsCameraController::getCameraToViewportRay( const Vector2<real_Num> &screenPosition ) const
        -> Ray3F
    {
        Ray3F ray;

        /*if(m_selectedCamera)
        {
        Ogre::Viewport* viewport = m_selectedCamera->getViewport();
        f32 width = viewport->getActualWidth();
        f32 height = viewport->getActualHeight();

        Vector2<real_Num> normalisedScreenCoords;
        normalisedScreenCoords.X() = screenPosition.X() / width;
        normalisedScreenCoords.Y() = screenPosition.Y() / height;

        Ogre::Ray ogreRay = m_selectedCamera->getCameraToViewportRay(normalisedScreenCoords.X(),
        normalisedScreenCoords.Y());

        Ogre::Vector3 ogreCameraPosition = m_selectedCamera->getDerivedPosition();
        Ogre::Vector3 rayOrigin = ogreRay.getOrigin();
        Ogre::Vector3 rayDirection = ogreRay.getDirection();
        ray.Start = Vector3<real_Num>(&rayOrigin[0]);
        ray.Direction = Vector3<real_Num>(&rayDirection[0]);

        ray.Start.Z() = -ray.Start.Z();
        ray.Direction.Z() = -ray.Direction.Z();
        }*/

        return ray;
    }

    auto FpsCameraController::isInFrustum( const AABB3F &box ) const -> bool
    {
        return true;
    }

    void FpsCameraController::addCamera( SmartPtr<render::ICamera> camera )
    {
        m_cameras.push_back( camera );
    }

    auto FpsCameraController::removeCamera( SmartPtr<render::ICamera> camera ) -> bool
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

    FpsCameraController::SCamKeyMap::SCamKeyMap( s32 a, s32 k, s32 k1 ) :
        action( a ),
        keycode( k ),
        keycode1( k1 )
    {
    }

    FpsCameraController::SCamKeyMap::SCamKeyMap() = default;
}  // namespace fb::scene
