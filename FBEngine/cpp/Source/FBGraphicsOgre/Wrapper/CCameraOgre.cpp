#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CCameraOgre.h>
#include <FBGraphicsOgre/Wrapper/CViewportOgre.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSceneOgre.h>
#include <FBCore/FBCore.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb, CCameraOgre, ICamera );

        u32 CCameraOgre::m_nameExt = 0;

        CCameraOgre::CCameraOgre()
        {
            setupStateObject();
        }

        CCameraOgre::~CCameraOgre()
        {
            unload( nullptr );
        }

        void CCameraOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                Ogre::SceneManager *ogreSmgr = nullptr;
                if( m_creator )
                {
                    m_creator->_getObject( (void **)&ogreSmgr );
                }

                auto handle = getHandle();
                auto name = handle->getName();

                FB_ASSERT( ogreSmgr );
                auto ogreCamera = ogreSmgr->createCamera( name );
                m_camera = ogreCamera;

                setMovable( m_camera );

                ogreCamera->setNearClipDistance( 0.001f );
                ogreCamera->setFarClipDistance( 10000.0f );

                //ogreCamera->setVisibilityFlags( 0 );

                setLoadingState( LoadingState::Loaded );
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription() );
            }
        }

        void CCameraOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( isLoaded() )
                {
                    setLoadingState( LoadingState::Unloading );

                    //FB_ASSERT( !getOwner() );

                    auto applicationManager = core::IApplicationManager::instance();
                    auto graphicsSystem = applicationManager->getGraphicsSystem();

                    ISharedObject::ScopedLock lock( graphicsSystem );

                    SmartPtr<CGraphicsSceneOgre> smgr = getCreator();
                    if( smgr )
                    {
                        auto ogreSmgr = smgr->getSceneManager();
                        if( ogreSmgr )
                        {
                            ogreSmgr->destroyCamera( m_camera );
                            m_camera = nullptr;
                        }
                    }

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

                    registerForUpdates( false );

                    CGraphicsObjectOgre<ICamera>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription() );
            }
        }

        void CCameraOgre::initialise( Ogre::Camera *camera )
        {
            m_camera = camera;

            if( m_camera )
            {
                m_camera->setAutoAspectRatio( true );
            }
        }

        void CCameraOgre::_update()
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            // Ogre::Matrix4 mat = (m_camera->getProjectionMatrix() *
            // m_camera->getViewMatrix(true)).inverse();

            // inverseVP[0] = mat[0][0];
            // inverseVP[1] = mat[0][1];
            // inverseVP[2] = mat[0][2];
            // inverseVP[3] = mat[0][3];

            // inverseVP[4] = mat[1][0];
            // inverseVP[5] = mat[1][1];
            // inverseVP[6] = mat[1][2];
            // inverseVP[7] = mat[1][3];

            // inverseVP[8] = mat[2][0];
            // inverseVP[9] = mat[2][1];
            // inverseVP[10] = mat[2][2];
            // inverseVP[11] = mat[2][3];

            // inverseVP[12] = mat[3][0];
            // inverseVP[13] = mat[3][1];
            // inverseVP[14] = mat[3][2];
            // inverseVP[15] = mat[3][3];

            // m_realDirection = Vector3F(m_camera->getRealDirection().ptr());
            // m_realPosition = Vector3F(m_camera->getRealPosition().ptr());
            // m_realRight = Vector3F(m_camera->getRealRight().ptr());
            // m_realUp = Vector3F(m_camera->getRealUp().ptr());

            // m_direction = Vector3F(m_camera->getDirection().ptr());
        }

        void CCameraOgre::registerForUpdates( bool registerObject )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            // if(registerObject)
            //{
            //	SmartPtr<CSceneManagerOgre> smgr = m_creator;
            //	if(smgr)
            //		smgr->registerForUpdates(SmartPtr<IGraphicsObject>(this));
            // }
            // else
            //{
            //	SmartPtr<CSceneManagerOgre> smgr = m_creator;
            //	if(smgr)
            //		smgr->unregisteredForUpdates(this);
            // }
        }

        bool CCameraOgre::isRegisteredForUpdates()
        {
            return false;
        }

        void CCameraOgre::setViewport( SmartPtr<IViewport> viewport )
        {
            m_viewport = viewport;
        }

        SmartPtr<IViewport> CCameraOgre::getViewport() const
        {
            return m_viewport.lock();
        }

        void CCameraOgre::setMaterialName( const String &materialName, s32 index )
        {
            m_materialName = materialName;
        }

        String CCameraOgre::getMaterialName( s32 index ) const
        {
            return m_materialName;
        }

        void CCameraOgre::setCastShadows( bool castShadows )
        {
            m_castShadows = castShadows;
        }

        bool CCameraOgre::getCastShadows() const
        {
            return m_castShadows;
        }

        void CCameraOgre::setReceiveShadows( bool receiveShadows )
        {
            m_receiveShadows = receiveShadows;
        }

        bool CCameraOgre::getReceiveShadows() const
        {
            return m_receiveShadows;
        }

        void CCameraOgre::setRenderQueueGroup( u8 renderQueue )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_camera->setRenderQueueGroup( renderQueue );
        }

        void CCameraOgre::setVisibilityFlags( u32 flags )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_camera->setVisibilityFlags( flags );
        }

        u32 CCameraOgre::getVisibilityFlags() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_camera->getVisibilityFlags();
        }

        SmartPtr<IGraphicsObject> CCameraOgre::clone( const String &name ) const
        {
            auto camera = fb::make_ptr<CCameraOgre>();
            return camera;
        }

        void CCameraOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_camera;
        }

        void CCameraOgre::setNearClipDistance( f32 nearDist )
        {
            if( m_camera )
            {
                m_camera->setNearClipDistance( nearDist );
            }
        }

        f32 CCameraOgre::getNearClipDistance() const
        {
            if( m_camera )
            {
                return m_camera->getNearClipDistance();
            }

            return 0.0f;
        }

        void CCameraOgre::setFarClipDistance( f32 farDist )
        {
            if( m_camera )
            {
                m_camera->setFarClipDistance( farDist );
            }
        }

        f32 CCameraOgre::getFarClipDistance() const
        {
            if( m_camera )
            {
                return m_camera->getFarClipDistance();
            }

            return 0.0f;
        }

        void CCameraOgre::setAspectRatio( f32 ratio )
        {
            if( m_camera )
            {
                ISharedObject::ScopedLock lock(
                    core::IApplicationManager::instance()->getGraphicsSystem() );
                m_camera->setAspectRatio( ratio );
                m_aspectRatio = ratio;
            }
        }

        bool CCameraOgre::isVisible( const AABB3F &box ) const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::AxisAlignedBox ogreBox;
            // ogreBox.setMinimum(Ogre::Vector3(box.Min));
            // ogreBox.setMaximum(Ogre::Vector3(box.Max));

            return m_camera->isVisible( ogreBox );
        }

        bool CCameraOgre::isVisible( const Sphere3F &bound ) const
        {
            return false;
        }

        bool CCameraOgre::isVisible( const Vector3F &vert ) const
        {
            return false;
        }

        //
        // ICamera functions
        //

        void CCameraOgre::setPosition( const Vector3F &position )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_position = position;
            // m_camera->setPosition(Ogre::Vector3(mPosition.X(), mPosition.Y(), mPosition.Z()));
        }

        Vector3F CCameraOgre::getPosition() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            Ogre::Vector3 position;  // = m_camera->getPosition();
            m_position = Vector3F( position.ptr() );
            return m_position;
        }

        void CCameraOgre::setDirection( const Vector3F &vec )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->setPosition(Ogre::Vector3(vec));
        }

        Vector3F CCameraOgre::getDirection() const
        {
            return m_direction;
        }

        Vector3F CCameraOgre::getDirection( const Vector2F &screenPosition ) const
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

        Vector3F CCameraOgre::getDirection( const Vector2F &screenPosition,
                                            Vector3F &worldPosition ) const
        {
            auto ray = getRay( screenPosition.X(), screenPosition.Y() );
            worldPosition = ray.getDirection();

            // return getDirection(screenPosition);

            return ray.getDirection();
        }

        void CCameraOgre::setOrientation( const QuaternionF &q )
        {
            m_orientation = q;
        }

        QuaternionF CCameraOgre::getOrientation() const
        {
            auto orientation = m_camera->getRealOrientation();
            m_orientation.W() = orientation.w;
            m_orientation.X() = orientation.x;
            m_orientation.Y() = orientation.y;
            m_orientation.Z() = orientation.z;
            return m_orientation;
        }

        Vector3F CCameraOgre::getUp() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_owner->getOrientation() * Vector3F::unitY();
        }

        Vector3F CCameraOgre::getRight() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_owner->getOrientation() * Vector3F::unitX();
        }

        void CCameraOgre::lookAt( const Vector3F &targetPoint )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->lookAt(Ogre::Vector3(targetPoint));
        }

        void CCameraOgre::roll( const f32 &angle )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->roll(Ogre::Radian(angle));
        }

        void CCameraOgre::yaw( const f32 &angle )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->yaw(Ogre::Radian(angle));
        }

        void CCameraOgre::pitch( const f32 &angle )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->pitch(Ogre::Radian(angle));
        }

        void CCameraOgre::rotate( const Vector3F &axis, const f32 &angle )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->rotate(Ogre::Vector3(axis), Ogre::Radian(angle));
        }

        void CCameraOgre::rotate( const QuaternionF &q )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->rotate(q);
        }

        void CCameraOgre::setFixedYawAxis( bool useFixed, const Vector3F &fixedAxis )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            // m_camera->setFixedYawAxis(useFixed, Ogre::Vector3(fixedAxis));
        }

        QuaternionF CCameraOgre::getDerivedOrientation() const
        {
            auto orientation = m_camera->getRealOrientation();
            m_derivedOrientation.W() = orientation.w;
            m_derivedOrientation.X() = orientation.x;
            m_derivedOrientation.Y() = orientation.y;
            m_derivedOrientation.Z() = orientation.z;
            return m_derivedOrientation;
        }

        Vector3F CCameraOgre::getDerivedPosition() const
        {
            return m_derivedPosition;
        }

        Vector3F CCameraOgre::getDerivedDirection() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return Vector3F( m_camera->getDerivedDirection().ptr() );
        }

        Vector3F CCameraOgre::getDerivedUp() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return Vector3F( m_camera->getDerivedUp().ptr() );
        }

        Vector3F CCameraOgre::getDerivedRight() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return Vector3F( m_camera->getDerivedRight().ptr() );
        }

        QuaternionF CCameraOgre::getRealOrientation() const
        {
            auto orientation = m_camera->getRealOrientation();
            m_realOrientation.W() = orientation.w;
            m_realOrientation.X() = orientation.x;
            m_realOrientation.Y() = orientation.y;
            m_realOrientation.Z() = orientation.z;
            return m_realOrientation;
        }

        Vector3F CCameraOgre::getRealPosition() const
        {
            return m_realPosition;
        }

        Vector3F CCameraOgre::getRealDirection() const
        {
            return m_realDirection;
        }

        Vector3F CCameraOgre::getRealUp() const
        {
            return m_realUp;
        }

        Vector3F CCameraOgre::getRealRight() const
        {
            return m_realRight;
        }

        void CCameraOgre::setLodBias( f32 factor )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_camera->setLodBias( factor );
        }

        f32 CCameraOgre::getLodBias() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_camera->getLodBias();
        }

        Ray3F CCameraOgre::getRay( f32 screenx, f32 screeny ) const
        {
            Ray3F outRay;
            getRay( screenx, screeny, outRay );
            return outRay;
        }

        void CCameraOgre::getRay( f32 screenX, f32 screenY, Ray3F &outRay ) const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::Ray ray = m_camera->getCameraToViewportRay( screenX, screenY );

            const Ogre::Vector3 &origin = ray.getOrigin();
            const Ogre::Vector3 &direction = ray.getDirection();

            outRay.setOrigin( Vector3F( origin.x, origin.y, origin.z ) );
            outRay.setDirection( Vector3F( direction.x, direction.y, direction.z ) );
        }

        Vector2F CCameraOgre::getScreenPosition( const Vector3F &position )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::Vector3 vPosition;  // (position);
            Ogre::Vector4 vTPosition( vPosition.x, vPosition.y, vPosition.z, 1.0f );
            vTPosition = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * vTPosition;

            if( vTPosition[3] < 0 )
                return Vector2F( -10000, -10000 );

            f32 zDiv = vTPosition[3] == 0.0f ? 1.0f : ( 1.0f / vTPosition[3] );

            // float x = 1.0f - (vPosition.x / vPosition.z + 0.5);
            // float y = vPosition.y / vPosition.z + 0.5;

            // float x = (s32)(dim.Width * transformedPos.ptr() * zDiv) + dim.Width;
            // float y = ((s32)(dim.Height - (dim.Height * (transformedPos[1] * zDiv))));
            float x = ( 0.5f * vTPosition[0] * zDiv ) + 0.5f;
            float y = ( 0.5f - ( 0.5f * ( vTPosition[1] * zDiv ) ) );

            return Vector2F( x, y );
        }

        void CCameraOgre::setWindow( f32 left, f32 top, f32 right, f32 bottom )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_camera->setWindow( left, top, right, bottom );
        }

        void CCameraOgre::resetWindow()
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_camera->resetWindow();
        }

        bool CCameraOgre::isWindowSet() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_camera->isWindowSet();
        }

        void CCameraOgre::setAutoAspectRatio( bool autoratio )
        {
            if( m_camera )
            {
                m_camera->setAutoAspectRatio( autoratio );
            }
        }

        bool CCameraOgre::getAutoAspectRatio() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_camera->getAutoAspectRatio();
        }

        f32 CCameraOgre::getAspectRatio() const
        {
            return m_aspectRatio;
        }

        f32 CCameraOgre::getFOVy() const
        {
            return 0.0f;
        }

        void CCameraOgre::setFOVy( f32 fov )
        {
            m_camera->setFOVy( Ogre::Radian( fov ) );
        }

        Matrix4F CCameraOgre::getViewMatrix() const
        {
            return Matrix4F::identity();
        }

        Matrix4F CCameraOgre::getProjectionMatrix() const
        {
            return Matrix4F::identity();
        }

        void *CCameraOgre::getRenderViewMatrix() const
        {
            return nullptr;
        }

        void *CCameraOgre::getRenderProjectionMatrix() const
        {
            return nullptr;
        }

        SmartPtr<ITexture> CCameraOgre::getTargetTexture() const
        {
            return m_targetTexture.lock();
        }

        void CCameraOgre::setTargetTexture( SmartPtr<ITexture> targetTexture )
        {
            m_targetTexture = targetTexture;
        }

        SmartPtr<ITexture> CCameraOgre::getEditorTexture() const
        {
            return m_editorTexture.lock();
        }

        void CCameraOgre::setEditorTexture( SmartPtr<ITexture> editorTexture )
        {
            m_editorTexture = editorTexture;
        }

        void CCameraOgre::setRenderUI( bool enabled )
        {
            m_renderUI = enabled;
        }

        bool CCameraOgre::getRenderUI() const
        {
            return m_renderUI;
        }

        void CCameraOgre::setupStateObject()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            CGraphicsObjectOgre<ICamera>::setupStateObject();

            auto stateObject = getStateObject();
            setStateObject( stateObject );

            auto state = factoryManager->make_ptr<CameraState>();
            setGraphicsObjectState( state );

            stateObject->setState( state );

            auto stateListener = factoryManager->make_ptr<CCameraStateListener>();
            stateListener->setOwner( this );

            // setGraphicsObjectState( stateListener );

            setStateListener( stateListener );
            stateObject->addStateListener( m_stateListener );
        }

        void CCameraOgre::CCameraStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            StateListener::handleStateChanged( state );
        }

        void CCameraOgre::CCameraStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( message->isExactly<StateMessageVisible>() )
            {
                auto visibleMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                FB_ASSERT( visibleMessage );

                auto visible = visibleMessage->isVisible();
                m_owner->setVisible( visible );
            }

            StateListener::handleStateChanged( message );
        }

        void CCameraOgre::CCameraStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        void CCameraOgre::CCameraStateListener::setOwner( CCameraOgre *owner )
        {
            m_owner = owner;
        }

        CCameraOgre *CCameraOgre::CCameraStateListener::getOwner() const
        {
            return m_owner;
        }
    }  // end namespace render
}  // end namespace fb
