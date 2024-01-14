#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Graphics/IViewport.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Camera, Component );
    u32 Camera::m_nameExt = 0;
    u32 Camera::m_zorderExt = 0;

    Camera::Camera() : Component()
    {
    }

    Camera::~Camera()
    {
        unload( nullptr );
    }

    void Camera::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Component::load( data );

            auto actor = getActor();

            auto applicationManager = core::ApplicationManager::instance();
            auto sceneManager = applicationManager->getSceneManager();

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto window = graphicsSystem->getDefaultWindow();
            FB_ASSERT( window );

            auto smgr = graphicsSystem->getGraphicsScene();
            FB_ASSERT( smgr );

            auto cameraName = String( "CameraComponent" ) + StringUtil::toString( m_nameExt++ );
            auto camera = smgr->addCamera( cameraName );
            FB_ASSERT( camera );

            if( !actor->getFlag( IActor::ActorFlagIsEditor ) )
            {
                camera->setRenderUI( false );
            }

            m_camera = camera;

            m_camera->setVisible( false );

            m_node = m_camera->getOwner();
            if( !m_node )
            {
                auto rootNode = smgr->getRootSceneNode();
                FB_ASSERT( rootNode );

                if( rootNode )
                {
                    auto cameraNodeName =
                        String( "CameraComponentNode" ) + StringUtil::toString( m_nameExt++ );
                    m_node = rootNode->addChildSceneNode( cameraNodeName );
                    if( m_node )
                    {
                        m_node->attachObject( m_camera );
                    }
                }
            }

            auto cameraManager = applicationManager->getCameraManager();
            if( cameraManager )
            {
                auto cameraActor = getActor();
                cameraManager->addCamera( cameraActor );
            }

            m_camera->setAutoAspectRatio( true );

            setActive( false );

            sceneManager->registerComponentUpdate( Thread::Task::Render, Thread::UpdateState::Transform,
                                                   this );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Camera::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                auto sceneManager = applicationManager->getSceneManager();

                if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
                {
                    auto window = graphicsSystem->getDefaultWindow();
                    FB_ASSERT( window );

                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    m_editorTexture = nullptr;

                    auto cameraManager = applicationManager->getCameraManager();
                    if( cameraManager )
                    {
                        auto cameraActor = getActor();
                        cameraManager->removeCamera( cameraActor );
                    }

                    if( m_camera )
                    {
                        m_camera->setVisible( false );
                    }

                    if( m_viewport )
                    {
                        m_viewport->setCamera( nullptr );
                    }

                    if( m_viewport )
                    {
                        m_viewport->setRenderTarget( nullptr );

                        if( auto renderTarget = getRenderTarget() )
                        {
                            renderTarget->removeViewport( m_viewport );
                        }
                        else
                        {
                            window->removeViewport( m_viewport );
                        }

                        m_viewport = nullptr;
                    }

                    if( m_camera )
                    {
                        m_camera->setEditorTexture( nullptr );
                        m_camera->setTargetTexture( nullptr );

                        if( m_camera->isAttached() )
                        {
                            if( m_node )
                            {
                                m_camera->detachFromParent( m_node );
                            }
                        }

                        smgr->removeGraphicsObject( m_camera );
                        m_camera = nullptr;
                    }

                    if( m_node )
                    {
                        smgr->removeSceneNode( m_node );
                        m_node = nullptr;
                    }
                }

                sceneManager->unregisterAllComponent( this );

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Camera::getTargetTexture() const -> SmartPtr<render::ITexture>
    {
        return m_camera->getTargetTexture();
    }

    void Camera::setTargetTexture( SmartPtr<render::ITexture> targetTexture )
    {
        if( targetTexture )
        {
            if( !m_viewport )
            {
                createViewport();
            }

            auto active = isActive();
            updateActiveState( active );
        }

        m_camera->setTargetTexture( targetTexture );
    }

    auto Camera::getEditorTexture() const -> SmartPtr<render::ITexture>
    {
        return m_editorTexture;
    }

    auto Camera::getProperties() const -> SmartPtr<Properties>
    {
        if( auto properties = Component::getProperties() )
        {
            properties->setProperty( "zOrder", m_zOrder );
            properties->setProperty( "isActive", m_isActive );
            properties->setPropertyAsButton( "reset", "reset" );
            properties->setPropertyAsButton( "updateActiveState", "updateActiveState" );
            return properties;
        }

        return nullptr;
    }

    void Camera::setProperties( SmartPtr<Properties> properties )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto cameraManager = applicationManager->getCameraManager();

        auto wasEnabled = isEnabled();

        properties->getPropertyValue( "zOrder", m_zOrder );

        auto active = isActive();
        properties->getPropertyValue( "isActive", active );

        if( properties->isButtonPressed( "reset" ) )
        {
            cameraManager->reset();
        }

        if( properties->isButtonPressed( "updateActiveState" ) )
        {
            m_isActive = true;
            updateActiveState( m_isActive );
        }

        Component::setProperties( properties );

        auto enabled = isEnabled();

        if( wasEnabled != enabled )
        {
            cameraManager->reset();
        }

        if( m_isActive != active )
        {
            m_isActive = active;
            cameraManager->reset();
        }
    }

    auto Camera::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Component::getChildObjects();

        objects.emplace_back( m_camera );
        objects.emplace_back( m_node );

        return objects;
    }

    void Camera::setEditorTexture( SmartPtr<render::ITexture> editorTexture )
    {
        if( m_editorTexture != editorTexture )
        {
            m_editorTexture = editorTexture;

            if( m_editorTexture )
            {
                if( !m_viewport )
                {
                    createViewport();
                }

                auto active = isActive();
                updateActiveState( active );
            }

            if( m_camera )
            {
                m_camera->setEditorTexture( editorTexture );
            }
        }
    }

    auto Camera::getCamera() const -> SmartPtr<render::ICamera>
    {
        return m_camera;
    }

    void Camera::setCamera( SmartPtr<render::ICamera> camera )
    {
        m_camera = camera;
    }

    auto Camera::getNode() const -> SmartPtr<render::ISceneNode>
    {
        return m_node;
    }

    void Camera::setNode( SmartPtr<render::ISceneNode> node )
    {
        m_node = node;
    }

    auto Camera::isActive() const -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( applicationManager->isEditorCamera() )
        {
            if( auto actor = getActor() )
            {
                if( actor->getFlag( IActor::ActorFlagIsEditor ) )
                {
                    return true;
                }
            }
        }

        auto actor = getActor();
        auto enabled = isEnabled() && actor->isEnabledInScene();
        return enabled && m_isActive;
    }

    void Camera::setActive( bool active )
    {
        m_isActive = active;

        updateActiveState( isActive() );
    }

    void Camera::updateTransform()
    {
        switch( auto task = Thread::getCurrentTask() )
        {
        case Thread::Task::Application:
        {
            auto state = getState();
            switch( state )
            {
            case State::Edit:
            case State::Play:
            {
                if( isActive() )
                {
                    if( auto actor = getActor() )
                    {
                        if( auto transform = actor->getTransform() )
                        {
                            auto t = transform->getWorldTransform();

                            auto p = t.getPosition();
                            auto r = t.getOrientation();

                            m_node->setPosition( p );
                            m_node->setOrientation( r );
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
        }
        break;
        case Thread::Task::Render:
        {
        }
        break;
        default:
        {
        }
        break;
        }
    }

    auto Camera::getCameraToViewportRay( const Vector2<real_Num> &screenPosition ) -> Ray3<real_Num>
    {
        if( auto camera = getCamera() )
        {
            //Ray3<real_Num> ray;

            //auto viewport = getViewport();
            //f32 width = viewport->getActualWidth();
            //f32 height = viewport->getActualHeight();

            //Vector2F normalizedScreenCoords;
            //normalizedScreenCoords.X() = screenPosition.X() / width;
            //normalizedScreenCoords.Y() = screenPosition.Y() / height;

            //Ogre::Ray ogreRay = m_selectedCamera->getCameraToViewportRay(
            //    normalizedScreenCoords.X(), normalizedScreenCoords.Y() );

            //Ogre::Vector3 ogreCameraPosition = m_selectedCamera->getDerivedPosition();
            //Ogre::Vector3 rayOrigin = ogreRay.getOrigin();
            //Ogre::Vector3 rayDirection = ogreRay.getDirection();
            //ray.Start = Vector3F( &rayOrigin[0] );
            //ray.Direction = Vector3F( &rayDirection[0] );

            //ray.Start.Z() = -ray.Start.Z();
            //ray.Direction.Z() = -ray.Direction.Z();

            //return ray;

            return camera->getRay( screenPosition.x, screenPosition.y );
        }

        return {};
    }

    auto Camera::isInFrustum( const AABB3F &box ) const -> bool
    {
        return true;
    }

    void Camera::updateOrder()
    {
        if( auto actor = getActor() )
        {
            if( actor->getFlag( IActor::ActorFlagIsEditor ) )
            {
                setZOrder( 100 );
                return;
            }
        }

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto zorder = 1;

        auto cameras = Array<SmartPtr<Camera>>();
        cameras.reserve( 12 );

        auto actors = scene->getActors();
        for( auto actor : actors )
        {
            auto cameras = actor->getComponentsAndInChildren<Camera>();
            for( auto camera : cameras )
            {
                cameras.push_back( camera );
            }
        }

        for( auto camera : cameras )
        {
            if( camera == this )
            {
                break;
            }

            ++zorder;
        }

        setZOrder( zorder );
    }

    auto Camera::getZOrder( SmartPtr<IActor> other ) -> s32
    {
        if( auto actor = getActor() )
        {
            auto children = actor->getAllComponentsInChildren<Camera>();

            auto count = 100;
            for( auto &child : children )
            {
                if( child->getActor() == other )
                {
                    return count;
                }

                ++count;
            }
        }

        return 0;
    }

    auto Camera::getZOrder() const -> u32
    {
        return m_zOrder;
    }

    void Camera::setZOrder( u32 zOrder )
    {
        m_zOrder = zOrder;

        if( m_viewport )
        {
            m_viewport->setZOrder( zOrder );
        }
    }

    void Camera::updateTransform( const Transform3<real_Num> &t )
    {
        auto actor = getActor();
        auto editorActor = actor->getFlag( IActor::ActorFlagIsEditor );

        if( isActive() )
        {
            if( m_node )
            {
                auto p = t.getPosition();
                auto r = t.getOrientation();

                m_node->setPosition( p );
                m_node->setOrientation( r );
            }
        }
    }

    void Camera::createViewport()
    {
        if( !m_viewport )
        {
            if( auto renderTarget = getRenderTarget() )
            {
                updateOrder();

                auto zOrder = getZOrder();
                auto vp = renderTarget->addViewport( 0, m_camera, zOrder );
                FB_ASSERT( vp );

                auto viewportColour = ColourF::Blue * 0.5f;
                vp->setBackgroundColour( viewportColour );

                vp->setEnableUI( false );
                vp->setClearEveryFrame( true );
                vp->setOverlaysEnabled( true );
                vp->setAutoUpdated( true );

                vp->setActive( false );

                m_viewport = vp;
            }
        }
    }

    auto Camera::getRenderTarget() const -> SmartPtr<render::IRenderTarget>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( applicationManager->isEditor() )
        {
            if( auto t = getEditorTexture() )
            {
                return t->getRenderTarget();
            }

            return nullptr;
        }

        return applicationManager->getWindow();
    }

    void Camera::updateFlags( u32 flags, u32 oldFlags )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto smgr = graphicsSystem->getGraphicsScene();
        FB_ASSERT( smgr );

        auto cameraManager = applicationManager->getCameraManager();

        if( auto actor = getActor() )
        {
            if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
            {
            }
            else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                     BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
            {
            }
        }

        auto active = isActive();
        updateActiveState( active );
    }

    auto Camera::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto smgr = graphicsSystem->getGraphicsScene();
        FB_ASSERT( smgr );

        auto cameraManager = applicationManager->getCameraManager();

        switch( eventType )
        {
        case IFSM::Event::Change:
        {
        }
        break;
        case IFSM::Event::Enter:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Edit:
            {
                updateOrder();
                createViewport();

                auto active = isActive();
                updateActiveState( active );
            }
            break;
            case State::Play:
            {
                updateOrder();
                createViewport();

                auto active = isActive();
                updateActiveState( active );
            }
            break;
            default:
            {
            }
            }
        }
        break;
        case IFSM::Event::Leave:
        {
            auto eState = static_cast<State>( state );
            switch( eState )
            {
            case State::Edit:
            case State::Play:
            {
                m_camera->setVisible( false );

                if( m_viewport )
                {
                    m_viewport->setCamera( nullptr );
                }

                setActive( false );
                updateActiveState( false );
            }
            break;
            default:
            {
            }
            }
        }
        break;
        case IFSM::Event::Pending:
            break;
        case IFSM::Event::Complete:
            break;
        case IFSM::Event::NewState:
            break;
        case IFSM::Event::WaitForChange:
            break;
        default:
        {
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    void Camera::updateActiveState( bool active )
    {
#if 0
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( applicationManager->isEditorCamera() )
            {
                if( auto actor = getActor() )
                {
                    if( !actor->getFlag( scene::IActor::ActorFlagIsEditor ) )
                    {
                        if( m_camera )
                        {
                            m_camera->setRenderUI( false );
                        }

                        if( m_viewport )
                        {
                            m_viewport->setEnableUI( false );
                            m_viewport->setActive( false );
                        }

                        if( m_camera )
                        {
                            m_camera->setVisible( false );
                        }
                    }
                    else
                    {
                        if( m_camera )
                        {
                            m_camera->setRenderUI( false );
                        }

                        if( m_viewport )
                        {
                            m_viewport->setEnableUI( false );
                            m_viewport->setActive( active );
                        }

                        if( m_camera )
                        {
                            m_camera->setVisible( active );
                        }
                    }
                }
            }
            else
            {
                if( m_camera )
                {
                    m_camera->setRenderUI( false );
                }

                if( m_viewport )
                {
                    m_viewport->setEnableUI( false );
                    m_viewport->setActive( active );
                }

                if( m_camera )
                {
                    m_camera->setVisible( active );
                }
            }
#else
        if( m_camera )
        {
            m_camera->setVisible( active );
        }

        if( m_viewport )
        {
            m_viewport->setActive( active );
        }
#endif
    }

    auto Camera::getViewport() const -> SmartPtr<render::IViewport>
    {
        return m_viewport;
    }

    void Camera::setViewport( SmartPtr<render::IViewport> viewport )
    {
        m_viewport = viewport;
    }
}  // namespace fb::scene
