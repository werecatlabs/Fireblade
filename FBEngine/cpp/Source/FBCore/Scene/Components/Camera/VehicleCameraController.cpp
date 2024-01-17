#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/VehicleCameraController.h>
#include <FBCore/Scene/Components/VehicleController.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb, VehicleCameraController, CameraController );

    VehicleCameraController::VehicleCameraController() = default;

    VehicleCameraController::~VehicleCameraController()
    {
        unload( nullptr );
    }

    void VehicleCameraController::load( SmartPtr<ISharedObject> data )
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

    void VehicleCameraController::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                m_target = nullptr;
                CameraController::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void VehicleCameraController::update()
    {
        auto task = Thread::getCurrentTask();
        switch( task )
        {
        case Thread::Task::Application:
        {
            auto actor = getActor();
            if( !actor )
            {
                return;
            }

            auto enabled = isEnabled() && actor->isEnabledInScene();
            if( enabled )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto sceneManager = applicationManager->getSceneManager();

                auto timer = applicationManager->getTimer();
                FB_ASSERT( timer );

                auto t = timer->getTime();
                auto dt = timer->getDeltaTime();

                auto inputDeviceManager = applicationManager->getInputDeviceManager();

                if( applicationManager->isPlaying() )
                {
                    if( !m_target )
                    {
                        auto sceneManager = applicationManager->getSceneManager();
                        auto vehicle = sceneManager->getObjectByType<VehicleController>();
                        if( vehicle )
                        {
                            m_target = vehicle->getActor();
                        }
                    }

                    if( inputDeviceManager )
                    {
                        auto mouseScrollDelta = inputDeviceManager->getMouseScroll();
                        if( mouseScrollDelta.y > 0.0f )
                        {
                            m_distance -= m_zoomSpeed * static_cast<real_Num>( dt );
                        }
                        else if( mouseScrollDelta.y < 0.0f )
                        {
                            m_distance += m_zoomSpeed * static_cast<real_Num>( dt );
                        }
                    }

                    m_distance = Math<real_Num>::clamp( m_distance, m_minDistance, m_maxDistance );

                    if( m_target )
                    {
                        auto worldTransform = m_target->getTransform();
                        if( worldTransform )
                        {
                            auto position = worldTransform->getPosition();
                            auto orientation = worldTransform->getOrientation();

                            m_targetHeight = position.y + m_height;

                            auto fRotationSnapTime =
                                m_maxRotationSnapTime - ( m_rotationSnapTime + m_minRotationSnapTime );
                            currentRotationAngle = Math<real_Num>::smoothDampAngle(
                                currentRotationAngle, m_targetRotationAngle, real_Num( 0.0 ),
                                (real_Num)dt, fRotationSnapTime );

                            m_currentHeight =
                                Math<real_Num>::lerp( m_currentHeight, m_targetHeight,
                                                      m_heightDamping * static_cast<real_Num>( dt ) );

                            m_targetPosition = position;
                            m_targetPosition.y = position.y + m_targetOffset;

                            position += Vector3<real_Num>::unitY() * real_Num( 0.1 );

                            auto cameraPosition =
                                position + ( orientation * Vector3<real_Num>::unitZ() * m_distance );
                            cameraPosition.y = m_currentHeight;

                            m_lastPosition += ( cameraPosition - m_lastPosition ) *
                                              static_cast<real_Num>( dt ) *
                                              static_cast<real_Num>( 10.0 );
                            m_lastTarget += ( m_targetPosition - m_lastTarget ) *
                                            static_cast<real_Num>( dt ) * static_cast<real_Num>( 10.0 );

                            actor->setPosition( m_lastPosition );
                            actor->lookAt( m_lastTarget, Vector3<real_Num>::unitY() );
                            //actor->updateTransform();
                        }
                    }
                }
            }
        }
        break;
        default:
        {
        }
        }
    }

    auto VehicleCameraController::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = CameraController::getChildObjects();
        objects.emplace_back( m_target );
        return objects;
    }

    auto VehicleCameraController::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = CameraController::getProperties();

        if( m_target )
        {
            auto handle = m_target->getHandle();
            auto uuid = handle->getUUID();
            properties->setProperty( "target", uuid, "resource", false );
        }
        else
        {
            properties->setProperty( "target", "", "resource", false );
        }

        properties->setProperty( "height", m_height );
        properties->setProperty( "targetOffset", m_targetOffset );
        properties->setProperty( "distance", m_distance );
        properties->setProperty( "minDistance", m_minDistance );
        properties->setProperty( "maxDistance", m_maxDistance );
        properties->setProperty( "heightDamping", m_heightDamping );
        properties->setProperty( "rotationSnapTime", m_rotationSnapTime );
        properties->setProperty( "maxRotationSnapTime", m_maxRotationSnapTime );
        properties->setProperty( "minRotationSnapTime", m_minRotationSnapTime );
        properties->setProperty( "zoomSpeed", m_zoomSpeed );

        return properties;
    }

    void VehicleCameraController::setProperties( SmartPtr<Properties> properties )
    {
        CameraController::setProperties( properties );

        properties->getPropertyValue( "height", m_height );
        properties->getPropertyValue( "targetOffset", m_targetOffset );
        properties->getPropertyValue( "distance", m_distance );
        properties->getPropertyValue( "minDistance", m_minDistance );
        properties->getPropertyValue( "maxDistance", m_maxDistance );
        properties->getPropertyValue( "heightDamping", m_heightDamping );
        properties->getPropertyValue( "rotationSnapTime", m_rotationSnapTime );
        properties->getPropertyValue( "maxRotationSnapTime", m_maxRotationSnapTime );
        properties->getPropertyValue( "minRotationSnapTime", m_minRotationSnapTime );
        properties->getPropertyValue( "zoomSpeed", m_zoomSpeed );
    }

    auto VehicleCameraController::getTarget() const -> SmartPtr<IActor>
    {
        return m_target;
    }

    void VehicleCameraController::setTarget( SmartPtr<IActor> target )
    {
        m_target = target;
    }

    auto VehicleCameraController::getDistance() const -> real_Num
    {
        return m_distance;
    }

    void VehicleCameraController::setDistance( real_Num distance )
    {
        m_distance = distance;
    }

    auto VehicleCameraController::getMinDistance() const -> real_Num
    {
        return m_minDistance;
    }

    void VehicleCameraController::setMinDistance( real_Num minDistance )
    {
        m_minDistance = minDistance;
    }

    auto VehicleCameraController::getMaxDistance() const -> real_Num
    {
        return m_maxDistance;
    }

    void VehicleCameraController::setMaxDistance( real_Num maxDistance )
    {
        m_maxDistance = maxDistance;
    }
}  // namespace fb::scene
