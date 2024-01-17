#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/CameraManager.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Interface/Graphics/ITexture.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, CameraManager, ICameraManager );

    CameraManager::CameraManager( u32 id )
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "CameraManager" );
        }
    }

    CameraManager::CameraManager()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "CameraManager" );
        }
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

        setLoadingState( LoadingState::Unloaded );
    }

    void CameraManager::update()
    {
        auto task = Thread::getCurrentTask();
        switch( task )
        {
        case Thread::Task::Application:
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( applicationManager->isEditorCamera() )
            {
                if( auto editorCamera = getEditorCamera() )
                {
                    editorCamera->update();
                }
            }

            auto sceneManager = applicationManager->getSceneManager();
            auto scene = sceneManager->getCurrentScene();

            auto cameras = scene->getComponents<Camera>();
            for( auto camera : cameras )
            {
                if( auto cameraActor = camera->getActor() )
                {
                    cameraActor->update();
                }
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

    auto CameraManager::removeCamera( SmartPtr<IActor> camera ) -> bool
    {
        if( camera )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            auto it = std::remove( m_cameras.begin(), m_cameras.end(), camera );
            if( it != m_cameras.end() )
            {
                auto cameraActor = ( *it );
                auto cameraComponent = cameraActor->getComponent<Camera>();
                if( cameraComponent )
                {
                    cameraComponent->setEditorTexture( nullptr );
                }

                m_cameras.erase( it, m_cameras.end() );
            }
        }

        return true;
    }

    auto CameraManager::findCamera( const String &name ) const -> SmartPtr<IActor>
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

    auto CameraManager::getCameras() const -> Array<SmartPtr<IActor>>
    {
        return m_cameras;
    }

    void CameraManager::reset()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto cameras = scene->getComponents<Camera>();
        for( auto camera : cameras )
        {
            camera->setActive( false );
        }

        if( applicationManager->isEditorCamera() )
        {
            if( auto editorCamera = getEditorCamera() )
            {
                auto camera = editorCamera->getComponent<Camera>();
                if( camera )
                {
                    camera->setActive( true );
                }
            }
        }
        else
        {
            if( auto editorCamera = getEditorCamera() )
            {
                auto camera = editorCamera->getComponent<Camera>();
                if( camera )
                {
                    camera->setActive( false );
                }
            }

            for( auto camera : cameras )
            {
                auto cameraActor = camera->getActor();
                if( !cameraActor->getFlag( IActor::ActorFlagIsEditor ) )
                {
                    if( auto editorTexture = getEditorTexture() )
                    {
                        camera->setEditorTexture( editorTexture );
                    }

                    auto enable = camera->isEnabled() && cameraActor->isEnabledInScene();
                    camera->setActive( enable );
                }
            }
        }
    }

    auto CameraManager::getEditorTexture() const -> SmartPtr<render::ITexture>
    {
        return m_editorTexture;
    }

    void CameraManager::setEditorTexture( SmartPtr<render::ITexture> editorTexture )
    {
        m_editorTexture = editorTexture;
    }

    auto CameraManager::getEditorCamera() const -> SmartPtr<IActor>
    {
        return m_editorCamera;
    }

    void CameraManager::setEditorCamera( SmartPtr<IActor> editorCamera )
    {
        m_editorCamera = editorCamera;
    }

    auto CameraManager::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = fb::make_ptr<Properties>();
        return properties;
    }

    void CameraManager::setProperties( SmartPtr<Properties> properties )
    {
    }

    auto CameraManager::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> objects;
        objects.emplace_back( getEditorTexture() );
        objects.emplace_back( getEditorCamera() );

        auto cameras = getCameras();
        for( auto camera : cameras )
        {
            objects.emplace_back( camera );
        }

        return objects;
    }
}  // namespace fb::scene
