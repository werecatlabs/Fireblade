#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/CameraManager.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/Interface/Graphics/ITexture.h>

namespace fb
{
    namespace scene
    {
        CameraManager::CameraManager( u32 id )
        {
        }

        CameraManager::~CameraManager()
        {
            unload( nullptr );
        }

        void CameraManager::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loading );

            m_cameras.reserve( 12 );

            setLoadingState( LoadingState::Loaded );
        }

        void CameraManager::unload( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Unloading );

            m_editorTexture = nullptr;

            m_cameras.clear();

            m_selectedCamera = nullptr;

            setLoadingState( LoadingState::Unloaded );
        }

        void CameraManager::update()
        {
            switch( auto task = Thread::getCurrentTask() )
            {
            case Thread::Task::Application:
            {
                if( auto selectedCamera = getCurrentCamera() )
                {
                    selectedCamera->update();
                }
            }
            break;
            default:
            {
            }
            }
        }

        void CameraManager::addCamera( SmartPtr<IActor> camera )
        {
            if( camera )
            {
                RecursiveMutex::ScopedLock lock( m_mutex );
                m_cameras.push_back( camera );
            }

            if( camera )
            {
                if( auto editorTexture = getEditorTexture() )
                {
                    auto cameraComponent = camera->getComponent<Camera>();
                    if( cameraComponent )
                    {
                        cameraComponent->setEditorTexture( editorTexture );
                    }
                }
            }
        }

        bool CameraManager::removeCamera( SmartPtr<IActor> camera )
        {
            if( camera == m_selectedCamera )
            {
                m_selectedCamera = nullptr;
            }

            if( camera )
            {
                RecursiveMutex::ScopedLock lock( m_mutex );
                auto it = std::find( m_cameras.begin(), m_cameras.end(), camera );
                if( it != m_cameras.end() )
                {
                    auto cameraActor = ( *it );
                    auto cameraComponent = cameraActor->getComponent<Camera>();
                    if( cameraComponent )
                    {
                        cameraComponent->setEditorTexture( nullptr );
                    }

                    m_cameras.erase( it );
                }
            }

            return true;
        }

        SmartPtr<IActor> CameraManager::findCamera( const String &name ) const
        {
            auto cameras = getCameras();
            for( auto camera : cameras )
            {
                if( camera->getName() == name )
                {
                    return camera;
                }
            }

            return nullptr;
        }

        void CameraManager::setCurrentCamera( const String &name )
        {
            auto camera = findCamera( name );
            if( camera != nullptr )
            {
                auto selectedCamera = getCurrentCamera();
                if( selectedCamera != nullptr )
                {
                    auto cameraController = selectedCamera->getComponent<Camera>();
                    cameraController->setActive( false );
                }

                m_selectedCamera = camera;

                if( camera != nullptr )
                {
                    auto cameraController = camera->getComponent<Camera>();
                    cameraController->setActive( true );
                }
            }
        }

        void CameraManager::setCurrentCamera( SmartPtr<IActor> camera )
        {
            for( auto camera : m_cameras )
            {
                auto cameraController = camera->getComponent<Camera>();
                if( cameraController )
                {
                    cameraController->setActive( false );
                }
            }

            m_selectedCamera = camera;

            if( camera != nullptr )
            {
                if( auto editorTexture = getEditorTexture() )
                {
                    auto cameraComponent = camera->getComponent<Camera>();
                    if( cameraComponent )
                    {
                        cameraComponent->setEditorTexture( editorTexture );
                    }
                }

                auto cameraController = camera->getComponent<Camera>();
                if( cameraController )
                {
                    cameraController->setActive( true );
                }
            }
        }

        String CameraManager::getCurrentCameraName() const
        {
            auto selectedCamera = getCurrentCamera();
            if( selectedCamera != nullptr )
            {
                return selectedCamera->getName();
            }

            return StringUtil::EmptyString;
        }

        SmartPtr<IActor> CameraManager::getCurrentCamera() const
        {
            return m_selectedCamera;
        }

        Array<SmartPtr<IActor>> CameraManager::getCameras() const
        {
            return m_cameras;
        }

        void CameraManager::play()
        {
            reset();
        }

        void CameraManager::edit()
        {
            reset();
        }

        void CameraManager::stop()
        {
        }

        void CameraManager::reset()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( applicationManager->isEditorCamera() )
            {
                if( applicationManager->isPlaying() )
                {
                    auto cameras = getCameras();
                    if( !cameras.empty() )
                    {
                        for( auto cameraActor : cameras )
                        {
                            cameraActor->setState( IActor::State::Play );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( cameraActor->isEnabledInScene() )
                            {
                                if( cameraActor->getFlag( IActor::ActorFlagIsEditor ) )
                                {
                                    setCurrentCamera( cameraActor );
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    auto cameras = getCameras();
                    if( !cameras.empty() )
                    {
                        for( auto cameraActor : cameras )
                        {
                            cameraActor->setState( IActor::State::Edit );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( cameraActor->isEnabledInScene() )
                            {
                                if( cameraActor->getFlag( IActor::ActorFlagIsEditor ) )
                                {
                                    setCurrentCamera( cameraActor );
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if( applicationManager->isPlaying() )
                {
                    auto cameras = getCameras();
                    if( !cameras.empty() )
                    {
                        for( auto cameraActor : cameras )
                        {
                            cameraActor->setState( IActor::State::Play );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( cameraActor->isEnabledInScene() )
                            {
                                if( !cameraActor->getFlag( IActor::ActorFlagIsEditor ) )
                                {
                                    setCurrentCamera( cameraActor );
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    auto cameras = getCameras();
                    if( !cameras.empty() )
                    {
                        for( auto cameraActor : cameras )
                        {
                            cameraActor->setState( IActor::State::Edit );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( cameraActor->isEnabledInScene() )
                            {
                                if( !cameraActor->getFlag( IActor::ActorFlagIsEditor ) )
                                {
                                    setCurrentCamera( cameraActor );
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        SmartPtr<render::ITexture> CameraManager::getEditorTexture() const
        {
            return m_editorTexture.lock();
        }

        void CameraManager::setEditorTexture( SmartPtr<render::ITexture> editorTexture )
        {
            m_editorTexture = editorTexture;
        }
    }  // namespace scene
}  // end namespace fb
