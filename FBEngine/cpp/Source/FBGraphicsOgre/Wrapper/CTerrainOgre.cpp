#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CTerrainOgre.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSystemOgre.h>
#include <FBGraphicsOgre/Wrapper/CTerrainRayResult.h>
#include <FBCore/FBCore.h>
#include <FBMesh/FBMesh.h>
#include <Ogre.h>
#include <OgreSharedPtr.h>
#include <OgreShadowCameraSetup.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainMaterialGeneratorA.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainMaterialGenerator.h>
#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrainQuadTreeNode.h>

// Include PagedGeometry headers that will be needed
#include "FBGraphicsOgre/PagedGeometry/PagedGeometry.h"
#include "FBGraphicsOgre/PagedGeometry/BatchPage.h"
#include "FBGraphicsOgre/PagedGeometry/ImpostorPage.h"
#include "FBGraphicsOgre/PagedGeometry/TreeLoader3D.h"

// Include "LegacyTerrainLoader.h", a header that allows loading Ogre 1.7 style terrain
#include "FBGraphicsOgre/PagedGeometry/LegacyTerrainLoader.h"

// Include "HeightFunction.h", a header that provides some useful functions for quickly and easily
// getting the height of the terrain at a given point.
#include "OgreUtil.h"
#include "FBGraphicsOgre/PagedGeometry/HeightFunction.h"
#include "Terrain/TerrainComponent/OgreTerrainAutoUpdateLod.h"

#define TERRAIN_FILE_PREFIX Ogre::String( "testTerrain" )
#define TERRAIN_FILE_SUFFIX Ogre::String( "dat" )
#define TERRAIN_WORLD_SIZE 1000.0f
#define TERRAIN_SIZE 257

#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0

