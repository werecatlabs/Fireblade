#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/SphericalCameraController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, SphericalCameraController, CameraController );

        const f32 SphericalCameraController::DEFAULT_FAR_DISTANCE = 2500.0f;
        const f32 SphericalCameraController::DEFAULT_NEAR_DISTANCE = 0.01f;
        const f32 SphericalCameraController::DEFAULT_ZOOM_SPEED = 1.0f;
        const f32 SphericalCameraController::DEFAULT_MOVE_SPEED = 1.0f;

        const Vector3F SphericalCameraController::DEFAULT_SPHERICAL_COORDS =
            Vector3F( 1.0f, 0.0f, 0.5f );

        SphericalCameraController::SphericalCameraController() :
            m_rotationSpeed( 1.0f ),

            m_maxDistance( DEFAULT_FAR_DISTANCE ),
            m_nearDistance( DEFAULT_NEAR_DISTANCE ),
            m_zoomSpeed( DEFAULT_ZOOM_SPEED ),
            m_moveSpeed( DEFAULT_MOVE_SPEED )
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

            // auto applicationManager = core::IApplicationManager::instance();
            // auto inputManager = applicationManager->getInputManager();
            // inputManager->addListener(SmartPtr<IEventListener>(new FPSCameraInputListener(this)));
        }

        SphericalCameraController::~SphericalCameraController()
        {
            unload( nullptr );
        }

        void SphericalCameraController::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                CameraController::load( data );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SphericalCameraController::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    CameraController::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void SphericalCameraController::update()
        {
            switch( auto task = Thread::getCurrentTask() )
            {
            case Thread::Task::Application:
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto sceneManager = applicationManager->getSceneManager();

                auto cameraManager = applicationManager->getCameraManager();
                FB_ASSERT( cameraManager );

                auto timer = applicationManager->getTimer();
                FB_ASSERT( timer );

                auto t = timer->getTime();
                auto dt = timer->getDeltaTime();

                // if (cameraManager->getCurrentCamera() != SmartPtr<scene::CameraController>(this))
                //{
                //	return;
                // }

                if( isMouseBtnDown( 0 ) )
                {
                    m_spherical.Y() += m_sphericalForce.Y() * -m_moveSpeed * static_cast<f32>( dt );
                    m_spherical.Z() += m_sphericalForce.Z() * -m_moveSpeed * static_cast<f32>( dt );
                    FB_ASSERT( m_spherical.isValid() );
                }
                else if( isMouseBtnDown( 2 ) )
                {
                    if( auto actor = getActor() )
                    {
                        auto transform = actor->getTransform();
                        auto orientation = transform->getOrientation();
                        auto translateX = orientation * Vector3<real_Num>::right() *
                                          m_sphericalForce.Y() * m_moveSpeed * static_cast<f32>( dt ) *
                                          30.0f;
                        auto translateY = orientation * Vector3<real_Num>::up() * m_sphericalForce.Z() *
                                          -m_moveSpeed * static_cast<f32>( dt ) * 30.0f;

                        m_target += translateX + translateY;
                        FB_ASSERT( m_target.isValid() );
                    }
                }

                m_spherical.X() += m_sphericalForce.X() * -m_zoomSpeed * static_cast<f32>( dt );

                if( m_spherical.X() < m_nearDistance )
                {
                    m_spherical.X() = m_nearDistance;
                }
                else if( m_spherical.X() > m_maxDistance )
                {
                    m_spherical.X() = m_maxDistance;
                }

                m_sphericalForce *= static_cast<f32>( dt );
                FB_ASSERT( m_sphericalForce.isValid() );

                wrapSphericalCoords( m_spherical );
                m_position = m_target + getCartCoords( m_spherical );
                FB_ASSERT( m_position.isValid() );

                auto yawAxis = Vector3<real_Num>::unitY();
                if( m_spherical.Z() >= 0.0f )
                {
                    if( m_spherical.Z() <= Math<real_Num>::pi() )
                    {
                        yawAxis = Vector3<real_Num>::UNIT_Y;
                    }
                    else
                    {
                        yawAxis = -Vector3<real_Num>::UNIT_Y;
                    }
                }
                else
                {
                    if( m_spherical.Z() >= -MathF::pi() )
                    {
                        yawAxis = -Vector3<real_Num>::UNIT_Y;
                    }
                    else
                    {
                        yawAxis = Vector3<real_Num>::UNIT_Y;
                    }
                }

                auto direction = ( m_target - m_position ).normaliseCopy();
                FB_ASSERT( direction.isValid() );

                auto actorOrientation = MathUtil<real_Num>::getOrientationFromDirection(
                    direction, -Vector3<real_Num>::unitZ(), true, yawAxis );
                FB_ASSERT( actorOrientation.isValid() );

                if( auto actor = getActor() )
                {
                    actor->setPosition( m_position );
                    actor->setOrientation( actorOrientation );
                    actor->updateTransform();
                }
            }
            break;
            default:
            {
            }
            }
        }

        bool SphericalCameraController::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            bool eventHandled = false;

            switch( auto inputEventType = event->getEventType() )
            {
            case IInputEvent::EventType::Key:
            {
                auto keyboardState = event->getKeyboardState();
                auto keycode = keyboardState->getKeyCode();

                for( auto keyMap : m_keyMap )
                {
                    if( keyMap.keycode == keycode || keyMap.keycode1 == keycode )
                    {
                        m_cursorKeys[keyMap.action] = keyboardState->isPressedDown();
                    }
                }
            }
            break;
            case IInputEvent::EventType::Mouse:
            {
                if( auto mouseState = event->getMouseState() )
                {
                    auto relativePosition = mouseState->getRelativePosition();

                    if( auto uiWindow = getUiWindow() )
                    {
                        if( auto mainWindow = applicationManager->getWindow() )
                        {
                            auto mainWindowSize = mainWindow->getSize();
                            auto mainWindowSizeF =
                                Vector2F( (f32)mainWindowSize.x, (f32)mainWindowSize.y );

                            auto pos = uiWindow->getPosition() / mainWindowSizeF;
                            auto size = uiWindow->getSize() / mainWindowSizeF;

                            auto aabb = AABB2F( pos, size, true );
                            if( !aabb.isInside( relativePosition ) )
                            {
                                for( size_t i = 0; i < m_mouseKeys.size(); ++i )
                                {
                                    m_mouseKeys[i] = false;
                                }

                                m_sphericalForce = Vector3<real_Num>::zero();

                                return false;
                            }
                        }
                    }
                    else
                    {
                        auto actor = getActor();
                        auto cameraComponent = actor->getComponent<Camera>();
                        auto camera = cameraComponent->getCamera();

                        if( camera )
                        {
                            auto vp = camera->getViewport();
                            auto pos = vp->getPosition();
                            auto size = vp->getSize();
                            if( !( relativePosition > pos && relativePosition < ( pos + size ) ) )
                            {
                                return false;
                            }
                        }
                    }

                    auto mouseEventType = mouseState->getEventType();
                    switch( mouseEventType )
                    {
                    case IMouseState::Event::LeftPressed:
                    {
                        m_mouseKeys[0] = true;
                    }
                    break;
                    case IMouseState::Event::RightPressed:
                    {
                        m_mouseKeys[2] = true;
                    }
                    break;
                    case IMouseState::Event::MiddlePressed:
                    {
                        m_mouseKeys[1] = true;
                    }
                    break;
                    case IMouseState::Event::LeftReleased:
                    {
                        m_mouseKeys[0] = false;
                    }
                    break;
                    case IMouseState::Event::RightReleased:
                    {
                        m_mouseKeys[2] = false;
                    }
                    break;
                    case IMouseState::Event::MiddleReleased:
                    {
                        m_mouseKeys[1] = false;
                    }
                    break;
                    case IMouseState::Event::Moved:
                    {
                        // Update rotation
                        auto sphericalForce = mouseState->getRelativeMove();
                        m_sphericalForce.Y() += sphericalForce.X();
                        m_sphericalForce.Z() += sphericalForce.Y();

                        if( mouseState->getWheelPosition() != 0.0f )
                        {
                            m_sphericalForce.X() += mouseState->getWheelPosition();
                            eventHandled = true;
                        }

                        eventHandled = true;
                    }
                    break;
                    case IMouseState::Event::Wheel:
                    {
                    }
                    break;
                    case IMouseState::Event::Count:
                    {
                    }
                    }
                }
            }
            break;
            default:
            {
            }
            break;
            }

            return eventHandled;
        }

        void SphericalCameraController::setPosition( const Vector3<real_Num> &position )
        {
            m_position = position;
        }

        Vector3<real_Num> SphericalCameraController::getPosition() const
        {
            return m_position;
        }

        void SphericalCameraController::setTargetPosition( const Vector3<real_Num> &position )
        {
            m_target = position;
        }

        Vector3<real_Num> SphericalCameraController::getTargetPosition() const
        {
            return m_target;
        }

        void SphericalCameraController::setOrientation( const QuaternionF &orientation )
        {
        }

        QuaternionF SphericalCameraController::getOrientation() const
        {
            QuaternionF yaw;
            yaw.fromAngleAxis( MathF::DegToRad( m_rotation.Y() ), Vector3F::UNIT_Y );

            QuaternionF pitch;
            pitch.fromAngleAxis( MathF::DegToRad( m_rotation.X() ), Vector3F::UNIT_X );

            // the orientation
            return yaw * pitch;
        }

        void SphericalCameraController::setDirection( const Vector3F &direction )
        {
        }

        Vector3F SphericalCameraController::getDirection() const
        {
            return getOrientation() * -Vector3F::UNIT_Z;
        }

        void SphericalCameraController::setPropertyValue( const String &name, const String &value )
        {
            if( name == ( "MoveSpeed" ) )
            {
                m_translationSpeed = StringUtil::parseFloat( value );
            }
            else if( name == ( "FarDistance" ) )
            {
            }
        }

        void SphericalCameraController::setProperties( const Properties &propertyGroup )
        {
            // Vector3<real_Num> position;

            // Array<Property> properties = propertyGroup.getPropertiesAsArray();

            ////loop through the keys and set the variables
            // for (u32 i = 0; i < properties.size(); ++i)
            //{
            //	const Property& key = properties[i];
            //	if (key.getName() == ("Position"))
            //	{
            //		position = StringUtil::parseVector3<real_Num>(key.getValue());
            //	}
            // }

            // setPosition(position);
            // setTargetPosition(position + Vector3<real_Num>(0, 0, 1));

            // m_propertyGroup = propertyGroup;
        }

        void SphericalCameraController::getProperties( Properties &propertyGroup ) const
        {
            // propertyGroup = m_propertyGroup;
            // p/ropertyGroup.setProperty("Position", StringUtil::toString(m_position));
        }

        bool SphericalCameraController::isMouseBtnDown( s32 key )
        {
            return m_mouseKeys[key];
        }

        void SphericalCameraController::allKeysUp()
        {
            for( u32 i = 0; i < 6; ++i )
                m_cursorKeys[i] = false;
        }

        Ray3F SphericalCameraController::getCameraToViewportRay( const Vector2F &screenPosition ) const
        {
            // SmartPtr<render::ICamera> selectedCamera;

            // if (!m_cameras.empty())
            //{
            //	selectedCamera = m_cameras[0];
            // }

            // if (selectedCamera)
            //{
            //	auto viewport = selectedCamera->getViewport();
            //	auto width = viewport->getActualWidth();
            //	auto height = viewport->getActualHeight();

            //	Vector2F normalizedScreenCoords;
            //	normalizedScreenCoords.X() = screenPosition.X() / width;
            //	normalizedScreenCoords.Y() = screenPosition.Y() / height;

            //	return selectedCamera->getRay(normalizedScreenCoords.X(), normalizedScreenCoords.Y());
            //}

            return Ray3F();
        }

        bool SphericalCameraController::isInFrustum( const AABB3F &box ) const
        {
            return true;
        }

        Vector3<real_Num> SphericalCameraController::getSphericalCoords() const
        {
            return m_spherical;
        }

        void SphericalCameraController::setSphericalCoords( const Vector3<real_Num> &val )
        {
            m_spherical = val;
        }

        bool SphericalCameraController::InputListener::inputEvent( SmartPtr<IInputEvent> event )
        {
            return m_owner->handleEvent( event );
        }

        bool SphericalCameraController::InputListener::updateEvent( const SmartPtr<IInputEvent> &event )
        {
            return false;
        }

        s32 SphericalCameraController::InputListener::getPriority() const
        {
            return 0;
        }

        void SphericalCameraController::InputListener::setPriority( s32 priority )
        {
        }

        SphericalCameraController::InputListener::InputListener( SphericalCameraController *owner ) :
            m_owner( owner )
        {
        }

        Vector3<real_Num> SphericalCameraController::getCartCoords( Vector3<real_Num> &spherical )
        {
            Vector3<real_Num> cart;
            cart.Z() = spherical.X() * sin( spherical.Z() ) * cos( spherical.Y() );
            cart.X() = spherical.X() * sin( spherical.Z() ) * sin( spherical.Y() );
            cart.Y() = spherical.X() * -cos( spherical.Z() );
            return cart;
        }

        Vector3F SphericalCameraController::getSphericalCoords( Vector3F &vec )
        {
            auto coords = Vector3<real_Num>::zero();
            coords.X() = Math<real_Num>::Abs( vec.length() );
            coords.Y() = Math<real_Num>::ATan2( vec.X(), vec.Z() );
            coords.Z() = Math<real_Num>::ACos( vec.Y() / coords.X() );
            return coords;
        }

        void SphericalCameraController::wrapSphericalCoords( Vector3<real_Num> &spherical )
        {
            spherical.Z() = Math<real_Num>::Mod( spherical.Z(),
                                                 Math<real_Num>::pi() * static_cast<real_Num>( 2.0 ) );
        }

        f32 SphericalCameraController::getMaxDistance() const
        {
            return m_maxDistance;
        }

        void SphericalCameraController::setMaxDistance( f32 val )
        {
            m_maxDistance = val;
        }

        void SphericalCameraController::setRotationSpeed( f32 val )
        {
            m_rotationSpeed = val;
        }

        f32 SphericalCameraController::getRotationSpeed() const
        {
            return m_rotationSpeed;
        }

        Vector3F SphericalCameraController::getRight() const
        {
            return Vector3F::zero();
        }

        Vector3F SphericalCameraController::getUp() const
        {
            return Vector3F::zero();
        }

        void SphericalCameraController::focusSelection()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto position = Vector3<real_Num>::zero();
            auto rotation = Vector3<real_Num>::zero();

            auto count = 0;
            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<IActor>() )
                {
                    auto actor = fb::static_pointer_cast<IActor>( selected );
                    if( actor )
                    {
                        if( auto t = actor->getTransform() )
                        {
                            position += t->getPosition();
                            ++count;
                        }
                    }
                }
            }

            FB_ASSERT( position.isValid() );

            if( count > 0 )
            {
                m_target = position / (real_Num)count;
            }

            FB_ASSERT( m_target.isValid() );
        }

        SmartPtr<ui::IUIWindow> SphericalCameraController::getUiWindow() const
        {
            return m_uiWindow;
        }

        void SphericalCameraController::setUiWindow( SmartPtr<ui::IUIWindow> uiWindow )
        {
            m_uiWindow = uiWindow;
        }

        SphericalCameraController::SCamKeyMap::SCamKeyMap( s32 a, s32 k, s32 k1 ) :
            action( a ),
            keycode( k ),
            keycode1( k1 )
        {
        }

        SphericalCameraController::SCamKeyMap::SCamKeyMap()
        {
        }
    }  // namespace scene
}  // end namespace fb