#include <FBCore/FBCorePCH.h>
#include <FBCore/System/TranslateManipulator.h>
#include <FBCore/Scene/Components/Camera/CameraController.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/Interface/Graphics/IDebug.h>
#include <FBCore/Interface/Graphics/IDebugLine.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/IWindow.h>
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
    FB_CLASS_REGISTER_DERIVED( fb, TranslateManipulator, Manipulator );

    TranslateManipulator::TranslateManipulator() = default;

    TranslateManipulator::~TranslateManipulator()
    {
        unload( nullptr );
    }

    void TranslateManipulator::load( SmartPtr<ISharedObject> data )
    {
        m_lines.resize( 3 );
        m_selected.resize( 3 );

        Manipulator::load( data );
    }

    void TranslateManipulator::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                auto selectionManager = applicationManager->getSelectionManager();

                if( selectionManager )
                {
                    if( m_selectionManagerListener )
                    {
                        selectionManager->removeObjectListener( m_selectionManagerListener );
                        m_selectionManagerListener = nullptr;
                    }
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TranslateManipulator::preUpdate()
    {
        auto task = Thread::getCurrentTask();

        switch( task )
        {
        case Thread::Task::Application:
        {
            if( !m_enabled )
            {
                for( auto line : m_lines )
                {
                    if( line )
                    {
                        line->setVisible( false );
                    }
                }

                return;
            }

            auto &applicationManager = core::ApplicationManager::instance();
            auto &graphicsSystem = applicationManager->getGraphicsSystem();
            auto &debug = graphicsSystem->getDebug();

            auto cameraManager = applicationManager->getCameraManager();

            auto scale = Vector3F::unit();

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
            static const auto hash = StringUtil::getHash( "TranslateManipulator" );

            auto orientation = Quaternion<real_Num>::identity();

            m_lines[0] =
                debug->drawLine( hash + static_cast<hash_type>( AT_X ), pos,
                                 pos + ( orientation * Vector3F::unitX() * m_fScale ), colours[0] );
            m_lines[1] =
                debug->drawLine( hash + static_cast<hash_type>( AT_Y ), pos,
                                 pos + ( orientation * Vector3F::unitY() * m_fScale ), colours[1] );
            m_lines[2] =
                debug->drawLine( hash + static_cast<hash_type>( AT_Z ), pos,
                                 pos + ( orientation * Vector3F::unitZ() * m_fScale ), colours[2] );

            for( auto line : m_lines )
            {
                if( line )
                {
                    line->setVisible( true );
                    line->setMaxLifeTime( 1e10 );
                }
            }
        }
        break;
        default:
        {
        }
        }
    }

    void TranslateManipulator::update()
    {
    }

    void TranslateManipulator::postUpdate()
    {
    }

    auto TranslateManipulator::handleEvent( const SmartPtr<IInputEvent> &event ) -> bool
    {
        if( !isLoaded() )
        {
            return false;
        }

        if( !m_enabled )
        {
            return false;
        }

        //if( !isVisible() )
        //{
        //    return false;
        //}

        //if( event->getEventType() != IInputEvent::EventType::Mouse )
        //{
        //    return false;
        //}

        auto applicationManager = core::ApplicationManager::instance();
        auto cameraManager = applicationManager->getCameraManager();
        auto selectionManager = applicationManager->getSelectionManager();

        auto bResult = false;

        m_previousCursorPosition = m_currentCursorPosition;

        auto mouseState = event->getMouseState();
        if( mouseState )
        {
            auto mousePosition = mouseState->getAbsolutePosition();
            auto relativeMousePosition = mouseState->getRelativePosition();

            auto mouseStateRelative = ApplicationUtil::getRelativeMousePos( relativeMousePosition );

            m_currentCursorPosition.X() = mousePosition.X();
            m_currentCursorPosition.Y() = mousePosition.Y();

            auto cameraActor = cameraManager->getEditorCamera();
            if( !cameraActor )
            {
                return false;
            }

            auto camera = cameraActor->getComponent<scene::Camera>();
            auto cameraWorldTransform = cameraActor->getTransform();

            auto ray = camera->getCameraToViewportRay( mouseStateRelative );
            auto handles = getHandles();

            // Y Axis
            if( !m_bMouseButtonDown )
            {
                auto handle = handles[AT_Y];
                auto result = MathUtil<real_Num>::intersects( ray, handle );
                if( result.first )
                {
                    m_selected[1] = true;
                    m_vAxis = Vector3<real_Num>( 0.f, 1.f, 0.f );
                }
                else
                {
                    m_selected[1] = false;
                }
            }

            // x axis
            if( !m_bMouseButtonDown )
            {
                auto handle = handles[AT_X];
                auto result = MathUtil<real_Num>::intersects( ray, handle );
                if( result.first )
                {
                    m_selected[0] = true;
                    m_vAxis = Vector3<real_Num>( 1.f, 0.f, 0.f );
                }
                else
                {
                    m_selected[0] = false;
                }
            }

            // z axis
            if( !m_bMouseButtonDown )
            {
                auto handle = handles[AT_Z];
                auto result = MathUtil<real_Num>::intersects( ray, handle );
                if( result.first )
                {
                    m_selected[2] = true;
                    m_vAxis = Vector3<real_Num>( 0.f, 0.f, -1.f );
                }
                else
                {
                    m_selected[2] = false;
                }
            }

            // Mouse event
            auto mouseStateEvent = mouseState->getEventType();
            if( mouseStateEvent == IMouseState::Event::LeftReleased )
            {
                FB_LOG_MESSAGE( "TranslateManipulator", "TranslateManipulator: MouseButtonUp" );

                m_bMouseButtonDown = false;
                bResult = false;
            }
            else if( mouseStateEvent == IMouseState::Event::LeftPressed )
            {
                FB_LOG_MESSAGE( "TranslateManipulator", "CTranslateGizmo: MouseButtonDown" );

                m_bMouseButtonDown = true;
                if( m_selected[0] || m_selected[1] || m_selected[2] )
                {
                    bResult = true;
                }
            }
            else if( ( mouseStateEvent == IMouseState::Event::Moved ) && m_bMouseButtonDown )
            {
                FB_LOG_MESSAGE( "TranslateManipulator", "CTranslateGizmo: EMIE_MOUSE_MOVED" );

                if( m_selected[0] || m_selected[1] || m_selected[2] )
                {
                    auto mouseMove = m_previousCursorPosition - m_currentCursorPosition;

                    auto xAxis = Vector3<real_Num>::UNIT_X;
                    auto yAxis = Vector3<real_Num>::UNIT_Y;
                    auto zAxis = Vector3<real_Num>::UNIT_Z;

                    auto actor = camera->getActor();
                    auto worldTransform = actor->getTransform();
                    auto cameraOrientation = worldTransform->getOrientation();

                    Matrix3<real_Num> cameraRotation;

                    cameraOrientation.toRotationMatrix( cameraRotation );
                    xAxis = Vector3<real_Num>( cameraRotation.m_[0], cameraRotation.m_[1],
                                               cameraRotation.m_[2] );
                    yAxis = Vector3<real_Num>( cameraRotation.m_[4], cameraRotation.m_[5],
                                               cameraRotation.m_[6] );
                    zAxis = Vector3<real_Num>( cameraRotation.m_[8], cameraRotation.m_[9],
                                               cameraRotation.m_[10] );

                    Vector3<real_Num> translation;

                    if( m_selected[0] )
                    {
                        auto xyPlane =
                            ( zAxis.Y() < static_cast<real_Num>( 0.0 ) ? static_cast<real_Num>( -1.0 )
                                                                       : static_cast<real_Num>( 1.0 ) );
                        auto yAxis =
                            ( xAxis.Z() < static_cast<real_Num>( 0.0 ) ? static_cast<real_Num>( 1.0 )
                                                                       : static_cast<real_Num>( -1.0 ) );

                        translation.X() += mouseMove.X() * xAxis.X();
                        translation.X() += mouseMove.Y() * ( 1 - xAxis.X() ) * xyPlane * yAxis;
                    }
                    else if( m_selected[1] )
                    {
                        translation.Y() += -mouseMove.Y() * ( yAxis.Y() > static_cast<real_Num>( 0.0 )
                                                                  ? static_cast<real_Num>( 1.0 )
                                                                  : static_cast<real_Num>( -1.0 ) );
                    }
                    else if( m_selected[2] )
                    {
                        auto yAxis = zAxis.Z() < static_cast<real_Num>( 0.0 )
                                         ? static_cast<real_Num>( -1.0 )
                                         : static_cast<real_Num>( 1.0 );

                        translation.Z() += mouseMove.X() * xAxis.Z();
                        translation.Z() +=
                            mouseMove.Y() * ( static_cast<real_Num>( 1.0 ) - xAxis.Z() ) * yAxis;
                    }

                    translation *= m_fScale * m_translateReduction;

                    auto selection = selectionManager->getSelection();

                    for( auto selected : selection )
                    {
                        auto actor = fb::dynamic_pointer_cast<scene::IActor>( selected );
                        if( actor )
                        {
                            if( auto transform = actor->getTransform() )
                            {
                                auto entityPos = transform->getLocalPosition();

                                auto diff =
                                    Math<real_Num>::Abs( ( entityPos - translation ).squaredLength() );

                                if( diff > Math<real_Num>::epsilon() )
                                {
                                    auto localPosition = entityPos - translation;
                                    transform->setLocalPosition( localPosition );
                                    transform->setDirty( true );
                                    actor->updateTransform();
                                }
                            }
                        }
                    }

                    updateManipulatorPosition();
                    bResult = true;
                }
            }
        }

        return bResult;
    }

    auto TranslateManipulator::getLines() const -> Array<Line3<real_Num>>
    {
        Array<Line3<real_Num>> lines;
        lines.resize( 3 );

        auto pos = getPosition();
        lines[0] = Line3<real_Num>( pos, pos + Vector3F::unitX() * m_fScale );
        lines[1] = Line3<real_Num>( pos, pos + Vector3F::unitY() * m_fScale );
        lines[2] = Line3<real_Num>( pos, pos + Vector3F::unitZ() * m_fScale );

        return lines;
    }

    auto TranslateManipulator::getHandles() const -> Array<Cylinder3<real_Num>>
    {
        Array<Cylinder3<real_Num>> handles;
        handles.resize( 3 );

        auto pos = getPosition();

        auto radius = m_handleRadius * m_fScale;
        auto height = m_handleHeight * m_fScale;

        handles[0] = Cylinder3<real_Num>( Line3<real_Num>( pos, pos + Vector3<real_Num>::unitX() ),
                                          radius, height );
        handles[1] = Cylinder3<real_Num>( Line3<real_Num>( pos, pos + Vector3<real_Num>::unitY() ),
                                          radius, height );
        handles[2] = Cylinder3<real_Num>( Line3<real_Num>( pos, pos + Vector3<real_Num>::unitZ() ),
                                          radius, height );

        return handles;
    }

}  // end namespace fb
