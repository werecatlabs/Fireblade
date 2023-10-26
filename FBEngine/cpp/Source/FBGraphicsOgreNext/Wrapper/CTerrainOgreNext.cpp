#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CTerrainOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/CompositorManager.h>
#include <FBCore/FBCore.h>
#include <Wrapper/CGraphicsSystemOgreNext.h>
#include <Utils/MeshUtils.h>
#include <Terra/Terra.h>
#include <Terra/TerraShadowMapper.h>
#include <Terra/Hlms/PbsListener/OgreHlmsPbsTerraShadows.h>
#include <Terra/Hlms/OgreHlmsTerraDatablock.h>
#include <Terra/Hlms/OgreHlmsTerra.h>
#include <Ogre.h>
#include <OgreTextureGpuManager.h>

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb, CTerrainOgreNext, ITerrain );

        CTerrainOgreNext::CTerrainOgreNext()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();
            FB_ASSERT( stateObject );
            setStateObject( stateObject );

            auto stateListener = factoryManager->make_ptr<StateListener>();
            stateListener->setOwner( this );
            setStateListener( stateListener );
            stateObject->addStateListener( stateListener );
        }

        CTerrainOgreNext::~CTerrainOgreNext()
        {
            unload( nullptr );
        }

        void CTerrainOgreNext::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                using namespace Ogre;

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto timer = applicationManager->getTimer();

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto pGraphicsSystem =
                    fb::static_pointer_cast<CGraphicsSystemOgreNext>( graphicsSystem );
                FB_ASSERT( pGraphicsSystem );

                auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
                FB_ASSERT( resourceGroupManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                m_nextUpdateTime = timer->getTime() + 3.0;

                auto root = Ogre::Root::getSingletonPtr();

                auto hlmsManager = root->getHlmsManager();
                auto hlmsCompute = hlmsManager->getComputeHlms();

                // auto computeJobName = "Terra/ShadowGenerator";
                // hlmsCompute->createComputeJob (computeJobName, computeJobName,
                // "TerraShadowGenerator.material.json", Ogre::StringVector());

                // auto gaussianBlurHComputeJobName = "Terra/GaussianBlurH";
                // hlmsCompute->createComputeJob(gaussianBlurHComputeJobName,
                // gaussianBlurHComputeJobName, "TerraShadowGenerator.material.json",
                // Ogre::StringVector());

                // auto gaussianBlurVComputeJobName = "Terra/GaussianBlurV";
                // hlmsCompute->createComputeJob(gaussianBlurVComputeJobName,
                // gaussianBlurVComputeJobName, "TerraShadowGenerator.material.json",
                // Ogre::StringVector());

                // auto terraComputeJobName = "TerraExampleMaterial";
                // hlmsCompute->createComputeJob(terraComputeJobName, terraComputeJobName,
                // "Terra.material.json", Ogre::StringVector());

                auto pSceneManager = fb::static_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );

                auto sceneManager = pSceneManager->getSceneManager();

                auto pCamera = fb::static_pointer_cast<CCameraOgreNext>( pSceneManager->getCamera() );
                auto camera = pCamera->getCamera();

                createTerraInstance();

                // mTerra->load( "Heightmap.png", Ogre::Vector3::ZERO, Ogre::Vector3( 256.0f, 1.0f,
                // 256.0f ) ); mTerra->load( "Heightmap.png", Ogre::Vector3( 64.0f, 0, 64.0f ),
                // Ogre::Vector3( 128.0f, 5.0f, 128.0f ) ); mTerra->load( "Heightmap.png",
                // Ogre::Vector3( 64.0f, 0, 64.0f ), Ogre::Vector3( 1024.0f, 5.0f, 1024.0f ) );
                // mTerra->load( "Heightmap.png", Ogre::Vector3( 64.0f, 0, 64.0f ), Ogre::Vector3(
                // 4096.0f * 4, 15.0f * 64.0f*4, 4096.0f * 4 ) );

                // heightmap = "Heightmap.png";

                m_center = Ogre::Vector3( 0.0f, m_dimensions.y * 0.5f, 0.0f );

                auto textureWidth = 256;
                auto textureHeight = 256;

                Ogre::Image2 image;
                image.createEmptyImage( textureWidth, textureHeight, 1,
                                        Ogre::TextureTypes::TextureTypes::Type2D,
                                        Ogre::PixelFormatGpu::PFG_R8_UNORM, 1u );

                for( size_t x = 0; x < textureWidth; ++x )
                {
                    for( size_t y = 0; y < textureWidth; ++y )
                    {
                        image.setColourAt( Ogre::ColourValue::Black, x, y, 0, 0 );
                    }
                }

                m_terra->load( image, m_center, m_dimensions, false, false );

                // m_terra->load( heightmap, m_center, m_dimensions, false, false );
                //m_terra->load( "Heightmap.png", Ogre::Vector3( 64.0f, 4096.0f * 0.5f, 64.0f ),
                //               Ogre::Vector3( 14096.0f, 14096.0f, 14096.0f ), false, false );

                auto rootNode = sceneManager->getRootSceneNode( Ogre::SCENE_STATIC );
                auto sceneNode = rootNode->createChildSceneNode( Ogre::SCENE_STATIC );
                sceneNode->attachObject( m_terra );
                setSceneNode(sceneNode);

                setupMaterial();

                pGraphicsSystem->setTerra( m_terra );

                // auto compositorManager = pGraphicsSystem->getCompositorManager();
                // if( compositorManager )
                //{
                //     compositorManager->reload( 0 );
                // }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                auto message = String( e.what() );
                FB_LOG_ERROR( message );
            }
        }

        void CTerrainOgreNext::createTerraInstance()
        {
            auto root = Ogre::Root::getSingletonPtr();

            auto pSceneManager = fb::static_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );
            auto sceneManager = pSceneManager->getSceneManager();

            auto pCamera = fb::static_pointer_cast<CCameraOgreNext>( pSceneManager->getCamera() );
            auto camera = pCamera->getCamera();

            auto compositorManager = root->getCompositorManager2();

            auto entityMemoryManager = &sceneManager->_getEntityMemoryManager( Ogre::SCENE_STATIC );

            auto renderQueueId = (u8)IGraphicsObject::RenderQueueGroupID::RENDER_QUEUE_WORLD_GEOMETRY_2;
            renderQueueId = 11u;

            auto id = Ogre::Id::generateNewId<Ogre::MovableObject>();

            // Render terrain after most objects, to improve performance by taking advantage of early Z
            m_terra = new Ogre::Terra( id, entityMemoryManager, sceneManager, renderQueueId,
                                       compositorManager, camera, false );
            m_terra->setCastShadows( false );
        }

        void CTerrainOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                if ( m_terra )
				{
					delete m_terra;
					m_terra = nullptr;
				}

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTerrainOgreNext::postUpdate()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto timer = applicationManager->getTimer();

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto renderTask = graphicsSystem->getRenderTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();

                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( m_nextUpdateTime < timer->getTime() )
                    {
                        auto window = graphicsSystem->getDefaultWindow();
                        if( window )
                        {
                            if( window->isVisible() )
                            {
                                if( m_terra )
                                {
                                    auto pSceneManager =
                                        fb::static_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );
                                    auto sceneManager = pSceneManager->getSceneManager();

                                    auto pCamera = pSceneManager->getActiveCamera();
                                    if( !pCamera )
                                    {
                                        pCamera = pSceneManager->getCamera();
                                    }

                                    auto pCameraOgreNext =
                                        fb::static_pointer_cast<CCameraOgreNext>( pCamera );
                                    auto camera = pCameraOgreNext->getCamera();

                                    m_terra->setCamera( camera );
                                    m_terra->update( m_lightDirection, m_lightEpsilon );
                                    m_terra->setCamera( nullptr );
                                }
                            }
                        }

                        m_nextUpdateTime = timer->getTime() + ( 1.0 / 30.0 );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Vector3F CTerrainOgreNext::getPosition() const
        {
            return Vector3F::zero();
        }

        void CTerrainOgreNext::setPosition( const Vector3F &position )
        {
        }

        f32 CTerrainOgreNext::getHeightAtWorldPosition( const Vector3F &position ) const
        {
            return 0.0f;
        }

        u16 CTerrainOgreNext::getSize() const
        {
            return 0;
        }

        Vector3F CTerrainOgreNext::getTerrainSpacePosition( const Vector3F &worldSpace ) const
        {
            return Vector3F::zero();
        }

        void CTerrainOgreNext::setTextureLayer( s32 layer, const String &textureName )
        {
        }

        Ogre::SceneNode *CTerrainOgreNext::getSceneNode() const
        {
            return m_sceneNode;
        }

        void CTerrainOgreNext::setSceneNode( Ogre::SceneNode *sceneNode )
        {
            m_sceneNode = sceneNode;
        }

        Array<f32> CTerrainOgreNext::getHeightData() const
        {
            return Array<f32>();
        }

        bool CTerrainOgreNext::isVisible() const
        {
            return m_isVisible;
        }

        void CTerrainOgreNext::setVisible( bool visible )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();

                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    m_isVisible = visible;
                    m_terra->setVisible( visible );
                }
                else
                {
                    auto stateMessageVisible = factoryManager->make_ptr<StateMessageVisible>();
                    stateMessageVisible->setVisible( visible );
                    // stateMessageVisible->setCascade( cascade );

                    if( auto stateContext = getStateObject() )
                    {
                        stateContext->addMessage( stateTask, stateMessageVisible );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        String CTerrainOgreNext::getMaterialName() const
        {
            return m_materialName;
        }

        void CTerrainOgreNext::setMaterialName( const String &materialName )
        {
            m_materialName = materialName;
        }

        SmartPtr<ITerrainBlendMap> CTerrainOgreNext::getBlendMap( u32 index )
        {
            return nullptr;
        }

        u16 CTerrainOgreNext::getLayerBlendMapSize() const
        {
            return 0;
        }

        SmartPtr<ITerrainRayResult> CTerrainOgreNext::intersects( const Ray3F &ray ) const
        {
            return nullptr;
        }

        SmartPtr<IMesh> CTerrainOgreNext::getMesh() const
        {
            return nullptr;
        }

        f32 CTerrainOgreNext::getHeightScale() const
        {
            return 0.0f;
        }

        void CTerrainOgreNext::setHeightScale( f32 val )
        {
        }

        void CTerrainOgreNext::_getObject( void **ppObject ) const
        {
            *ppObject = m_terra;
        }

        SmartPtr<render::IGraphicsScene> CTerrainOgreNext::getSceneManager() const
        {
            return m_sceneManager;
        }

        void CTerrainOgreNext::setSceneManager( SmartPtr<render::IGraphicsScene> sceneManger )
        {
            m_sceneManager = sceneManger;
        }

        void CTerrainOgreNext::setupMaterial()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto materialManager = graphicsSystem->getMaterialManager();

            auto materialName = getMaterialName();
            if( !StringUtil::isNullOrEmpty( materialName ) )
            {
                auto materialResource = materialManager->load( materialName );
                if( materialResource )
                {
                    auto material = fb::static_pointer_cast<IMaterial>( materialResource );
                    if( material )
                    {
                        auto techniques = material->getTechniques();
                        if( !techniques.empty() )
                        {
                            auto technique = techniques.front();
                            if( technique )
                            {
                                auto passes = technique->getPasses();
                                if( !passes.empty() )
                                {
                                    auto pass = passes.front();
                                    if( pass )
                                    {
                                        auto textureUnits = pass->getTextureUnits();
                                        for( auto textureUnit : textureUnits )
                                        {
                                            // textureUnit->ty
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                setupMaterial2();  // default
            }
        }

        void CTerrainOgreNext::setupMaterial2()
        {
#if 0
            auto root = Ogre::Root::getSingletonPtr();

            auto hlmsManager = root->getHlmsManager();
            auto hlmsCompute = hlmsManager->getComputeHlms();

            //wireframe
            auto hlms = hlmsManager->getHlms( Ogre::HLMS_USER3 );
            auto datablock = hlms->getDefaultDatablock();
            Ogre::HlmsMacroblock macroblock;
            macroblock.mPolygonMode = Ogre::PM_WIREFRAME;
            datablock->setMacroblock( macroblock );
#else
            auto root = Ogre::Root::getSingletonPtr();

            auto hlmsManager = root->getHlmsManager();
            auto hlmsCompute = hlmsManager->getComputeHlms();

            auto pDatablock = hlmsManager->getDatablock( "TerraExampleMaterial" );
            auto datablock = (Ogre::HlmsTerraDatablock *)pDatablock;

            auto texUnit = 0;
            auto creator = datablock->getCreator();

            auto renderSystem = creator->getRenderSystem();
            auto textureManager = renderSystem->getTextureGpuManager();

            auto diffuseTexture = textureManager->createOrRetrieveTexture(
                "Albedo_4K__vl1jbbtlw.png", Ogre::GpuPageOutStrategy::Discard,
                Ogre::TextureFlags::AutomaticBatching, Ogre::TextureTypes::Type2D,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            auto diffuseTextureNM = textureManager->createOrRetrieveTexture(
                "Normal_4K__vl1jbbtlw.png", Ogre::GpuPageOutStrategy::Discard,
                Ogre::TextureFlags::AutomaticBatching, Ogre::TextureTypes::Type2D,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            auto splatTexture = textureManager->createOrRetrieveTexture(
                "heightfield_0_splatmap_0.png", Ogre::GpuPageOutStrategy::Discard,
                Ogre::TextureFlags::AutomaticBatching, Ogre::TextureTypes::Type2D,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            auto detailTexture0 = textureManager->createOrRetrieveTexture(
                "Albedo_4K__vmclacwlw.png", Ogre::GpuPageOutStrategy::Discard,
                Ogre::TextureFlags::AutomaticBatching, Ogre::TextureTypes::Type2D,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            auto detailTextureNm0 = textureManager->createOrRetrieveTexture(
                "Normal_4K__vmclacwlw.png", Ogre::GpuPageOutStrategy::Discard,
                Ogre::TextureFlags::AutomaticBatching, Ogre::TextureTypes::Type2D,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            auto detailTexture1 = textureManager->createOrRetrieveTexture(
                "Albedo_4K__vckmabus.png", Ogre::GpuPageOutStrategy::Discard,
                Ogre::TextureFlags::AutomaticBatching, Ogre::TextureTypes::Type2D,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            auto detailTextureNm1 = textureManager->createOrRetrieveTexture(
                "Normal_4K__vckmabus.png", Ogre::GpuPageOutStrategy::Discard,
                Ogre::TextureFlags::AutomaticBatching, Ogre::TextureTypes::Type2D,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );

            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DIFFUSE, diffuseTexture );
            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DETAIL_WEIGHT, splatTexture );
            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DETAIL0, diffuseTexture );
            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DETAIL1, detailTexture0 );
            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DETAIL2, detailTexture1 );

            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DETAIL0_NM, diffuseTextureNM );
            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DETAIL1_NM, detailTextureNm0 );
            datablock->setTexture( Ogre::TerraTextureTypes::TERRA_DETAIL2_NM, detailTextureNm1 );
#endif

            datablock->setMetalness( Ogre::TerraTextureTypes::TERRA_DIFFUSE, 0.5f );
            datablock->setMetalness( Ogre::TerraTextureTypes::TERRA_DETAIL_WEIGHT, 0.5f );
            datablock->setMetalness( Ogre::TerraTextureTypes::TERRA_DETAIL0, 0.5f );
            datablock->setMetalness( Ogre::TerraTextureTypes::TERRA_DETAIL1, 0.5f );
            datablock->setMetalness( Ogre::TerraTextureTypes::TERRA_DETAIL2, 0.5f );

            m_terra->setDatablock( datablock );

            m_hlmsPbsTerraShadows = new Ogre::HlmsPbsTerraShadows();
            m_hlmsPbsTerraShadows->setTerra( m_terra );

            // Set the PBS listener so regular objects also receive terrain shadows
            auto hlmsPbs = hlmsManager->getHlms( Ogre::HLMS_PBS );
            hlmsPbs->setListener( m_hlmsPbsTerraShadows );
        }

        void CTerrainOgreNext::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void CTerrainOgreNext::StateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( message->isExactly<StateMessageVisible>() )
            {
                auto visibleMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                FB_ASSERT( visibleMessage );

                auto visible = visibleMessage->isVisible();
                auto cascade = visibleMessage->getCascade();

                m_owner->setVisible( visible );
            }
        }

        void CTerrainOgreNext::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CTerrainOgreNext *CTerrainOgreNext::StateListener::getOwner() const
        {
            return m_owner;
        }

        void CTerrainOgreNext::StateListener::setOwner( CTerrainOgreNext *owner )
        {
            m_owner = owner;
        }

    }  // end namespace render
}  // end namespace fb
