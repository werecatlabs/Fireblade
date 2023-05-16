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
        }

        void CameraManager::load( SmartPtr<ISharedObject> data )
        {
        }

        void CameraManager::unload( SmartPtr<ISharedObject> data )
        {
            m_editorTexture = nullptr;

            m_cameras.clear();
            m_sceneCameras.clear();

            m_selectedCameraCtrl = nullptr;
        }

        void CameraManager::update()
        {
            switch( auto task = Thread::getCurrentTask() )
            {
            case Thread::Task::Application:
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( applicationManager->isPlaying() )
                {
                    //auto selectedCamera = getCurrentCamera();
                    //if( selectedCamera )
                    //{
                    //    if( !selectedCamera->getFlag( scene::IActor::ActorFlagIsEditor ) )
                    //    {
                    //        selectedCamera->update();
                    //    }
                    //}

                    auto cameras = getCameras();
                    for( auto &camera : cameras )
                    {
                        if( camera )
                        {
                            if( !camera->getFlag( scene::IActor::ActorFlagIsEditor ) )
                            {
                                camera->update();
                            }
                        }
                    }
                }
                else
                {
                    auto cameras = getCameras();
                    for( auto &camera : cameras )
                    {
                        if( camera )
                        {
                            if( camera->getFlag( scene::IActor::ActorFlagIsEditor ) )
                            {
                                camera->update();
                            }
                        }
                    }
                }
            }
            break;
            default:
            {
                int stop = 0;
                stop = 0;
            }
            }
        }

        bool CameraManager::postEvent( const String &cameraName, const SmartPtr<IInputEvent> &event )
        {
            return false;
        }

        void CameraManager::addCamera( SmartPtr<scene::IActor> camera )
        {
            if( camera )
            {
                RecursiveMutex::ScopedLock lock( m_camerasMutex );
                m_cameras.push_back( camera );
            }

            if( camera )
            {
                if( auto editorTexture = getEditorTexture() )
                {
                    auto cameraComponent = camera->getComponent<scene::Camera>();
                    if( cameraComponent )
                    {
                        cameraComponent->setEditorTexture( editorTexture );
                    }
                }
            }
        }

        bool CameraManager::removeCamera( SmartPtr<scene::IActor> camera )
        {
            if( camera == m_selectedCameraCtrl )
            {
                m_selectedCameraCtrl = nullptr;
            }

            if( camera )
            {
                RecursiveMutex::ScopedLock lock( m_camerasMutex );
                auto it = std::find( m_cameras.begin(), m_cameras.end(), camera );
                if( it != m_cameras.end() )
                {
                    auto cameraActor = ( *it );
                    auto cameraComponent = cameraActor->getComponent<scene::Camera>();
                    if( cameraComponent )
                    {
                        cameraComponent->setEditorTexture( nullptr );
                    }

                    m_cameras.erase( it );
                }
            }

            return true;
        }

        bool CameraManager::removeCamera( const String &name )
        {
            return true;
        }

        SmartPtr<scene::IActor> CameraManager::findCamera( const String &name ) const
        {
            auto cameras = getCameras();
            for( auto camera : cameras )
            {
                if( camera->getName() == ( name ) )
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
                    auto cameraController = selectedCamera->getComponent<scene::Camera>();
                    cameraController->setActive( false );
                }

                m_selectedCameraCtrl = camera;

                if( camera != nullptr )
                {
                    auto cameraController = camera->getComponent<scene::Camera>();
                    cameraController->setActive( true );
                }
            }
        }

        void CameraManager::setCurrentCamera( SmartPtr<scene::IActor> camera )
        {
            for( auto selectedCamera : m_cameras )
            {
                auto cameraController = selectedCamera->getComponent<scene::Camera>();
                if( cameraController )
                {
                    cameraController->setActive( false );
                }
            }

            m_selectedCameraCtrl = camera;

            if( camera != nullptr )
            {
                if( auto editorTexture = getEditorTexture() )
                {
                    auto cameraComponent = camera->getComponent<scene::Camera>();
                    if( cameraComponent )
                    {
                        cameraComponent->setEditorTexture( editorTexture );
                    }
                }

                auto cameraController = camera->getComponent<scene::Camera>();
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

        SmartPtr<scene::IActor> CameraManager::getCurrentCamera() const
        {
            return m_selectedCameraCtrl;
        }

        Array<SmartPtr<scene::IActor>> CameraManager::getCameras() const
        {
            return m_cameras;
        }

        void CameraManager::setFlag( u32 flag, bool value )
        {
        }

        bool CameraManager::getFlag( u32 flag ) const
        {
            return false;
        }

        void CameraManager::setCameraSettings( const String &name, const Properties &properties )
        {
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
                            cameraActor->setState( scene::IActor::State::Play );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( cameraActor->getFlag( scene::IActor::ActorFlagIsEditor ) )
                            {
                                setCurrentCamera( cameraActor );
                                break;
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
                            cameraActor->setState( scene::IActor::State::Edit );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( cameraActor->getFlag( scene::IActor::ActorFlagIsEditor ) )
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
                if( applicationManager->isPlaying() )
                {
                    auto cameras = getCameras();
                    if( !cameras.empty() )
                    {
                        for( auto cameraActor : cameras )
                        {
                            cameraActor->setState( scene::IActor::State::Play );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( !cameraActor->getFlag( scene::IActor::ActorFlagIsEditor ) )
                            {
                                setCurrentCamera( cameraActor );
                                break;
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
                            cameraActor->setState( scene::IActor::State::Edit );
                        }

                        for( auto cameraActor : cameras )
                        {
                            if( !cameraActor->getFlag( scene::IActor::ActorFlagIsEditor ) )
                            {
                                setCurrentCamera( cameraActor );
                                break;
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
