#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Camera.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto window = graphicsSystem->getDefaultWindow();
                FB_ASSERT( window );

                auto smgr = graphicsSystem->getGraphicsScene();
                FB_ASSERT( smgr );

                auto cameraName = String( "CameraComponent" ) + StringUtil::toString( m_nameExt++ );
                auto camera = smgr->addCamera( cameraName );
                FB_ASSERT( camera );

                if( !actor->getFlag( scene::IActor::ActorFlagIsEditor ) )
                {
                    camera->setRenderUI( false );
                }

                m_camera = camera;
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

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

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

                        if( smgr->getActiveCamera() == m_camera )
                        {
                            smgr->setActiveCamera( nullptr );
                        }

                        if( m_viewport )
                        {
                            m_viewport->setRenderTarget( nullptr );

                            window->removeViewport( m_viewport );
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

                    Component::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<render::ITexture> Camera::getTargetTexture() const
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

        SmartPtr<render::ITexture> Camera::getEditorTexture() const
        {
            return m_editorTexture.lock();
        }

        SmartPtr<Properties> Camera::getProperties() const
        {
            if( auto properties = Component::getProperties() )
            {
                properties->setProperty( "isActive", m_isActive );

                return properties;
            }

            return nullptr;
        }

        void Camera::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "isActive", m_isActive );
            Component::setProperties( properties );
        }

        Array<SmartPtr<ISharedObject>> Camera::getChildObjects() const
        {
            auto objects = Component::getChildObjects();

            objects.push_back( m_camera );
            objects.push_back( m_node );

            return objects;
        }

        void Camera::setEditorTexture( SmartPtr<render::ITexture> editorTexture )
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

        SmartPtr<render::ICamera> Camera::getCamera() const
        {
            return m_camera;
        }

        void Camera::setCamera( SmartPtr<render::ICamera> camera )
        {
            m_camera = camera;
        }

        SmartPtr<render::ISceneNode> Camera::getNode() const
        {
            return m_node;
        }

        void Camera::setNode( SmartPtr<render::ISceneNode> node )
        {
            m_node = node;
        }

        bool Camera::isActive() const
        {
            return m_isActive;
        }

        void Camera::setActive( bool active )
        {
            m_isActive = active;

            updateActiveState( active );
        }

        void Camera::setupCamera()
        {
            bool active = isActive();

            //if( auto editorTexture = getEditorTexture() )
            //{
            //    auto rt = editorTexture->getRenderTarget();
            //    auto viewports = rt->getViewports();
            //    if( !viewports.empty() )
            //    {
            //        m_viewport = viewports.front();
            //    }
            //}

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto smgr = graphicsSystem->getGraphicsScene();
            FB_ASSERT( smgr );

            if( active )
            {
                m_camera->setVisible( true );

                if( smgr->getActiveCamera() != m_camera )
                {
                    smgr->setActiveCamera( m_camera );
                }

                if( m_viewport )
                {
                    m_viewport->setCamera( m_camera );
                }
            }
            else
            {
                m_camera->setVisible( false );

                if( m_viewport )
                {
                    m_viewport->setCamera( nullptr );
                }

                if( smgr->getActiveCamera() == m_camera )
                {
                    smgr->setActiveCamera( nullptr );
                }
            }
        }

        void Camera::updateTransform()
        {
            auto state = getState();
            switch( state )
            {
            case State::Edit:
            case State::Play:
            {
                auto actor = getActor();
                if( actor )
                {
                    auto p = actor->getPosition();
                    auto r = actor->getOrientation();

                    m_node->setPosition( p );
                    m_node->setOrientation( r );

                    // m_camera->setPosition( p );
                    // m_camera->setOrientation( r );

#ifdef _DEBUG
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    FB_ASSERT( graphicsSystem );

                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    // auto activeCamera = smgr->getActiveCamera();
                    // FB_ASSERT( isActive() && activeCamera == m_camera );
#endif
                }
            }
            break;
            default:
            {
            }
            break;
            }
        }

        void Camera::updateOrder()
        {
            if( auto actor = getActor() )
            {
                if( actor->getFlag( IActor::ActorFlagIsEditor ) )
                {
                    setZOrder( 0 );
                    return;
                }
            }

            auto applicationManager = core::IApplicationManager::instance();
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

        s32 Camera::getZOrder( SmartPtr<IActor> obj )
        {
            if( auto actor = getActor() )
            {
                auto children = actor->getAllComponentsInChildren<Camera>();

                auto count = 100;
                for( auto &child : children )
                {
                    if( child->getActor() == obj )
                    {
                        return count;
                    }

                    ++count;
                }
            }

            return 0;
        }

        u32 Camera::getZOrder() const
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

        SmartPtr<render::IRenderTarget> Camera::getRenderTarget() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( applicationManager->isEditor() )
            {
                auto t = m_editorTexture.lock();
                return t->getRenderTarget();
            }

            return applicationManager->getWindow();
        }

        void Camera::updateDirty( u32 flags, u32 oldFlags )
        {
            auto applicationManager = core::IApplicationManager::instance();
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
        }

        IFSM::ReturnType Camera::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            auto applicationManager = core::IApplicationManager::instance();
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
                    auto actor = getActor();
                    if( actor->getFlag( IActor::ActorFlagIsEditor ) )
                    {
                        setupCamera();
                    }

                    updateOrder();
                    createViewport();

                    auto active = isActive();
                    updateActiveState( active );
                }
                break;
                case State::Play:
                {
                    auto actor = getActor();
                    if( !actor->getFlag( IActor::ActorFlagIsEditor ) )
                    {
                        setupCamera();
                    }

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

                    if( smgr->getActiveCamera() == m_camera )
                    {
                        smgr->setActiveCamera( nullptr );
                    }

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
#if 1
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( applicationManager->isEditorCamera() )
            {
                auto actor = getActor();
                if( !actor->getFlag( scene::IActor::ActorFlagIsEditor ) )
                {
                    m_camera->setRenderUI( false );

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
                    m_camera->setRenderUI( false );

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
            else
            {
                m_camera->setRenderUI( false );

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
            if( m_viewport )
            {
                m_viewport->setActive( active );
            }
#endif
        }

    }  // namespace scene
}  // end namespace fb