// PagedGeometry's classes and functions are under the "Forests" namespace
using namespace Forests;

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CTerrainOgre, SharedObject<ITerrain> );
        u32 CTerrainOgre::m_ext = 0;

        CTerrainOgre::CTerrainOgre() :
            mTerrainGroup( nullptr ),
            mTerrainPaging( nullptr ),
            mPageManager( nullptr ),

            mTerrainPos( 0, 0, 0 )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto stateManager = applicationManager->getStateManager();
            auto factoryManager = applicationManager->getFactoryManager();

            auto stateContext = stateManager->addStateObject();
            setStateObject( stateContext );

            auto state = factoryManager->make_ptr<TerrainState>();
            stateContext->setState( state );

            auto stateListener = factoryManager->make_ptr<TerrainStateListener>();
            stateListener->setOwner( this );
            stateContext->addStateListener( stateListener );
            setStateListener( stateListener );
        }

        CTerrainOgre::~CTerrainOgre()
        {
            unload( nullptr );
        }

        void CTerrainOgre::update()
        {
            try
            {
                switch( auto task = Thread::getCurrentTask() )
                {
                case Thread::Task::Render:
                {
                    if( auto terrainGroup = getTerrainGroup() )
                    {
                        terrainGroup->update( true );

                        auto terrain = terrainGroup->getTerrain( 0, 0 );
                        if( terrain )
                        {
                            auto node = terrain->_getRootSceneNode();
                            if( node )
                            {
                                node->_updateBounds();
                            }
                        }
                    }

                    if( auto trees = getTrees() )
                    {
                        setTreeMask( trees->getSceneNode() );

                        auto applicationManager = core::IApplicationManager::instance();
                        auto graphicsSystem = applicationManager->getGraphicsSystem();
                        auto window = graphicsSystem->getDefaultWindow();
                        auto viewports = window->getViewports();
                        for( auto viewport : viewports )
                        {
                            if( viewport )
                            {
                                auto camera = viewport->getCamera();
                                if( camera )
                                {
                                    Ogre::Camera *ogreCamera = nullptr;
                                    camera->_getObject( (void **)&ogreCamera );

                                    auto ogreSceneManager = getOgreSceneManager();

                                    if( ogreCamera->getSceneManager() == ogreSceneManager )
                                    {
                                        trees->setCamera( ogreCamera );
                                    }
                                }
                            }
                        }

                        trees->update();
                    }
                }
                break;
                default:
                {
                }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTerrainOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto factoryManager = applicationManager->getFactoryManager();

                auto renderTask = graphicsSystem->getRenderTask();
                auto task = Thread::getCurrentTask();

                if( task == renderTask )
                {
                    setLoadingState( LoadingState::Loading );

                    auto ogreSceneManager = getOgreSceneManager();

                    auto cameraName = String( "TerrainCamera_" ) + StringUtil::toString( m_ext++ );
                    auto terrainCamera = ogreSceneManager->createCamera( cameraName );
                    auto terrainCameraSceneNode =
                        ogreSceneManager->getRootSceneNode()->createChildSceneNode();
                    terrainCameraSceneNode->attachObject( terrainCamera );

                    setTerrainCamera( terrainCamera );
                    setTerrainCameraSceneNode( terrainCameraSceneNode );

                    initialise2();
                    loadTrees();

                    auto message = fb::make_ptr<StateMessageUIntValue>();
                    message->setType( StringUtil::getHash( "loadComplete" ) );

                    if( auto stateContext = getStateObject() )
                    {
                        stateContext->addMessage( renderTask, message );
                    }

                    setLoadingState( LoadingState::Loaded );
                }
                else
                {
                    // SmartPtr<StateMessageLoad> message( new StateMessageLoad );
                    // m_stateObject->addMessage( renderTask, message );

                    graphicsSystem->loadObject( this );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTerrainOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    // m_terrainTemplate.setNull();
                    // m_stateObject.setNull();
                    // m_stateListener.setNull();

                    m_blendLayers.clear();
                    FB_SAFE_DELETE( mTerrainGroup );

                    if( m_trees )
                    {
                        delete m_trees;
                        m_trees = nullptr;
                    }

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto stateManager = applicationManager->getStateManager();

                    if( auto stateObject = getStateObject() )
                    {
                        if( auto stateListener = getStateListener() )
                        {
                            stateObject->removeStateListener( stateListener );

                            stateListener->unload( nullptr );
                            setStateListener( nullptr );
                        }

                        if( stateManager )
                        {
                            stateManager->removeStateObject( stateObject );
                        }

                        setStateObject( nullptr );
                    }

                    m_sceneManager = nullptr;

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        //
        // void CTerrain::initialise( SmartPtr<scene::IDirector> objectTemplate )
        //{
        //     auto applicationManager = core::IApplicationManager::instance();
        //     auto graphicsSystem = applicationManager->getGraphicsSystem();
        //     auto factoryManager = applicationManager->getFactoryManager();
        //     auto renderTask = graphicsSystem->getRenderTask();
        //     auto task = Thread::getCurrentTask();

        //    const auto &loadingState = getLoadingState();
        //    if( loadingState == LoadingState::Loaded && task == renderTask )
        //    {
        //        initialise();
        //        loadTrees();

        //        m_isLoaded = true;

        //        SmartPtr<StateMessageUIntValue> message( new StateMessageUIntValue );
        //        message->setType( StringUtil::getHash( "loadComplete" ) );
        //        m_stateObject->addMessage( renderTask, message );
        //    }
        //    else
        //    {
        //        SmartPtr<StateMessageLoad> message( new StateMessageLoad );
        //        m_stateObject->addMessage( Thread::Task::Render, message );
        //    }
        //}

        void CTerrainOgre::initialise()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                bool deferredEnabled = false;  // = graphicsSystem->isDeferredShadingSystemEnabled();

                bool blankTerrain = false;
                blankTerrain = true;  // initial terrain

                // mSceneMgr->setFog(Ogre::FOG_EXP, Ogre::ColourValue::Red);

                Ogre::LogManager::getSingleton().setLogDetail( Ogre::LL_BOREME );

                Ogre::Vector3 lightdir( 0.55, -0.3, 0.75 );
                lightdir.normalise();

                // Ogre::Light* l = mSceneMgr->createLight("Light0");
                // l->setType(Ogre::Light::LT_DIRECTIONAL);
                // l->setDirection(lightdir);
                // l->setDiffuseColour(Ogre::ColourValue::White);
                // l->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

                //mSceneMgr->setAmbientLight( Ogre::ColourValue( 0.2, 0.2, 0.2 ) );

                auto terrainGlobals = Ogre::TerrainGlobalOptions::getSingletonPtr();

                // mTerrainGlobals->setCompositeMapDistance(100.0f);

                if( !deferredEnabled )
                {
                    auto matProfile = static_cast<Ogre::TerrainMaterialGeneratorA::SM2Profile *>(
                        terrainGlobals->getDefaultMaterialGenerator()->getActiveProfile() );
                    matProfile->setCompositeMapEnabled( false );
                    matProfile->setGlobalColourMapEnabled( true );
                }
                else
                {
                    /*
                    Ogre::TerrainMaterialGeneratorDeferredPtr terrainMaterialGenerator(
                        new Ogre::TerrainMaterialGeneratorDeferred );
                    mTerrainGlobals->setDefaultMaterialGenerator( terrainMaterialGenerator );
                    Ogre::TerrainGlobalOptions::getSingleton().setCastsDynamicShadows( true );
                    auto matProfile = static_cast<Ogre::TerrainMaterialGeneratorDeferred::SM2Profile *>(
                        terrainMaterialGenerator->getActiveProfile() );
                    matProfile->setCompositeMapEnabled( false );
                    matProfile->setLightmapEnabled( false );
                    matProfile->setGlobalColourMapEnabled( false );
                     */
                }

                // mTerrainGlobals->setVisibilityFlags(render::IGraphicsSceneManager::VIEWPORT_MASK_TERRAIN);

                //mTerrainGroup = OGRE_NEW Ogre::TerrainGroup( mSceneMgr );

                //mTerrainGroup->loadLegacyTerrain("terrain.cfg");

                auto ogreSceneManager = getOgreSceneManager();
                mTerrainGroup = loadLegacyTerrain( "terrain.cfg", ogreSceneManager );
                // mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
                mTerrainGroup->setAutoUpdateLod(
                    Ogre::TerrainAutoUpdateLodFactory::getAutoUpdateLod( Ogre::BY_DISTANCE ) );

                // Vector3F position = Vector3F::zero();
                // if (m_terrainTemplate)
                //{
                //	position = m_terrainTemplate->getPosition();
                // }

                // mTerrainPos = Ogre::Vector3(position.X(), position.Y(), position.Z());
                // mTerrainGroup->setOrigin(mTerrainPos);

                // configureTerrainDefaults(l);

                // for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
                //	for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
                //		defineTerrain(x, y, blankTerrain);

                // sync load since we want everything in place when we start
                mTerrainGroup->loadAllTerrains( true );

                ////if (mTerrainsImported)
                //{
                // Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
                // while (ti.hasMoreElements())
                //{
                //	Ogre::Terrain* t = ti.getNext()->instance;
                //	initBlendMaps(t);
                //}
                //}

                mTerrainGroup->updateGeometry();
                mTerrainGroup->updateDerivedData( true );
                mTerrainGroup->update( true );

                mTerrainGroup->freeTemporaryResources();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTerrainOgre::initialise2()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                bool deferredEnabled = false;  //graphicsSystem->isDeferred();

                bool blankTerrain = false;
                blankTerrain = true;  // initial terrain

                auto ogreSceneManager = getOgreSceneManager();

                // mSceneMgr->setFog(Ogre::FOG_EXP, Ogre::ColourValue::Red);

                Ogre::LogManager::getSingleton().setLogDetail( Ogre::LL_BOREME );

                Ogre::Vector3 lightdir( 0.55, -0.3, 0.75 );
                lightdir.normalise();

                auto lightName = String( "Light0" ) + StringUtil::toString( m_ext++ );
                Ogre::Light *l = ogreSceneManager->createLight( lightName );
                l->setType( Ogre::Light::LT_DIRECTIONAL );
                l->setDirection( lightdir );
                l->setDiffuseColour( Ogre::ColourValue::White );
                l->setSpecularColour( Ogre::ColourValue( 0.4, 0.4, 0.4 ) );

                //mSceneMgr->setAmbientLight( Ogre::ColourValue( 0.2, 0.2, 0.2 ) );

                auto terrainGlobals = Ogre::TerrainGlobalOptions::getSingletonPtr();

                // mTerrainGlobals->setCompositeMapDistance(100.0f);

                if( !deferredEnabled )
                {
                    auto generator = terrainGlobals->getDefaultMaterialGenerator();
                    auto activeProfile = generator->getActiveProfile();

                    auto matProfile =
                        static_cast<Ogre::TerrainMaterialGeneratorA::SM2Profile *>( activeProfile );
                    matProfile->setLayerNormalMappingEnabled( false );
                    matProfile->setCompositeMapEnabled( false );
                    matProfile->setGlobalColourMapEnabled( true );
                }
                else
                {
                    /*
                    Ogre::TerrainMaterialGeneratorDeferredPtr terrainMaterialGenerator(
                        new Ogre::TerrainMaterialGeneratorDeferred );
                    mTerrainGlobals->setDefaultMaterialGenerator( terrainMaterialGenerator );
                    Ogre::TerrainGlobalOptions::getSingleton().setCastsDynamicShadows( true );
                    auto matProfile = static_cast<Ogre::TerrainMaterialGeneratorDeferred::SM2Profile *>(
                        terrainMaterialGenerator->getActiveProfile() );
                    matProfile->setCompositeMapEnabled( false );
                    matProfile->setLightmapEnabled( false );
                    matProfile->setGlobalColourMapEnabled( false );
                     */
                }

                //terrainGlobals->setVisibilityFlags(std::numeric_limits<u32>::max());

                //mTerrainGroup = OGRE_NEW Ogre::TerrainGroup( m_ogreSceneManager );

                //mTerrainGroup->loadLegacyTerrain("terrain.cfg");

                mTerrainGroup = OGRE_NEW Ogre::TerrainGroup( ogreSceneManager,
                                                             Ogre::Terrain::ALIGN_X_Z, 129, 512.0f );

                //mTerrainGroup = loadLegacyTerrain( "terrain.cfg", ogreSceneManager );
                // mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
                mTerrainGroup->setAutoUpdateLod(
                    Ogre::TerrainAutoUpdateLodFactory::getAutoUpdateLod( Ogre::BY_DISTANCE ) );

                mTerrainGroup->setOrigin( Ogre::Vector3::ZERO );
                mTerrainGroup->setResourceGroup(
                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                // Vector3F position = Vector3F::zero();
                // if (m_terrainTemplate)
                //{
                //	position = m_terrainTemplate->getPosition();
                // }

                // mTerrainPos = Ogre::Vector3(position.X(), position.Y(), position.Z());
                // mTerrainGroup->setOrigin(mTerrainPos);

                configureTerrainDefaults( l );

                // for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
                //	for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
                defineTerrain( 0, 0, blankTerrain );

                // sync load since we want everything in place when we start
                mTerrainGroup->loadAllTerrains( true );

                ////if (mTerrainsImported)
                //{
                // Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
                // while (ti.hasMoreElements())
                //{
                //	Ogre::Terrain* t = ti.getNext()->instance;
                //	initBlendMaps(t);
                //}
                //}

                mTerrainGroup->updateGeometry();
                mTerrainGroup->updateDerivedData( true );
                mTerrainGroup->update( true );

                mTerrainGroup->freeTemporaryResources();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Transform3<real_Num> CTerrainOgre::getWorldTransform() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<TerrainState>() )
                {
                    return state->getWorldTransform();
                }
            }

            return Transform3<real_Num>();
        }

        void CTerrainOgre::setWorldTransform( const Transform3<real_Num> &worldTransform )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<TerrainState>() )
                {
                    state->setWorldTransform( worldTransform );
                }
            }
        }

        Vector3F CTerrainOgre::getPosition() const
        {
            return Vector3F( mTerrainPos.x, mTerrainPos.y, mTerrainPos.z );
        }

        void CTerrainOgre::setPosition( const Vector3F &position )
        {
            if( isLoaded() != true )
            {
                return;
            }

            //auto pos = Ogre::Vector3( position.X(), position.Y(), position.Z() );
            //if( mTerrainPos != pos )
            //{
            //    // todo change terrain and tree positions
            //    mTerrainGroup->setOrigin( mTerrainPos );
            //    mTerrainGroup->update();
            //    mTerrainPos = pos;

            //    auto node = m_trees->getSceneNode();
            //    node->setPosition( pos );
            //}
        }

        f32 CTerrainOgre::getHeightAtWorldPosition( const Vector3F &position ) const
        {
            if( mTerrainGroup )
            {
                auto pos = OgreUtil::convertToOgreVector( position );
                return mTerrainGroup->getHeightAtWorldPosition( pos );
            }

            return 0.0f;
        }

        u16 CTerrainOgre::getSize() const
        {
            Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                return t->getSize();
            }

            return 0;
        }

        void CTerrainOgre::configureTerrainDefaults( Ogre::Light *l )
        {
            // Configure global
            //mTerrainGlobals->setMaxPixelError( 1 );
            //// testing composite map
            // mTerrainGlobals->setCompositeMapDistance(3000);
            ////mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
            ////mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
            ////mTerrainGlobals->setLightMapSize(256);
            //
            //// Important to set these so that the terrain knows what to use for derived (non-realtime)
            /// data
            // mTerrainGlobals->setLightMapDirection(l->getDerivedDirection());
            ////mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
            // mTerrainGlobals->setCompositeMapAmbient(Ogre::ColourValue::Red);
            // mTerrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());

            // mTerrainGlobals->setLayerBlendMapSize(4096);

            // Configure default import settings for if we use imported image
            Ogre::Terrain::ImportData &defaultimp = mTerrainGroup->getDefaultImportSettings();
            defaultimp = Ogre::Terrain::ImportData();

            defaultimp.terrainSize = 129;

            //defaultimp.terrainSize = TERRAIN_SIZE;
            //defaultimp.worldSize = TERRAIN_WORLD_SIZE;

            /*
            if( m_terrainTemplate )
            {
                defaultimp.inputScale = m_terrainTemplate->getHeightScale();
            }
            else
            {
                defaultimp.inputScale = 600;
            }
             */

            //defaultimp.minBatchSize = 33;
            //defaultimp.maxBatchSize = 65;

            // textures
            /*
            if( m_terrainTemplate )
            {
                Array<SmartPtr<TerrainLayerTemplate>> textureLayers =
                    m_terrainTemplate->getTerrainLayers();
                defaultimp.layerList.resize( textureLayers.size() );

                for( u32 i = 0; i < textureLayers.size(); ++i )
                {
                    SmartPtr<TerrainLayerTemplate> textureLayer = textureLayers[i];

                    defaultimp.layerList[i].worldSize = textureLayer->getWorldSize();
                    defaultimp.layerList[i].textureNames.push_back(
                        textureLayer->getDiffuseTexture().c_str() );
                    defaultimp.layerList[i].textureNames.push_back(
                        textureLayer->getNormalTexture().c_str() );
                }
            }
            else
            {
                // defaultimp.layerList.resize(3);
                // defaultimp.layerList[0].worldSize = 10;
                // defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
                // defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
                // defaultimp.layerList[1].worldSize = 10;
                // defaultimp.layerList[1].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
                // defaultimp.layerList[1].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
                // defaultimp.layerList[2].worldSize = 20;
                // defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
                // defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
            }
             */
        }

        Vector3F CTerrainOgre::getTerrainSpacePosition( const Vector3F &worldSpace ) const
        {
            Vector3F terrainSpaceVec;

            Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;

                Ogre::Vector3 outTerrainSpaceVec = Ogre::Vector3::ZERO;
                // t->getTerrainPosition(Ogre::Vector3(worldSpace.X(), worldSpace.Y(), worldSpace.Z()),
                // &outTerrainSpaceVec);
                return Vector3F( outTerrainSpaceVec.x, outTerrainSpaceVec.y, outTerrainSpaceVec.z );
            }

            return Vector3F::zero();
        }

        void CTerrainOgre::defineTerrain( long x, long y, bool flat )
        {
            // if a file is available, use it
            // if not, generate file from import

            // Usually in a real project you'll know whether the compact terrain data is
            // available or not; I'm doing it this way to save distribution size

            if( flat )
            {
                Ogre::Terrain::ImportData imp;

                //imp.pos = Ogre::Vector3::ZERO;
                //imp.inputImage = 0;
                //imp.inputFloat = 0;
                //imp.constantHeight = 0.0f;
                imp.terrainSize = 129;
                //imp.worldSize = 512;
                //imp.inputScale = 1;
                //imp.minBatchSize = 17;
                //imp.maxBatchSize = 129;

                imp.layerList.resize( 1 );
                imp.layerList[0].worldSize = 10;
                imp.layerList[0].textureNames.push_back( "checker.png" );
                //imp.layerList[0].textureNames.push_back( "checker.png" );

                mTerrainGroup->defineTerrain( x, y, &imp );

                mTerrainGroup->loadTerrain( x, y, true );

                //mTerrainGroup->defineTerrain( x, y, 0.0f );
            }
            else
            {
                Ogre::String filename = mTerrainGroup->generateFilename( x, y );
                if( Ogre::ResourceGroupManager::getSingleton().resourceExists(
                        mTerrainGroup->getResourceGroup(), filename ) )
                {
                    mTerrainGroup->defineTerrain( x, y );
                }
                else
                {
#if 0
					String filePath = m_terrainTemplate->getHeightMapPath();
					String fileNameExt = FileSystem::getFileExtension(filePath);

					Ogre::DataStreamPtr layerStream = Ogre::ResourceGroupManager::getSingletonPtr()->openResource(
						filePath.c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

					Ogre::Image img;
					img.load(layerStream, fileNameExt.subString(1, fileNameExt.length() - 1).c_str());
					img.flipAroundY();

					Ogre::PixelBox box = img.getPixelBox();


					int imageSize = std::max(img.getWidth(), img.getHeight());

					f32* pData = new float[mTerrainGroup->getTerrainSize() * mTerrainGroup->getTerrainSize()];
					f32* pPtr = pData;

					for (Ogre::uint16 y = 0; y < mTerrainGroup->getTerrainSize(); ++y)
					{
						for (Ogre::uint16 x = 0; x < mTerrainGroup->getTerrainSize(); ++x)
						{
							float xPos = (float)x / (float)mTerrainGroup->getTerrainSize();
							//float yPos = 1.0f - ((float)y / (float)mTerrainGroup->getTerrainSize());
							float yPos = (float)y / (float)mTerrainGroup->getTerrainSize();

							Ogre::ColourValue colourVal = box.getColourAt(xPos * (float)imageSize, yPos * (float)imageSize, 0);
							float val = *pPtr++ = colourVal.r;
							if (val < 0.0f || val > 1.0f)
								val = val;
						}
					}

					mTerrainGroup->defineTerrain(x, y, pData);

					delete[] pData;

					mTerrainsImported = true;
#else
                    Ogre::Image img;
                    // getTerrainImage(x % 2 != 0, y % 2 != 0, img);
                    if( getTerrainImage( false, false, img ) )
                    {
                        mTerrainGroup->defineTerrain( x, y, &img );
                    }
#endif
                }
            }
        }

        bool CTerrainOgre::getTerrainImage( bool flipX, bool flipY, Ogre::Image &img )
        {
            /*
            if( m_terrainTemplate )
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto fileSystem = applicationManager->getFileSystem();

                auto filePath = m_terrainTemplate->getHeightMap();
                if( fileSystem->isExistingFile( filePath ) )
                {
                    img.load( filePath.c_str(),
                              Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                    if( flipX )
                        img.flipAroundY();

                    if( flipY )
                        img.flipAroundX();

                    return true;
                }
            }
             */

            return false;
        }

        void CTerrainOgre::initBlendMaps( Ogre::Terrain *terrain )
        {
            /*
            if( m_terrainTemplate )
            {
                Array<SmartPtr<TerrainLayerTemplate>> layers = m_terrainTemplate->getTerrainLayers();
                for( u32 i = 1; i < layers.size(); ++i )
                {
                    SmartPtr<TerrainLayerTemplate> layerTemplate = layers[i];

                    String fileName = layerTemplate->getBlendTexture();
                    String fileNameExt = Path::getFileExtension( fileName );

                    Ogre::TerrainLayerBlendMap *blendMap = terrain->getLayerBlendMap( i );

                    Ogre::DataStreamPtr layerStream =
                        Ogre::ResourceGroupManager::getSingletonPtr()->openResource(
                            fileName.c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                    Ogre::Image img;
                    img.load( layerStream, fileNameExt.substr( 1, fileNameExt.length() - 1 ).c_str() );

                    Ogre::PixelBox box = img.getPixelBox();

                    float width = img.getWidth();
                    float height = img.getHeight();

                    // blendMap->loadImage(img);

                    float size = terrain->getLayerBlendMapSize();

                    float *pBlend0 = blendMap->getBlendPointer();
                    for( Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y )
                    {
                        for( Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x )
                        {
                            Ogre::ColourValue colourVal =
                                box.getColourAt( ( x / size ) * width, ( y / size ) * height, 0 );
                            // Ogre::ColourValue colourVal = box.getColourAt((x / size) * width, (1.0 -
                            // (y / size)) * height, 0);
                            float val = *pBlend0++ = colourVal.r;
                            if( val < 0.0f || val > 1.0f )
                                val = val;
                        }
                    }

                    blendMap->dirty();
                    blendMap->update();

                    // SmartPtr<CTerrainBlendMap> blendMap0_(new CTerrainBlendMap);
                    // blendMap0_->initialise(this, i);
                    // m_blendLayers.push_back(blendMap0_);
                }

                // for(u32 i=1; i<layers.size(); ++i)
                //{
                //	Ogre::TerrainLayerBlendMap* blendMap = terrain->getLayerBlendMap(i);
                //	blendMap->dirty();
                //	blendMap->update();
                // }
            }
             */
        }

        Array<f32> CTerrainOgre::getHeightData() const
        {
            Array<f32> data;

            Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                f32 *pData = t->getHeightData();
                u32 size = t->getSize() * t->getSize();
                data.reserve( size + data.size() );

                for( u32 i = 0; i < size; ++i )
                {
                    data.push_back( pData[i] );
                }
            }

            return data;
        }

        String CTerrainOgre::getMaterialName() const
        {
            Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                Ogre::String materialName = t->getMaterialName();
                return String( materialName.c_str() );
            }

            return StringUtil::EmptyString;
        }

        void CTerrainOgre::setMaterialName( const String &materialName )
        {
        }

        SmartPtr<ITerrainBlendMap> CTerrainOgre::getBlendMap( u32 index )
        {
            if( index >= m_blendLayers.size() )
            {
                return nullptr;
            }

            return m_blendLayers[index];
        }

        SmartPtr<ITerrainRayResult> CTerrainOgre::intersects( const Ray3F &ray ) const
        {
            Ogre::Ray orgeRay;
            orgeRay.setOrigin(
                Ogre::Vector3( ray.getOrigin().X(), ray.getOrigin().Y(), ray.getOrigin().Z() ) );
            orgeRay.setDirection( Ogre::Vector3( ray.getDirection().X(), ray.getDirection().Y(),
                                                 ray.getDirection().Z() ) );

            Ogre::TerrainGroup::RayResult rayResult = mTerrainGroup->rayIntersects( orgeRay );
            if( rayResult.hit )
            {
                SmartPtr<ITerrainRayResult> terrainRayResult( new CTerrainRayResult );
                terrainRayResult->setPosition(
                    Vector3F( rayResult.position.x, rayResult.position.y, rayResult.position.z ) );
                terrainRayResult->setIntersected( true );
                terrainRayResult->setTerrain( nullptr );

                return terrainRayResult;
            }

            return nullptr;
        }

        u16 CTerrainOgre::getLayerBlendMapSize() const
        {
            Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                return t->getLayerBlendMapSize();
            }

            return 0;
        }

        SmartPtr<IMesh> CTerrainOgre::getMesh() const
        {
            SmartPtr<IMesh> mesh( new Mesh );
            SmartPtr<ISubMesh> subMesh( new SubMesh );
            mesh->addSubMesh( subMesh );

            // create sub mesh data
            SmartPtr<IVertexDeclaration> vertexDeclaration( new VertexDeclaration );
            /*vertexDeclaration->addElement(sizeof(Vector3F), VertexDeclaration::VES_POSITION,
            VertexDeclaration::VET_FLOAT3); vertexDeclaration->addElement(sizeof(Vector3F),
            VertexDeclaration::VES_NORMAL, VertexDeclaration::VET_FLOAT3);
            vertexDeclaration->addElement(sizeof(Vector2F), VertexDeclaration::VES_TEXTURE_COORDINATES,
            VertexDeclaration::VET_FLOAT2, 0); vertexDeclaration->addElement(sizeof(Vector2F),
            VertexDeclaration::VES_TEXTURE_COORDINATES, VertexDeclaration::VET_FLOAT2, 1);
            */
            SmartPtr<IVertexBuffer> vertexBuffer( new VertexBuffer );
            // vertexBuffer->setVertexDeclaration(vertexDeclaration);

            u32 tileSize = 512;
            u32 numVerticies = tileSize * tileSize;

            vertexBuffer->setNumVerticies( numVerticies );
            auto vertexData = static_cast<f32 *>( vertexBuffer->createVertexData() );
            f32 *vertexDataPtr = vertexData;

            return mesh;
        }

        Ogre::TerrainGroup *CTerrainOgre::getTerrainGroup() const
        {
            return mTerrainGroup;
        }

        void CTerrainOgre::setTerrainGroup( Ogre::TerrainGroup *terrainGroup )
        {
            mTerrainGroup = terrainGroup;
        }

        f32 CTerrainOgre::getHeightScale() const
        {
            return 0.0f;
        }

        void CTerrainOgre::setHeightScale( f32 heightScale )
        {
            Ogre::Terrain::ImportData &defaultimp = mTerrainGroup->getDefaultImportSettings();
            defaultimp.inputScale = heightScale;

            Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
            while( ti.hasMoreElements() )
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                t->update();
            }
        }

        void CTerrainOgre::_getObject( void **ppObject ) const
        {
            *ppObject = mTerrainGroup;
        }

        SmartPtr<IGraphicsScene> CTerrainOgre::getSceneManager() const
        {
            return m_sceneManager;
        }

        void CTerrainOgre::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
        {
            m_sceneManager = sceneManager;
        }

        Ogre::SceneManager *CTerrainOgre::getOgreSceneManager() const
        {
            if( auto scene = getSceneManager() )
            {
                Ogre::SceneManager *smgr = nullptr;
                scene->_getObject( (void **)&smgr );
                return smgr;
            }

            return nullptr;
        }

        Ogre::Camera *CTerrainOgre::getTerrainCamera() const
        {
            return m_terrainCamera;
        }

        void CTerrainOgre::setTerrainCamera( Ogre::Camera *terrainCamera )
        {
            m_terrainCamera = terrainCamera;
        }

        Ogre::SceneNode *CTerrainOgre::getTerrainCameraSceneNode() const
        {
            return m_terrainCameraSceneNode;
        }

        void CTerrainOgre::setTerrainCameraSceneNode( Ogre::SceneNode *terrainCameraSceneNode )
        {
            m_terrainCameraSceneNode = terrainCameraSceneNode;
        }

        PagedGeometry *CTerrainOgre::getTrees() const
        {
            return m_trees;
        }

        void CTerrainOgre::setTrees( PagedGeometry *trees )
        {
            m_trees = trees;
        }

        u16 CTerrainOgre::getTerrainSize() const
        {
            return m_terrainSize;
        }

        void CTerrainOgre::setTerrainSize( u16 terrainSize )
        {
            m_terrainSize = terrainSize;
        }

        f32 CTerrainOgre::getTerrainWorldSize() const
        {
            return m_terrainWorldSize;
        }

        void CTerrainOgre::setTerrainWorldSize( f32 terrainWorldSize )
        {
            m_terrainWorldSize = terrainWorldSize;
        }

        void CTerrainOgre::setTextureLayer( s32 layer, const String &textureName )
        {
            auto terrain = mTerrainGroup->getTerrain( 0, 0 );
            terrain->setLayerTextureName( layer, 0, textureName );
        }

        void CTerrainOgre::loadTrees()
        {
            try
            {
                using namespace Ogre;

                Camera *ogreCamera = getTerrainCamera();

                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto window = graphicsSystem->getDefaultWindow();
                //auto viewports = window->getViewports();
                //for( auto viewport : viewports )
                //{
                //    if( viewport )
                //    {
                //        auto camera = viewport->getCamera();
                //        if( camera )
                //        {
                //            camera->_getObject( (void **)&ogreCamera );
                //        }
                //    }
                //}

                //if( !ogreCamera )
                //{
                //    FB_LOG_ERROR( "CTerrain::loadTrees No camera found" );
                //    return;
                //}

                //-------------------------------------- LOAD TREES
                //--------------------------------------
                // Create and configure a new PagedGeometry instance
                m_trees = new PagedGeometry();
                m_trees->setCamera(
                    ogreCamera );  // Set the camera so PagedGeometry knows how to calculate LODs
                m_trees->setPageSize( 80 );  // Set the size of each page of geometry
                m_trees->setInfinite();      // Use infinite paging mode
                m_trees->addDetailLevel<BatchPage>(
                    150, 50 );  // Use batches up to 150 units away, and fade for 30 more units
                //m_trees->addDetailLevel<ImpostorPage>(
                //    500, 50 );  // Use impostors up to 400 units, and for for 50 more units

                auto terrainWorldSize = getTerrainWorldSize();

                // Create a new TreeLoader3D object
                auto bounds =
                    TBounds( -terrainWorldSize, -terrainWorldSize, terrainWorldSize, terrainWorldSize );
                auto treeLoader = new TreeLoader3D( m_trees, bounds );

                // Assign the "treeLoader" to be used to load geometry
                m_trees->setPageLoader( treeLoader );
                // for the PagedGeometry instance

                auto sceneMgr = getOgreSceneManager();

                // Load a tree entity
                auto treeName = String( "Tree" ) + StringUtil::toString( m_ext++ );
                auto myEntity = sceneMgr->createEntity( treeName, "fir05_30.mesh" );

                // Setup the height function (so the Y values of trees can be calculated when they are
                // placed on the terrain)
                HeightFunction::initialize( mTerrainGroup );

                // Randomly place 20,000 copies of the tree on the terrain
                for( int i = 0; i < 1000; i++ )
                {
                    auto yaw = Degree( Ogre::Math::RangeRandom( 0, 360 ) );

                    Ogre::Vector3 position;
                    position.x = Ogre::Math::RangeRandom( -terrainWorldSize, terrainWorldSize );
                    position.z = Ogre::Math::RangeRandom( -terrainWorldSize, terrainWorldSize );
                    position.y = HeightFunction::getTerrainHeight( position.x, position.z );

                    auto scale = Ogre::Math::RangeRandom( 0.2f, 0.6f );

                    treeLoader->addTree( myEntity, position, yaw, scale );
                }

                setTreeMask( m_trees->getSceneNode() );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CTerrainOgre::setTreeMask( Ogre::SceneNode *node )
        {
            auto mask = static_cast<u32>( 0 );
            mask = BitUtil::setFlagValue( mask, IGraphicsObject::SceneFlag, true );

            auto attachedObjects = node->getAttachedObjects();
            for( auto obj : attachedObjects )
            {
                obj->setVisibilityFlags( mask );
            }

            auto children = node->getChildren();
            for( auto child : children )
            {
                setTreeMask( static_cast<Ogre::SceneNode *>( child ) );
            }
        }

        void CTerrainOgre::TerrainStateListener::unload( SmartPtr<ISharedObject> data )
        {
            m_owner = nullptr;
        }

        void CTerrainOgre::TerrainStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = getOwner() )
            {
                if( message->isDerived<StateMessageLoad>() )
                {
                    auto loadMsg = fb::static_pointer_cast<StateMessageLoad>( message );
                    auto type = loadMsg->getType();

                    if( type == StateMessageLoad::LOAD_HASH )
                    {
                        owner->load( nullptr );
                    }
                    else if( type == StateMessageLoad::RELOAD_HASH )
                    {
                        owner->load( nullptr );
                    }
                }
            }
        }

        void CTerrainOgre::TerrainStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto owner = getOwner() )
            {
                auto terrainState = fb::static_pointer_cast<TerrainState>( state );
                auto visible = terrainState->isVisible();
                auto worldTransform = terrainState->getWorldTransform();
                auto position = worldTransform.getPosition();

                auto pos = Ogre::Vector3( position.X(), position.Y(), position.Z() );

                auto terrainGroup = owner->getTerrainGroup();
                if( terrainGroup )
                {
                    auto terrain = terrainGroup->getTerrain( 0, 0 );
                    auto node = terrain->getQuadTree();

                    node->setVisible( visible );

                    if( auto trees = owner->getTrees() )
                    {
                        auto node = trees->getSceneNode();
                        node->setPosition( pos );

                        node->setVisible( visible );
                    }

                    terrainGroup->setOrigin( pos );
                    terrainGroup->update();

                    state->setDirty( false );
                }
            }
        }

        void CTerrainOgre::TerrainStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        SmartPtr<CTerrainOgre> CTerrainOgre::TerrainStateListener::getOwner() const
        {
            auto p = m_owner.load();
            return p.lock();
        }

        void CTerrainOgre::TerrainStateListener::setOwner( SmartPtr<CTerrainOgre> owner )
        {
            m_owner = owner;
        }

        void CTerrainOgre::ShadowRenderTargetListener::preRenderTargetUpdate(
            const Ogre::RenderTargetEvent &evt )
        {
            if( m_nextUpdate > 5 )
            {
                m_tex->getBuffer()->blitFromMemory( *box );
                m_nextUpdate = 0;
            }

            ++m_nextUpdate;
        }

        CTerrainOgre::ShadowRenderTargetListener::ShadowRenderTargetListener( Ogre::TexturePtr tex ) :
            m_tex( tex ),
            m_nextUpdate( 0 )
        {
            float floatMax = 1000.0f * 1000.0f;
            int maxValue = *reinterpret_cast<int *>( &floatMax );

            int size = m_tex->getWidth() * m_tex->getHeight() * sizeof( float ) * 2;
            data = new float[size];
            // memset((void*)data,maxValue,m_tex->getWidth()*m_tex->getHeight()*sizeof(float)*2);
            for( int i = 0; i < size; i += 2 )
            {
                data[i] = 1000.0f;
                data[i + 1] = 1000.0f * 1000.0f;
            }

            box = new Ogre::PixelBox( m_tex->getWidth(), m_tex->getHeight(), m_tex->getDepth(),
                                      m_tex->getFormat(), data );
        }

        CTerrainOgre::ShadowRenderTargetListener::~ShadowRenderTargetListener()
        {
            delete[] data;
        }

        void CTerrainOgre::ShadowRenderTargetListener::preViewportUpdate(
            const Ogre::RenderTargetViewportEvent &evt )
        {
        }
    }  // namespace render
}  // namespace fb
