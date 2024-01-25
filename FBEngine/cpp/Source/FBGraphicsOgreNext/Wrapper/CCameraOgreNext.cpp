#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CViewportOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/CompositorManager.h>
#include <FBGraphicsOgreNext/Compositor.h>
#include <FBCore/FBCore.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CCameraOgreNext, ICamera );

    u32 CCameraOgreNext::m_nameExt = 0;

    CCameraOgreNext::CCameraOgreNext()
    {
        static const auto Str = String( "Camera" );
        auto name = Str + StringUtil::toString( m_nameExt++ );

        auto handle = getHandle();
        handle->setName( name );

        setupStateObject();
    }

    CCameraOgreNext::CCameraOgreNext( CSceneManagerOgreNext *creator ) : m_viewport( nullptr )
    {
        m_creator = creator;

        static const auto Str = String( "Camera" );
        auto name = Str + StringUtil::toString( m_nameExt++ );

        auto handle = getHandle();
        handle->setName( name );

        setupStateObject();
    }

    CCameraOgreNext::~CCameraOgreNext()
    {
        unload( nullptr );
    }

    void CCameraOgreNext::setupStateObject()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        auto stateContext = stateManager->addStateObject();
        setStateContext( stateContext );

        auto stateListener = factoryManager->make_ptr<CCameraStateListener>();
        stateListener->setOwner( this );
        setStateListener( stateListener );
        stateContext->addStateListener( stateListener );

        auto state = factoryManager->make_ptr<CameraState>();

        stateContext->addState( state );
        stateContext->setOwner( this );
        setGraphicsObjectState( state );

        auto renderTask = graphicsSystem->getStateTask();
        state->setTaskId( renderTask );
    }

    void CCameraOgreNext::createCompositor()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        ScopedLock lock( graphicsSystem );

        auto window = graphicsSystem->getDefaultWindow();

        auto compositor = factoryManager->make_ptr<Compositor>();
        compositor->setWorkspaceName( "Camera_" + StringUtil::getUUID() );
        compositor->setWindow( window );
        compositor->setSceneManager( getCreator() );
        compositor->setCamera( this );
        setCompositor( compositor );

        graphicsSystem->loadObject( compositor );
    }

    void CCameraOgreNext::destroyCompositor()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        ScopedLock lock( graphicsSystem );

        if( m_compositor )
        {
            m_compositor->unload( nullptr );
            m_compositor = nullptr;
        }
    }

    void CCameraOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            ScopedLock lock( graphicsSystem );

            Ogre::SceneManager *ogreSmgr = nullptr;
            if( m_creator )
            {
                m_creator->_getObject( reinterpret_cast<void **>( &ogreSmgr ) );
            }

            auto handle = getHandle();
            FB_ASSERT( handle );

            auto name = handle->getName();

            FB_ASSERT( ogreSmgr );
            m_camera = ogreSmgr->createCamera( name );

            // auto parentNode = m_camera->getParentSceneNode();
            // if (parentNode)
            //{
            //	parentNode->detachObject(m_camera);
            // }

            // some defaults
            if( m_camera )
            {
                m_camera->setPosition( Ogre::Vector3( 0.0f, 0.0f, 0.0f ) );
                m_camera->lookAt( Ogre::Vector3::ZERO );
                m_camera->setNearClipDistance( 0.02f );
                m_camera->setFarClipDistance( 10000.0f );
                m_camera->setAutoAspectRatio( true );

                //auto visible = CGraphicsObjectOgreNext<ICamera>::isVisible();
                //m_camera->setVisible( visible );
            }

            createCompositor();

            if( auto stateContext = getStateContext() )
            {
                stateContext->setDirty( true );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_ERROR( e.getFullDescription() );
        }
    }

    void CCameraOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                //FB_ASSERT( !getOwner() );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

                SmartPtr<CSceneManagerOgreNext> smgr = getCreator();
                if( smgr )
                {
                    auto ogreSmgr = smgr->getSceneManager();
                    if( ogreSmgr )
                    {
                        ogreSmgr->destroyCamera( m_camera );
                        m_camera = nullptr;
                    }
                }

                destroyCompositor();

                if( m_viewport )
                {
                    m_viewport->setCamera( nullptr );
                }

                m_targetTexture = nullptr;
                m_editorTexture = nullptr;

                if( m_viewport )
                {
                    m_viewport->unload( nullptr );
                    m_viewport = nullptr;
                }

                CGraphicsObjectOgreNext<ICamera>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_ERROR( e.getFullDescription() );
        }
    }

    void CCameraOgreNext::initialise( Ogre::Camera *camera )
    {
        m_camera = camera;

        if( m_camera )
        {
            m_camera->setAutoAspectRatio( true );
        }
    }

    void CCameraOgreNext::setViewport( SmartPtr<IViewport> viewport )
    {
        m_viewport = fb::static_pointer_cast<CViewportOgreNext>( viewport );
    }

    auto CCameraOgreNext::getViewport() const -> SmartPtr<IViewport>
    {
        return m_viewport;
    }

    auto CCameraOgreNext::clone( const String &name ) const -> SmartPtr<IGraphicsObject>
    {
        auto camera = fb::make_ptr<CCameraOgreNext>();
        return camera;
    }

    void CCameraOgreNext::_getObject( void **ppObject ) const
    {
        *ppObject = m_camera;
    }

    void CCameraOgreNext::setNearClipDistance( f32 nearDist )
    {
        if( m_camera )
        {
            m_camera->setNearClipDistance( nearDist );
        }
    }

    auto CCameraOgreNext::getNearClipDistance() const -> f32
    {
        if( m_camera )
        {
            return m_camera->getNearClipDistance();
        }

        return 0.0f;
    }

    void CCameraOgreNext::setFarClipDistance( f32 farDist )
    {
        if( m_camera )
        {
            m_camera->setFarClipDistance( farDist );
        }
    }

    auto CCameraOgreNext::getFarClipDistance() const -> f32
    {
        if( m_camera )
        {
            return m_camera->getFarClipDistance();
        }

        return 0.0f;
    }

    void CCameraOgreNext::setAspectRatio( f32 ratio )
    {
        if( m_camera )
        {
            if( ratio > MathF::epsilon() && ratio < static_cast<f32>( 1e8 ) )
            {
                m_camera->setAspectRatio( ratio );
                m_aspectRatio = ratio;
            }
        }
    }

    auto CCameraOgreNext::isVisible( const AABB3F &box ) const -> bool
    {
        Ogre::AxisAlignedBox ogreBox;
        // ogreBox.setMinimum(Ogre::Vector3(box.Min));
        // ogreBox.setMaximum(Ogre::Vector3(box.Max));

        return m_camera->isVisible( ogreBox );
    }

    auto CCameraOgreNext::isVisible( const Sphere3F &bound ) const -> bool
    {
        return false;
    }

    auto CCameraOgreNext::isVisible( const Vector3F &vert ) const -> bool
    {
        return false;
    }

    //
    // ICamera functions
    //

    void CCameraOgreNext::setPosition( const Vector3F &position )
    {
        mPosition = position;

        if( m_camera )
        {
            // m_camera->setPosition( Ogre::Vector3( mPosition.X(), mPosition.Y(), mPosition.Z() ) );
            //m_camera->isViewOutOfDate();
        }
    }

    auto CCameraOgreNext::getPosition() const -> Vector3F
    {
        Ogre::Vector3 position;  // = m_camera->getPosition();
        mPosition = Vector3F( position.ptr() );
        return mPosition;
    }

    void CCameraOgreNext::setDirection( const Vector3F &vec )
    {
        // m_camera->setPosition(Ogre::Vector3(vec));
    }

    auto CCameraOgreNext::getDirection() const -> Vector3F
    {
        return m_direction;
    }

    auto CCameraOgreNext::getDirection( const Vector2F &screenPosition ) const -> Vector3F
    {
        f32 dx = screenPosition.X();
        f32 dy = screenPosition.Y();

        Vector3F right = m_realRight;
        Vector3F up = m_realUp;

        f32 bias = 1.0f;
        Vector3F direction;
        direction += right * ( ( dx - 0.5f ) * 1.0f ) * bias;
        direction += -up * ( ( dy - 0.5f ) * ( 1.f / m_aspectRatio ) ) * bias;
        direction += m_realDirection;
        direction.normalise();

        return direction;
    }

    auto CCameraOgreNext::getDirection( const Vector2F &screenPosition, Vector3F &worldPosition ) const
        -> Vector3F
    {
        Ray3F ray = getRay( screenPosition.X(), screenPosition.Y() );
        worldPosition = ray.getDirection();

        // return getDirection(screenPosition);

        return ray.getDirection();
    }

    void CCameraOgreNext::setOrientation( const QuaternionF &q )
    {
        m_orientation = q;

        if( m_camera )
        {
            // m_camera->setOrientation( Ogre::Quaternion( q.W(), q.X(), q.Y(), q.Z() ) );
            //m_camera->isViewOutOfDate();
        }
    }

    auto CCameraOgreNext::getOrientation() const -> QuaternionF
    {
        Ogre::Quaternion orientation = m_camera->getRealOrientation();
        m_orientation.W() = orientation.w;
        m_orientation.X() = orientation.x;
        m_orientation.Y() = orientation.y;
        m_orientation.Z() = orientation.z;
        return m_orientation;
    }

    auto CCameraOgreNext::getUp() const -> Vector3F
    {
        return m_owner->getOrientation() * Vector3F::unitY();
    }

    auto CCameraOgreNext::getRight() const -> Vector3F
    {
        return m_owner->getOrientation() * Vector3F::unitX();
    }

    void CCameraOgreNext::lookAt( const Vector3F &targetPoint )
    {
        // m_camera->lookAt(Ogre::Vector3(targetPoint));
    }

    void CCameraOgreNext::roll( const f32 &angle )
    {
        // m_camera->roll(Ogre::Radian(angle));
    }

    void CCameraOgreNext::yaw( const f32 &angle )
    {
        // m_camera->yaw(Ogre::Radian(angle));
    }

    void CCameraOgreNext::pitch( const f32 &angle )
    {
        // m_camera->pitch(Ogre::Radian(angle));
    }

    void CCameraOgreNext::rotate( const Vector3F &axis, const f32 &angle )
    {
        // m_camera->rotate(Ogre::Vector3(axis), Ogre::Radian(angle));
    }

    void CCameraOgreNext::rotate( const QuaternionF &q )
    {
        // m_camera->rotate(q);
    }

    void CCameraOgreNext::setFixedYawAxis( bool useFixed, const Vector3F &fixedAxis )
    {
        // m_camera->setFixedYawAxis(useFixed, Ogre::Vector3(fixedAxis));
    }

    auto CCameraOgreNext::getDerivedOrientation() const -> QuaternionF
    {
        Ogre::Quaternion orientation = m_camera->getRealOrientation();
        m_derivedOrientation.W() = orientation.w;
        m_derivedOrientation.X() = orientation.x;
        m_derivedOrientation.Y() = orientation.y;
        m_derivedOrientation.Z() = orientation.z;
        return m_derivedOrientation;
    }

    auto CCameraOgreNext::getDerivedPosition() const -> Vector3F
    {
        return m_derivedPosition;
    }

    auto CCameraOgreNext::getDerivedDirection() const -> Vector3F
    {
        return Vector3F( m_camera->getDerivedDirection().ptr() );
    }

    auto CCameraOgreNext::getDerivedUp() const -> Vector3F
    {
        return Vector3F( m_camera->getDerivedUp().ptr() );
    }

    auto CCameraOgreNext::getDerivedRight() const -> Vector3F
    {
        return Vector3F( m_camera->getDerivedRight().ptr() );
    }

    auto CCameraOgreNext::getRealOrientation() const -> QuaternionF
    {
        auto orientation = m_camera->getRealOrientation();
        m_realOrientation.W() = orientation.w;
        m_realOrientation.X() = orientation.x;
        m_realOrientation.Y() = orientation.y;
        m_realOrientation.Z() = orientation.z;
        return m_realOrientation;
    }

    auto CCameraOgreNext::getRealPosition() const -> Vector3F
    {
        return m_realPosition;
    }

    auto CCameraOgreNext::getRealDirection() const -> Vector3F
    {
        return m_realDirection;
    }

    auto CCameraOgreNext::getRealUp() const -> Vector3F
    {
        return m_realUp;
    }

    auto CCameraOgreNext::getRealRight() const -> Vector3F
    {
        return m_realRight;
    }

    void CCameraOgreNext::setLodBias( f32 factor )
    {
        m_camera->setLodBias( factor );
    }

    auto CCameraOgreNext::getLodBias() const -> f32
    {
        if( auto camera = getCamera() )
        {
            return camera->getLodBias();
        }

        return 0.0f;
    }

    auto CCameraOgreNext::getRay( f32 screenx, f32 screeny ) const -> Ray3F
    {
        Ray3F outRay;
        getRay( screenx, screeny, outRay );
        return outRay;
    }

    void CCameraOgreNext::getRay( f32 screenX, f32 screenY, Ray3F &outRay ) const
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        ScopedLock lock( graphicsSystem );

        if( auto camera = getCamera() )
        {
            auto ray = camera->getCameraToViewportRay( screenX, screenY );

            const auto &origin = ray.getOrigin();
            const auto &direction = ray.getDirection();

            outRay.setOrigin( Vector3F( origin.x, origin.y, origin.z ) );
            outRay.setDirection( Vector3F( direction.x, direction.y, direction.z ) );
        }
    }

    auto CCameraOgreNext::getScreenPosition( const Vector3F &position ) -> Vector2F
    {
        if( auto camera = getCamera() )
        {
            Ogre::Vector3 vPosition;  // (position);
            Ogre::Vector4 vTPosition( vPosition.x, vPosition.y, vPosition.z, 1.0f );
            vTPosition = camera->getProjectionMatrix() * camera->getViewMatrix() * vTPosition;

            if( vTPosition[3] < 0 )
            {
                return { -10000, -10000 };
            }

            f32 zDiv = vTPosition[3] == 0.0f ? 1.0f : ( 1.0f / vTPosition[3] );

            // f32 x = 1.0f - (vPosition.x / vPosition.z + 0.5);
            // f32 y = vPosition.y / vPosition.z + 0.5;

            // f32 x = (s32)(dim.Width * transformedPos.ptr() * zDiv) + dim.Width;
            // f32 y = ((s32)(dim.Height - (dim.Height * (transformedPos[1] * zDiv))));
            f32 x = ( 0.5f * vTPosition[0] * zDiv ) + 0.5f;
            f32 y = ( 0.5f - ( 0.5f * ( vTPosition[1] * zDiv ) ) );

            return { x, y };
        }

        return Vector2F::zero();
    }

    void CCameraOgreNext::setWindow( f32 left, f32 top, f32 right, f32 bottom )
    {
        if( auto camera = getCamera() )
        {
            camera->setWindow( left, top, right, bottom );
        }
    }

    void CCameraOgreNext::resetWindow()
    {
        if( auto camera = getCamera() )
        {
            camera->resetWindow();
        }
    }

    auto CCameraOgreNext::isWindowSet() const -> bool
    {
        if( auto camera = getCamera() )
        {
            return camera->isWindowSet();
        }

        return false;
    }

    void CCameraOgreNext::setAutoAspectRatio( bool autoratio )
    {
        if( auto camera = getCamera() )
        {
            camera->setAutoAspectRatio( autoratio );
        }
    }

    auto CCameraOgreNext::getAutoAspectRatio() const -> bool
    {
        if( auto camera = getCamera() )
        {
            return camera->getAutoAspectRatio();
        }

        return false;
    }

    auto CCameraOgreNext::getAspectRatio() const -> f32
    {
        return m_aspectRatio;
    }

    auto CCameraOgreNext::getFOVy() const -> f32
    {
        return 0;
    }

    void CCameraOgreNext::setFOVy( f32 val )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        ScopedLock lock( graphicsSystem );

        if( auto camera = getCamera() )
        {
            camera->setFOVy( Ogre::Radian( val ) );
        }
    }

    auto CCameraOgreNext::getViewMatrix() const -> Matrix4F
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        ScopedLock lock( graphicsSystem );

        if( auto camera = getCamera() )
        {
            auto node = camera->getParentSceneNode();

            auto projectionMatrix = camera->getViewMatrix();
            auto val = &projectionMatrix[0][0];

            m_viewMatrix = Matrix4F( val );

            return m_viewMatrix;
        }

        return {};
    }

    auto CCameraOgreNext::getProjectionMatrix() const -> Matrix4F
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        ScopedLock lock( graphicsSystem );

        if( auto camera = getCamera() )
        {
            auto node = camera->getParentSceneNode();

            auto projectionMatrix = camera->getProjectionMatrix();
            auto val = &projectionMatrix[0][0];
            m_projectionMatrix = Matrix4F( val );
            return m_projectionMatrix;
        }

        return {};
    }

    auto CCameraOgreNext::getRenderViewMatrix() const -> void *
    {
        return nullptr;
    }

    auto CCameraOgreNext::getRenderProjectionMatrix() const -> void *
    {
        return nullptr;
    }

    auto CCameraOgreNext::getTargetTexture() const -> SmartPtr<ITexture>
    {
        return m_targetTexture;
    }

    void CCameraOgreNext::setTargetTexture( SmartPtr<ITexture> targetTexture )
    {
        if( m_targetTexture != targetTexture )
        {
            m_targetTexture = targetTexture;

            auto compositor = getCompositor();
            if( compositor )
            {
                compositor->reload( nullptr );
            }
        }
    }

    auto CCameraOgreNext::getEditorTexture() const -> SmartPtr<ITexture>
    {
        return m_editorTexture;
    }

    void CCameraOgreNext::setEditorTexture( SmartPtr<ITexture> editorTexture )
    {
        m_editorTexture = editorTexture;
    }

    void CCameraOgreNext::setProperties( SmartPtr<Properties> properties )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto cameraState = stateContext->getStateByType<CameraState>() )
            {
                bool visible = cameraState->isVisible();
                properties->getPropertyValue( "isVisible", visible );

                setVisible( visible );
            }
        }
    }

    auto CCameraOgreNext::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = CGraphicsObjectOgreNext<ICamera>::getChildObjects();

        objects.emplace_back( m_compositor );
        objects.emplace_back( m_targetTexture );
        objects.emplace_back( m_editorTexture );
        objects.emplace_back( m_viewport );

        return objects;
    }

    auto CCameraOgreNext::getCamera() const -> Ogre::Camera *
    {
        //auto p = m_camera.load();
        //return p;

        return m_camera.get();
    }

    void CCameraOgreNext::setCamera( Ogre::Camera *camera )
    {
        m_camera = camera;
    }

    auto CCameraOgreNext::getCompositor() const -> SmartPtr<Compositor>
    {
        return m_compositor;
    }

    void CCameraOgreNext::setCompositor( SmartPtr<Compositor> compositor )
    {
        m_compositor = compositor;
    }

    void CCameraOgreNext::CCameraStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        ScopedLock lock( graphicsSystem );

        auto cameraState = fb::static_pointer_cast<CameraState>( state );
        auto visible = cameraState->isVisible();
        auto renderQueueGroup = cameraState->getRenderQueueGroup();
        auto flags = cameraState->getFlags();

        if( auto owner = getOwner() )
        {
            if( owner->isLoaded() )
            {
                if( auto camera = owner->getCamera() )
                {
                    camera->setVisible( visible );
                    //camera->setRenderQueueGroup( renderQueueGroup );
                    //camera->setVisibilityFlags( flags );
                }

                //if( visible )
                //{
                //    auto compositor = owner->getCompositor();
                //    if( !compositor )
                //    {
                //        owner->createCompositor();
                //    }
                //}
                //else
                //{
                //    owner->destroyCompositor();
                //}

                if( auto compositor = owner->getCompositor() )
                {
                    //if( compositor->isLoaded() )
                    {
                        //if( compositor->isEnabled() != visible )
                        {
                            compositor->setEnabled( visible );
                        }

                        //state->setDirty( false );
                    }
                }

                state->setDirty( false );
            }
        }
    }

    void CCameraOgreNext::CCameraStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();

        ScopedLock lock( graphicsSystem );

        if( message->isExactly<StateMessageVisible>() )
        {
            auto visibleMessage = fb::static_pointer_cast<StateMessageVisible>( message );
            FB_ASSERT( visibleMessage );

            auto visible = visibleMessage->isVisible();
            m_owner->setVisible( visible );
        }
    }

    void CCameraOgreNext::CCameraStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    void CCameraOgreNext::CCameraStateListener::setOwner( CCameraOgreNext *owner )
    {
        m_owner = owner;
    }

    auto CCameraOgreNext::CCameraStateListener::getOwner() const -> CCameraOgreNext *
    {
        return m_owner;
    }

    void CCameraOgreNext::setRenderUI( bool enabled )
    {
        m_renderUI = enabled;
    }

    auto CCameraOgreNext::getRenderUI() const -> bool
    {
        return m_renderUI;
    }

    auto CCameraOgreNext::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = CGraphicsObjectOgreNext<ICamera>::getProperties();

        if( auto stateContext = getStateContext() )
        {
            if( auto cameraState = stateContext->getStateByType<CameraState>() )
            {
                properties->setProperty( "isVisible", cameraState->isVisible() );
            }
        }

        return properties;
    }
}  // namespace fb::render
