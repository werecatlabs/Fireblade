#include <FBCore/FBCorePCH.h>
#include <FBCore/System/RotateManipulator.h>
#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Interface/Graphics/IDebugLine.h>
#include <FBCore/Interface/Graphics/IDebugCircle.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>
#include <FBCore/Interface/Input/IInputEvent.h>
#include <FBCore/Interface/Input/IMouseState.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/System/ISelectionManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>
#include <FBCore/ApplicationUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, RotateManipulator, Manipulator );

    RotateManipulator::RotateManipulator() = default;

    RotateManipulator::~RotateManipulator()
    {
        unload( nullptr );
    }

    void RotateManipulator::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Loading && loadingState != LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Loading );

                m_circles.resize( 3 );
                m_orientations.resize( 3 );
                m_selected.resize( 3 );

                m_orientations[0] = Quaternion<real_Num>::eulerDegrees( 90.0, 0.0, 0.0 );
                m_orientations[1] = Quaternion<real_Num>::eulerDegrees( 0.0, 90.0, 0.0 );
                m_orientations[2] = Quaternion<real_Num>::eulerDegrees( 0.0, 0.0, 90.0 );

                m_selected[0] = false;
                m_selected[1] = false;
                m_selected[2] = false;

                Manipulator::load( data );
                setLoadingState( LoadingState::Loaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void RotateManipulator::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            // if (m_msgDispatchListner)
            //{
            //	SmartPtr<ITaskManager>& taskManager = FBSystem::getSingletonPtr()->getTaskManager();
            //	bool result =
            // FBSystem::getSingletonPtr()->getTaskManager()->removeTaskListener(m_msgDispatchListner);
            //	_FB_DEBUG_BREAK_IF(!result);

            //	FB_SAFE_REMOVE(m_msgDispatchListner);
            //}

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void RotateManipulator::update()
    {
        if( !isEnabled() )
        {
            for( auto circle : m_circles )
            {
                if( circle )
                {
                    circle->setVisible( false );
                }
            }
            return;
        }

        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto debug = graphicsSystem->getDebug();
        auto inputManager = applicationManager->getInputDeviceManager();
        auto selectionManager = applicationManager->getSelectionManager();

        auto cameraManager = applicationManager->getCameraManager();
        auto scale = Vector3F( 1.f, 1.f, 1.f );

        auto cameraActor = cameraManager->getEditorCamera();
        if( cameraActor )
        {
            auto camera = cameraActor->getComponent<scene::CameraController>();
            auto cameraWorldTransform = cameraActor->getTransform();

            auto camPos = cameraWorldTransform->getPosition();
            auto camDist = ( m_vPosition - camPos ).length();
            if( camDist < 1.0f )
            {
                camDist = 1.0f;
            }

            scale *= camDist * m_cameraDistanceScale;
            m_fScale = scale.length();
        }

        Array<u32> colours;
        colours.resize( 3 );

        colours[0] = m_selected[0] ? 0xFFFF00 : 0xFF0000FF;
        colours[1] = m_selected[1] ? 0xFFFF00 : 0x00FF00FF;
        colours[2] = m_selected[2] ? 0xFFFF00 : 0x0000FFFF;

        auto pos = getPosition();
        static const auto hash = StringUtil::getHash( "RotateManipulator" );

        auto orientation = Quaternion<real_Num>::identity();

        for( size_t i = 0; i < 3; ++i )
        {
            auto circle = debug->drawCircle( hash + static_cast<hash_type>( i ), pos,
                                             orientation * m_orientations[i], m_fScale, colours[i] );
            circle->setMaxLifeTime( 1e10 );
            m_circles[i] = circle;
        }

        for( auto circle : m_circles )
        {
            if( circle )
            {
                circle->setVisible( true );
                circle->setMaxLifeTime( 1e10 );
            }
        }

        if( m_selected[0] || m_selected[1] || m_selected[2] )
        {
            if( inputManager->isMouseButtonDown( 0 ) )
            {
                auto mouse = m_prevCursor - m_cursorPos;
                auto rotationSpeed = 1.0f;

                // Rotate the object based on user input
                auto rotation = Vector3F( -mouse.y, mouse.x, 0 ) * rotationSpeed;
                auto selection = selectionManager->getSelection();

                for( auto selected : selection )
                {
                    auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                    if( actor )
                    {
                        auto transform = actor->getTransform();
                        auto orientation = transform->getOrientation();
                        transform->setLocalOrientation(
                            orientation *
                            Quaternion<real_Num>::eulerDegrees( rotation.x, rotation.y, rotation.z ) );

                        transform->setDirty( true );
                        actor->updateTransform();
                    }
                }
            }
        }
    }

    auto RotateManipulator::handleEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        if( !isEnabled() )
        {
            return false;
        }

        auto inputResult = false;

        auto applicationManager = core::ApplicationManager::instance();
        auto timer = applicationManager->getTimer();
        auto cameraManager = applicationManager->getCameraManager();
        auto selectionManager = applicationManager->getSelectionManager();

        auto position = getPosition();

        auto curControlWidth = m_controlWidth * m_fScale;

        auto cameraActor = cameraManager->getEditorCamera();
        auto camera = cameraActor->getComponent<scene::Camera>();
        if( camera )
        {
            auto mouseState = event->getMouseState();
            if( mouseState )
            {
                auto mousePosition = mouseState->getAbsolutePosition();
                auto relativeMousePosition = mouseState->getRelativePosition();

                auto mouseStateRelative = ApplicationUtil::getRelativeMousePos( relativeMousePosition );

                auto mouseStateEvent = mouseState->getEventType();

                if( ( mouseStateEvent == IMouseState::Event::Moved ) )
                {
                    m_prevCursor = m_cursorPos;
                    m_cursorPos = mousePosition;

                    for( size_t i = 0; i < 3; ++i )
                    {
                        m_selected[i] = false;
                    }

                    auto orientation = Quaternion<real_Num>::identity();

                    auto ray = camera->getCameraToViewportRay( mouseStateRelative );

                    for( size_t i = 0; i < 3; ++i )
                    {
                        auto line = Line3F( position - ( orientation * m_orientations[i] *
                                                         Vector3F::unitY() * m_controlWidth ),
                                            position + ( orientation * m_orientations[i] *
                                                         Vector3F::unitY() * m_controlWidth ) );
                        auto cylinder = Cylinder3F( line, m_fScale, curControlWidth );
                        auto cylinderResultX = MathUtilF::intersects( ray, cylinder );
                        if( cylinderResultX.first )
                        {
                            auto intersectionPoint =
                                ray.getOrigin() + ( ray.getDirection() * cylinderResultX.second );
                            auto intersectionDistance = ( intersectionPoint - position ).length();
                            if( intersectionDistance > m_fScale * ( 1.0f - m_tollerance ) &&
                                intersectionDistance < m_fScale * ( 1.0f + m_tollerance ) )
                            {
                                m_selected[i] = true;

                                if( mouseStateEvent == IMouseState::Event::LeftPressed )
                                {
                                    inputResult = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return inputResult;
    }

}  // end namespace fb
