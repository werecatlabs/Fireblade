#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera/VehicleCameraController.h>
#include <FBCore/Scene/Components/VehicleController.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb, VehicleCameraController, CameraController );

        VehicleCameraController::VehicleCameraController()
        {
        }

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
            switch( auto task = Thread::getCurrentTask() )
            {
            case Thread::Task::Application:
            {
                auto applicationManager = core::IApplicationManager::instance();
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
                            m_distance -= m_ZoomSpeed * static_cast<real_Num>( dt );
                        }
                        else if( mouseScrollDelta.y < 0.0f )
                        {
                            m_distance += m_ZoomSpeed * static_cast<real_Num>( dt );
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

                            //static Vector3F debugPos = Vector3F(0,1,-5);
                            //position = debugPos;
                            //orientation = QuaternionF::identity();

                            wantedHeight = position.y + m_height;
                            currentHeight = position.y;

                            auto fRotationSnapTime =
                                m_MaxRotationSnapTime - ( m_rotationSnapTime + m_MinRotationSnapTime );
                            currentRotationAngle = wantedRotationAngle;
                            //Math<real_Num>::smoothDampAngle( currentRotationAngle, wantedRotationAngle,
                            //                       ref yVelocity, fRotationSnapTime );

                            currentHeight = Math<real_Num>::lerp( currentHeight, wantedHeight,
                                                                  m_heightDamping * dt );

                            wantedPosition = position;
                            wantedPosition.y = currentHeight;

                            position += Vector3<real_Num>::unitY() * 0.1f;

                            auto cameraPosition =
                                position + ( orientation * Vector3<real_Num>::unitZ() * m_distance );
                            cameraPosition += Vector3<real_Num>::unitY() * currentHeight;

                            auto targetPosition = m_target->getPosition();

                            m_LastPosition += ( cameraPosition - m_LastPosition ) * dt * 10.0;
                            m_LastTarget += ( targetPosition - m_LastTarget ) * dt * 10.0;

                            // if (m_cameraSceneNode)
                            //{
                            //	m_cameraSceneNode->setPosition(
                            //			Vector3F(cameraPosition.X(), cameraPosition.Y(),
                            // cameraPosition.Z())); 	m_cameraSceneNode->lookAt(Vector3F(position.X(),
                            // position.Y(), position.Z()));
                            // }

                            if( auto actor = getActor() )
                            {
                                actor->setPosition( m_LastPosition );
                                actor->lookAt( m_LastTarget, Vector3<real_Num>::unitY() );
                                actor->updateTransform();

                                /*auto vec = m_LastTarget - m_LastPosition;
                                auto rot = MathUtil<real_Num>::getOrientationFromDirection(
                                   vec, -Vector3<real_Num>::unitZ(), true,  Vector3<real_Num>::up() );

                                auto transform =
                                    Transform3<real_Num>( m_LastPosition, rot, Vector3F::unit() );
                                auto id = getActor()->getTransform()->getHandle()->getInstanceId();
                                sceneManager->addTransformState( id, t, transform );*/
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

        Array<SmartPtr<ISharedObject>> VehicleCameraController::getChildObjects() const
        {
            auto objects = CameraController::getChildObjects();
            return objects;
        }

        SmartPtr<Properties> VehicleCameraController::getProperties() const
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

            return properties;
        }

        void VehicleCameraController::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "height", m_height );

            CameraController::setProperties( properties );
        }

        SmartPtr<IActor> VehicleCameraController::getTarget() const
        {
            return m_target;
        }

        void VehicleCameraController::setTarget( SmartPtr<IActor> target )
        {
            m_target = target;
        }

        real_Num VehicleCameraController::getDistance() const
        {
            return m_distance;
        }

        void VehicleCameraController::setDistance( real_Num distance )
        {
            m_distance = distance;
        }

        real_Num VehicleCameraController::getMinDistance() const
        {
            return m_minDistance;
        }

        void VehicleCameraController::setMinDistance( real_Num minDistance )
        {
            m_minDistance = minDistance;
        }

        real_Num VehicleCameraController::getMaxDistance() const
        {
            return m_maxDistance;
        }

        void VehicleCameraController::setMaxDistance( real_Num maxDistance )
        {
            m_maxDistance = maxDistance;
        }
    }  // namespace scene
}  // namespace fb
