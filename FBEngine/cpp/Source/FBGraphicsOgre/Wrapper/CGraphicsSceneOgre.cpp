#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSceneOgre.h>
#include <FBGraphicsOgre/Wrapper/CSceneNodeOgre.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsMeshOgre.h>
#include <FBGraphicsOgre/Wrapper/CLightOgre.h>
#include <FBGraphicsOgre/Wrapper/CParticleSystem.h>
#include <FBGraphicsOgre/Wrapper/CBillboardSetOgre.h>
#include <FBGraphicsOgre/Wrapper/CAnimationStateController.h>
#include <FBGraphicsOgre/Wrapper/CAnimationTextureControl.h>
#include <FBGraphicsOgre/Wrapper/CCameraOgre.h>
#include <FBGraphicsOgre/Wrapper/CDynamicLines.h>
#include <FBGraphicsOgre/Wrapper/CTerrainOgre.h>
#include <FBGraphicsOgre/Ogre/OgreUtil.h>
#include <FBGraphicsOgre/Wrapper/CDecalCursor.h>
#include <FBGraphicsOgre/Ogre/NGPlantLoader.h>
#include <FBGraphicsOgre/Ogre/ScreenSpaceEffect.h>
#include <FBGraphicsOgre/Ogre/SSEffectRenderer.h>
#include <FBGraphicsOgre/Ogre/SSEffectClear.h>
#include <FBGraphicsOgre/Ogre/LightShafts.h>
#include <FBGraphicsOgre/Ogre/ScreenSpaceEffect.h>
#include <FBGraphicsOgre/Ogre/SSEffectRenderer.h>
#include <FBGraphicsOgre/Ogre/SSEffectClear.h>
#include <FBGraphicsOgre/Ogre/ShadowFrameListener.h>
#include <FBGraphicsOgre/Wrapper/CInstanceManager.h>
#include <FBGraphicsOgre/Wrapper/CInstancedObject.h>
#include <FBGraphicsOgre/Wrapper/CMaterialOgre.h>
#include <FBGraphicsOgre/Ogre/DynamicMesh.h>
#include <FBCore/FBCore.h>
#include <OgreOverlaySystem.h>
#include <OgreSceneManager.h>
#include <OgreShaderGenerator.h>
#include <Ogre.h>
#include <tinyxml.h>

#include "FBParticleSystem/CParticleSystem.h"
#include "Wrapper/CDynamicMesh.h"

#ifdef FB_OGRE_USE_MESH_SPLITTERS
#    include <DestructibleMeshSplitter.h>
#    include <OgreMeshExtractor.h>
#    include <VertexMerger.h>
#endif

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb, CGraphicsSceneOgre, SharedObject<IGraphicsScene> );
        u32 CGraphicsSceneOgre::m_nextGeneratedNameExt = 0;

        CGraphicsSceneOgre::CGraphicsSceneOgre() :
            m_pRaySceneQuery( nullptr ),
            nextRenderQueueUpdate( 0 ),
            m_isClearing( false )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            auto rootSceneNode = factoryManager->make_ptr<RootSceneNode>();
            rootSceneNode->setCreator( this );
            m_rootSceneNode = rootSceneNode;

            // m_registeredSceneNodes.reserve(500);
            // m_registeredGfxObjects.reserve(500);

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto stateManager = applicationManager->getStateManager();
            auto threadPool = applicationManager->getThreadPool();

            //m_frameListener = fb::make_ptr<SceneManagerFrameListener>( this );
            //graphicsSystem->addFrameListener( m_frameListener );

            m_animationNamePrefix = StringUtil::EmptyString;
            m_animationNameSuffix = StringUtil::EmptyString;

            m_sceneNodes = fb::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>();
            m_registeredSceneNodes = fb::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>();

            m_state = fb::make_ptr<SceneManagerState>();

            m_stateObject = stateManager->addStateObject();
            m_stateObject->setState( m_state );

            auto sceneManagerStateListener = factoryManager->make_ptr<SceneManagerStateListener>();
            sceneManagerStateListener->setOwner( this );
            m_stateListener = sceneManagerStateListener;
            m_stateObject->addStateListener( m_stateListener );

            m_state->setStateContext( m_stateObject );

            auto renderTask = graphicsSystem->getRenderTask();
            // m_state->setTaskId(renderTask);

            m_materialSharedListener = fb::make_ptr<MaterialSharedListener>();
            m_materialSharedListener->setOwner( this );

            m_skyboxMaterialListener = fb::make_ptr<SkyboxStateListener>();
            m_skyboxMaterialListener->setOwner( this );
        }

        CGraphicsSceneOgre::CGraphicsSceneOgre( Ogre::SceneManager *sceneManager ) :
            m_pRaySceneQuery( nullptr ),
            nextRenderQueueUpdate( 0 ),
            m_isClearing( false )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            m_sceneManager = sceneManager;

            auto rootSceneNode = factoryManager->make_ptr<RootSceneNode>();
            rootSceneNode->setCreator( this );

            m_rootSceneNode = rootSceneNode;
            m_rootSceneNode->load( nullptr );

            // m_registeredSceneNodes.reserve(500);
            // m_registeredGfxObjects.reserve(500);

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            //m_frameListener = fb::make_ptr<SceneManagerFrameListener>( this );
            //graphicsSystem->addFrameListener( m_frameListener );

            m_animationNamePrefix = StringUtil::EmptyString;
            m_animationNameSuffix = StringUtil::EmptyString;

            m_state = fb::make_ptr<SceneManagerState>();

            auto stateManager = applicationManager->getStateManager();
            auto threadPool = applicationManager->getThreadPool();

            m_stateObject = stateManager->addStateObject();
            m_stateObject->setState( m_state );

            auto sceneManagerStateListener = factoryManager->make_ptr<SceneManagerStateListener>();
            sceneManagerStateListener->setOwner( this );
            m_stateListener = sceneManagerStateListener;
            m_stateObject->addStateListener( m_stateListener );

            m_state->setStateContext( m_stateObject );

            if( threadPool )
            {
                auto numThreads = threadPool->getNumThreads();
                if( numThreads == 0 )
                {
                    m_state->setTaskId( Thread::Task::Primary );
                }
                else
                {
                    m_state->setTaskId( Thread::Task::Render );
                }
            }
            else
            {
                m_state->setTaskId( Thread::Task::Primary );
            }

            m_materialSharedListener = fb::make_ptr<MaterialSharedListener>();
            m_materialSharedListener->setOwner( this );

            m_skyboxMaterialListener = fb::make_ptr<SkyboxStateListener>();
            m_skyboxMaterialListener->setOwner( this );
        }

        CGraphicsSceneOgre::~CGraphicsSceneOgre()
        {
            unload( nullptr );
        }

        void CGraphicsSceneOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
#if _DEBUG
                auto task = Thread::getCurrentTask();
                auto renderTask =
                    core::IApplicationManager::instance()->getGraphicsSystem()->getRenderTask();
                FB_ASSERT( task == renderTask );
#endif

                setLoadingState( LoadingState::Loading );

                auto root = Ogre::Root::getSingletonPtr();

                FB_ASSERT( root );

                auto type = getType();
                auto name = getName();

                auto sceneMgr = root->createSceneManager( type, name );
                FB_ASSERT( sceneMgr );

                auto overlaySystem = Ogre::OverlaySystem::getSingletonPtr();
                FB_ASSERT( overlaySystem );

                sceneMgr->addRenderQueueListener( overlaySystem );

                sceneMgr->setAmbientLight( Ogre::ColourValue::White * 0.5f );

                m_sceneManager = sceneMgr;

                auto rootNode = sceneMgr->getRootSceneNode();
                auto rootSceneNode = fb::static_pointer_cast<CSceneNodeOgre>( m_rootSceneNode );
                rootSceneNode->initialise( rootNode );

                auto shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
                if( shaderGenerator )
                {
                    shaderGenerator->addSceneManager( sceneMgr );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CGraphicsSceneOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto stateManager = applicationManager->getStateManager();
                    FB_ASSERT( stateManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    FB_ASSERT( graphicsSystem );

                    if( m_materialSharedListener )
                    {
                        if( auto skyboxMaterial = getSkyboxMaterial() )
                        {
                            skyboxMaterial->removeObjectListener( m_materialSharedListener.get() );
                        }

                        m_materialSharedListener->unload( data );
                        m_materialSharedListener = nullptr;
                    }

                    if( m_skyboxMaterialListener )
                    {
                        if( auto skyboxMaterial = getSkyboxMaterial() )
                        {
                            if( auto stateContext = skyboxMaterial->getStateObject() )
                            {
                                stateContext->removeStateListener( m_skyboxMaterialListener );
                            }
                        }

                        m_skyboxMaterialListener = nullptr;
                    }

                    m_skyboxMaterial = nullptr;

                    for( auto obj : m_graphicsObjects )
                    {
                        obj->unload( nullptr );
                    }

                    m_graphicsObjects.clear();
                    m_rootSceneNode->removeChildren();

                    m_registeredSceneNodes = nullptr;
                    m_registeredGfxObjects.clear();

                    //FB_ASSERT( getSceneNodes() && getSceneNodes()->empty() );

                    if( auto pSceneNodes = getSceneNodes() )
                    {
                        auto &sceneNodes = *pSceneNodes;
                        for( auto sceneNode : sceneNodes )
                        {
                            sceneNode->detachAllObjects();
                        }

                        for( auto sceneNode : sceneNodes )
                        {
                            sceneNode->unload( nullptr );
                        }
                    }

                    m_rootSceneNode->removeChildren();
                    setSceneNodes( nullptr );

                    m_rootSceneNode->unload( nullptr );
                    m_rootSceneNode = nullptr;

                    if( m_sceneManager )
                    {
                        m_sceneManager->clearScene();
                        m_sceneManager = nullptr;
                    }

                    //graphicsSystem->removeFrameListener( m_frameListener );

                    if( auto stateObject = getStateObject() )
                    {
                        if( auto stateListener = getStateListener() )
                        {
                            stateObject->removeStateListener( stateListener );
                        }

                        stateManager->removeStateObject( stateObject );

                        stateObject->unload( nullptr );
                        setStateObject( nullptr );
                    }

                    if( auto stateListener = getStateListener() )
                    {
                        stateListener->unload( nullptr );
                        setStateListener( nullptr );
                    }

                    CGraphicsScene::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CGraphicsSceneOgre::update()
        {
            auto activeCamera = getActiveCamera();
            if( !activeCamera )
            {
                for( auto object : m_graphicsObjects )
                {
                    if( object )
                    {
                        if( object->isDerived<ICamera>() )
                        {
                            auto camera = fb::static_pointer_cast<ICamera>( object );
                            camera->setVisible( true );
                            setActiveCamera( activeCamera );
                        }
                    }
                }
            }

            for( auto terrain : m_terrains )
            {
                if( terrain )
                {
                    terrain->update();
                }
            }
        }

        void CGraphicsSceneOgre::setType( const String &type )
        {
            m_type = type;
        }

        String CGraphicsSceneOgre::getName() const
        {
            return m_name;
        }

        void CGraphicsSceneOgre::setName( const String &name )
        {
            m_name = name;
        }

        SmartPtr<IStateContext> CGraphicsSceneOgre::getStateObject() const
        {
            return m_stateObject;
        }

        void CGraphicsSceneOgre::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        SmartPtr<IStateListener> CGraphicsSceneOgre::getStateListener() const
        {
            return m_stateListener;
        }

        void CGraphicsSceneOgre::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        SmartPtr<IMaterial> CGraphicsSceneOgre::getSkyboxMaterial() const
        {
            return m_skyboxMaterial;
        }

        void CGraphicsSceneOgre::setSkyboxMaterial( SmartPtr<IMaterial> skyboxMaterial )
        {
            m_skyboxMaterial = skyboxMaterial;
        }

        SmartPtr<IGraphicsObject> CGraphicsSceneOgre::addGraphicsObject( const String &name,
                                                                         const String &type )
        {
            if( type == ( "Billboard" ) )
            {
                return addBillboardSet( name );
            }
            if( type == ( "Camera" ) )
            {
                return addCamera( name );
            }
            if( type == ( "DynamicLines" ) )
            {
                auto dynamicLines = SmartPtr<CDynamicLines>( new CDynamicLines );
                dynamicLines->initialise();
                return dynamicLines;
            }
            if( type == ( "DynamicMesh" ) )
            {
                // return addDynamicMesh(name);
            }
            else if( type == ( "Light" ) )
            {
                return addLight( name );
            }
            else if( type == ( "ParticleSystem" ) )
            {
                // return addParticleSystem(name);
            }
            else
            {
                FB_EXCEPTION( "Could not create graphics object." );
            }

            return nullptr;
        }

        SmartPtr<IGraphicsObject> CGraphicsSceneOgre::addGraphicsObject( const String &type )
        {
            String name = getUniqueName( type );
            return addGraphicsObject( name, type );
        }

        SmartPtr<IGraphicsObject> CGraphicsSceneOgre::addGraphicsObjectByTypeId( u32 id )
        {
            if( id == IDynamicMesh::typeInfo() )
            {
                return fb::make_ptr<CDynamicMesh>();
            }
            else if( id == IParticleSystem::typeInfo() )
            {
                return fb::make_ptr<PUParticleSystem>();
                //return fb::make_ptr<CParticleSystem>();
            }

            return nullptr;
        }

        void CGraphicsSceneOgre::clearScene()
        {
            try
            {
                ISharedObject::ScopedLock lock(
                    core::IApplicationManager::instance()->getGraphicsSystem() );

                m_isClearing = true;

                m_registeredSceneNodes = nullptr;
                m_registeredGfxObjects.clear();

                m_graphicsObjects.clear();

                auto pSceneNodes = m_sceneNodes;
                auto &sceneNodes = *pSceneNodes;
                for( auto sceneNode : sceneNodes )
                {
                    sceneNode->detachAllObjects();
                }

                m_rootSceneNode->removeChildren();

                m_sceneNodes = nullptr;

                if( m_sceneManager )
                {
                    m_sceneManager->clearScene();
                }

                m_isClearing = false;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool CGraphicsSceneOgre::hasAnimation( const String &animationName )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_sceneManager->hasAnimation( animationName.c_str() );
        }

        bool CGraphicsSceneOgre::destroyAnimation( const String &animationName )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            if( m_sceneManager->hasAnimation( animationName.c_str() ) )
            {
                m_sceneManager->destroyAnimation( animationName.c_str() );
                return true;
            }

            return false;
        }

        void CGraphicsSceneOgre::setAmbientLight( const ColourF &colour )
        {
            if( m_state )
            {
                m_state->setAmbientLight( colour );
            }
        }

        bool CGraphicsSceneOgre::validateGfxObjName( const String &name ) const
        {
            if( name.length() == 0 )
            {
                return false;
            }

            for( auto object : m_graphicsObjects )
            {
                auto handle = object->getHandle();
                if( handle->getName() == name )
                {
                    return false;
                }
            }

            return true;
        }

        SmartPtr<ICamera> CGraphicsSceneOgre::addCamera( const String &name )
        {
            try
            {
                FB_ASSERT( validateGfxObjName( name ) );

                auto camera = fb::make_ptr<CCameraOgre>();

                if( auto handle = camera->getHandle() )
                {
                    handle->setName( name );
                }

                camera->setCreator( this );

                m_graphicsObjects.push_back( camera );

                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                graphicsSystem->loadObject( camera );

                return camera;
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_MESSAGE( "Graphics", e.getFullDescription().c_str() );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<ICamera> CGraphicsSceneOgre::getCamera( const String &name )
        {
            for( auto object : m_graphicsObjects )
            {
                auto handle = object->getHandle();
                if( handle->getName() == name )
                {
                    return object;
                }
            }

            return nullptr;
        }

        SmartPtr<ICamera> CGraphicsSceneOgre::getCamera() const
        {
            return nullptr;
        }

        SmartPtr<ICamera> CGraphicsSceneOgre::getActiveCamera() const
        {
            return nullptr;
        }

        void CGraphicsSceneOgre::setActiveCamera( SmartPtr<ICamera> camera )
        {
        }

        bool CGraphicsSceneOgre::hasCamera( const String &name )
        {
            for( auto object : m_graphicsObjects )
            {
                auto handle = object->getHandle();
                if( handle->getName() == name )
                {
                    return true;
                }
            }

            return false;
        }

        SmartPtr<ILight> CGraphicsSceneOgre::addLight( const String &name )
        {
            try
            {
                FB_ASSERT( validateGfxObjName( name ) );

                ISharedObject::ScopedLock lock(
                    core::IApplicationManager::instance()->getGraphicsSystem() );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto light = factoryManager->make_ptr<CLightOgre>();
                light->setCreator( this );

                if( auto handle = light->getHandle() )
                {
                    handle->setName( name );
                }

                graphicsSystem->loadObject( light, false );

                return light;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<ILight> CGraphicsSceneOgre::getLight( const String &name ) const
        {
            for( auto object : m_graphicsObjects )
            {
                auto handle = object->getHandle();
                if( handle->getName() == name )
                {
                    return object;
                }
            }

            return nullptr;
        }

        SmartPtr<ISceneNode> CGraphicsSceneOgre::getSceneNode( const String &name ) const
        {
            auto pSceneNodes = m_sceneNodes;
            auto &sceneNodes = *pSceneNodes;
            for( auto sceneNode : sceneNodes )
            {
                auto handle = sceneNode->getHandle();
                if( handle->getName() == name )
                {
                    return sceneNode;
                }
            }

            return nullptr;
        }

        SmartPtr<ISceneNode> CGraphicsSceneOgre::getSceneNodeById( hash32 id ) const
        {
            auto pSceneNodes = m_sceneNodes;
            auto &sceneNodes = *pSceneNodes;
            for( auto sceneNode : sceneNodes )
            {
                auto handle = sceneNode->getHandle();
                if( handle->getHash() == id )
                {
                    return sceneNode;
                }
            }

            return nullptr;
        }

        SmartPtr<ISceneNode> CGraphicsSceneOgre::getRootSceneNode() const
        {
            return m_rootSceneNode;
        }

        SmartPtr<IGraphicsMesh> CGraphicsSceneOgre::addMesh( const String &name, const String &meshName )
        {
            try
            {
#ifndef _FINAL_
                validateGfxObjName( name );
#endif

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto graphicsObject = factoryManager->make_ptr<CGraphicsMeshOgre>();
                FB_ASSERT( graphicsObject );

                auto handle = graphicsObject->getHandle();
                FB_ASSERT( handle );

                if( handle )
                {
                    handle->setName( name );
                }

                graphicsObject->setCreator( this );
                graphicsObject->setMeshName( meshName );

                _addGraphicsObject( graphicsObject );

                graphicsSystem->loadObject( graphicsObject );

                return graphicsObject;
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription() );
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IGraphicsMesh> CGraphicsSceneOgre::addMesh( const String &meshName )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto graphicsObject = factoryManager->make_ptr<CGraphicsMeshOgre>();
                FB_ASSERT( graphicsObject );

                auto handle = graphicsObject->getHandle();
                if( handle )
                {
                    auto name = getUniqueName( "Mesh" );
                    handle->setName( name );
                }

                graphicsObject->setCreator( this );
                graphicsObject->setMeshName( meshName );

                _addGraphicsObject( graphicsObject );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                graphicsSystem->loadObject( graphicsObject );

                return graphicsObject;
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription() );
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IGraphicsMesh> CGraphicsSceneOgre::getMesh( const String &name ) const
        {
            for( auto &graphicsObject : m_graphicsObjects )
            {
                auto handle = graphicsObject->getHandle();
                auto handleName = handle->getName();
                if( handleName == name )
                {
                    return graphicsObject;
                }
            }

            return nullptr;
        }

        SmartPtr<IBillboardSet> CGraphicsSceneOgre::addBillboardSet( const String &name, u32 poolSize )
        {
#ifndef _FINAL_
            validateGfxObjName( name );
#endif

            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::BillboardSet *bbSet = m_sceneManager->createBillboardSet( name.c_str(), poolSize );

            // auto billboardSet = SmartPtr<CBillboardSetOgre>( new CBillboardSetOgre );
            // billboardSet->initialise( bbSet );
            // billboardSet->getHandle()->setName( name );
            //// m_graphicsObjects[name] = billboardSet.get();
            // return billboardSet;

            return nullptr;
        }

        SmartPtr<IBillboardSet> CGraphicsSceneOgre::getBillboardSet( const String &name )
        {
            // auto it = m_graphicsObjects.find( name );
            // if( it != m_graphicsObjects.end() )
            //{
            //     return it->second;
            // }

            return nullptr;
        }

        SmartPtr<IParticleSystem> CGraphicsSceneOgre::addParticleSystem( const String &name,
                                                                         const String &templateName )
        {
#ifndef _FINAL_
            validateGfxObjName( name );
#endif

#if FB_OGRE_USE_PARTICLE_UNIVERSE
            SmartPtr<PUParticleSystem> particleSystem;  // (new PUParticleSystem(this), true);
            particleSystem->setName( name );
            particleSystem->setTemplateName( templateName );
            // m_graphicsObjects[name] = particleSystem.get();

            return particleSystem;
#endif

            return nullptr;
        }

        SmartPtr<IParticleSystem> CGraphicsSceneOgre::addParticleSystem(
            const String &name, SmartPtr<IParticleSystemBuilder> &builder )
        {
            return nullptr;
        }

        SmartPtr<IParticleSystem> CGraphicsSceneOgre::getParticleSystem( const String &name ) const
        {
            // GraphicsObjects::const_iterator it = m_graphicsObjects.find(name);
            // if( it != m_graphicsObjects.end() )
            //{
            //	return it->second;
            // }

            return nullptr;
        }

        SmartPtr<IAnimationStateController> CGraphicsSceneOgre::createAnimationStateController()
        {
            SmartPtr<IAnimationStateController>
                animStateCtrl;  // (new CAnimationStateController(this), true);
            return animStateCtrl;
        }

        SmartPtr<IAnimationTextureControl> CGraphicsSceneOgre::createAnimationTextureCtrl(
            SmartPtr<IMaterialTexture> textureUnit, bool clone, const String &clonedMaterialName )
        {
            Ogre::TextureUnitState *textureUnitState = nullptr;
            // textureUnit->_getObject((void**)&textureUnitState);

            auto textureCtrl = new CAnimationTextureControl;
            textureCtrl->initialise( textureUnitState );

            SmartPtr<IAnimationTextureControl> animTextureCtrl;  // (textureCtrl, true);
            return animTextureCtrl;
        }

        bool CGraphicsSceneOgre::removeSceneNode( SmartPtr<ISceneNode> sceneNode )
        {
            if( sceneNode )
            {
                if( m_isClearing )
                {
                    return false;
                }

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                graphicsSystem->unloadObject( sceneNode );

                if( auto pSceneNodes = getSceneNodes() )
                {
                    auto &sceneNodes = *pSceneNodes;
                    auto sceneNodesArray =
                        Array<SmartPtr<ISceneNode>>( sceneNodes.begin(), sceneNodes.end() );

                    auto it = std::find( sceneNodesArray.begin(), sceneNodesArray.end(), sceneNode );
                    if( it != sceneNodesArray.end() )
                    {
                        sceneNodesArray.erase( it );

                        auto sceneNodes = fb::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>();
                        *sceneNodes = ConcurrentArray<SmartPtr<ISceneNode>>( sceneNodesArray.begin(),
                                                                             sceneNodesArray.end() );
                        setSceneNodes( sceneNodes );

                        return true;
                    }
                }
            }

            return false;
        }

        bool CGraphicsSceneOgre::removeGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject )
        {
            if( m_isClearing )
            {
                return false;
            }

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            if( graphicsObject )
            {
                graphicsSystem->unloadObject( graphicsObject );

                auto it =
                    std::find( m_graphicsObjects.begin(), m_graphicsObjects.end(), graphicsObject );
                if( it != m_graphicsObjects.end() )
                {
                    m_graphicsObjects.erase( it );
                    return true;
                }
            }

            return false;
        }

        Array<SmartPtr<IGraphicsObject>> CGraphicsSceneOgre::getGraphicsObjects() const
        {
            return m_graphicsObjects;
        }

        bool CGraphicsSceneOgre::getEnableSkybox() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<SceneManagerState>() )
                {
                    return state->getEnableSkybox();
                }
            }

            return false;
        }

        void CGraphicsSceneOgre::setSkyBox( bool enable, SmartPtr<IMaterial> material, f32 distance,
                                            bool drawFirst )
        {
            try
            {
                if( auto stateContext = getStateObject() )
                {
                    if( auto state = stateContext->getStateByType<SceneManagerState>() )
                    {
                        state->setEnableSkybox( enable );
                    }
                }

                auto skyboxMaterial = getSkyboxMaterial();
                if( skyboxMaterial != material )
                {
                    if( skyboxMaterial )
                    {
                        skyboxMaterial->removeObjectListener( m_materialSharedListener.get() );

                        if( auto stateContext = skyboxMaterial->getStateObject() )
                        {
                            stateContext->removeStateListener( m_skyboxMaterialListener );
                        }
                    }

                    setSkyboxMaterial( material );

                    if( material )
                    {
                        material->addObjectListener( m_materialSharedListener );

                        if( auto stateContext = material->getStateObject() )
                        {
                            stateContext->addStateListener( m_skyboxMaterialListener );
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CGraphicsSceneOgre::setFog( u32 fogMode, const ColourF &colour, f32 expDensity,
                                         f32 linearStart, f32 linearEnd )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_sceneManager->setFog( static_cast<Ogre::FogMode>( fogMode ),
                                    Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a ),
                                    expDensity, linearStart, linearEnd );
        }

        SmartPtr<ISceneNode> CGraphicsSceneOgre::addSceneNode()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();
                auto sceneNode = factoryManager->make_ptr<CSceneNodeOgre>();
                sceneNode->setCreator( this );

                m_sceneNodes->push_back( sceneNode );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                graphicsSystem->loadObject( sceneNode );

                return sceneNode;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<ISceneNode> CGraphicsSceneOgre::addSceneNode( const String &name )
        {
            try
            {
                auto hash = StringUtil::getHash( name );

                auto applicationManager = core::IApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();
                auto sceneNode = factoryManager->make_ptr<CSceneNodeOgre>();
                sceneNode->setCreator( this );

                auto handle = sceneNode->getHandle();
                handle->setName( name );

                m_sceneNodes->push_back( sceneNode );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                graphicsSystem->loadObject( sceneNode );

                return sceneNode;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<ISceneNode> CGraphicsSceneOgre::addStaticNode()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();
            auto sceneNode = factoryManager->make_ptr<CSceneNodeOgre>();
            sceneNode->setCreator( this );

            m_sceneNodes->push_back( sceneNode );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            graphicsSystem->loadObject( sceneNode );

            return sceneNode;
        }

        void CGraphicsSceneOgre::registerSceneNodeForUpdates( SmartPtr<ISceneNode> sceneNode )
        {
            m_registeredSceneNodes->push_back( sceneNode );
        }

        bool CGraphicsSceneOgre::unregisteredForUpdates( SmartPtr<ISceneNode> sceneNode )
        {
            auto pSceneNodes = m_registeredSceneNodes;
            auto &sceneNodes = *pSceneNodes;
            auto sceneNodesArray = Array<SmartPtr<ISceneNode>>( sceneNodes.begin(), sceneNodes.end() );

            auto it = std::find( sceneNodesArray.begin(), sceneNodesArray.end(), sceneNode );
            if( it != sceneNodesArray.end() )
            {
                sceneNodesArray.erase( it );
                m_sceneNodes = boost::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>(
                    sceneNodesArray.begin(), sceneNodesArray.end() );
                return true;
            }

            return false;
        }

        bool CGraphicsSceneOgre::unregisteredForUpdates( ISceneNode *sceneNode )
        {
            auto pSceneNodes = m_registeredSceneNodes;
            auto &sceneNodes = *pSceneNodes;
            auto sceneNodesArray = Array<SmartPtr<ISceneNode>>( sceneNodes.begin(), sceneNodes.end() );

            auto it = std::find( sceneNodesArray.begin(), sceneNodesArray.end(), sceneNode );
            if( it != sceneNodesArray.end() )
            {
                sceneNodesArray.erase( it );
                m_sceneNodes = boost::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>(
                    sceneNodesArray.begin(), sceneNodesArray.end() );
                return true;
            }

            return false;
        }

        void CGraphicsSceneOgre::registerForUpdates( SmartPtr<IGraphicsObject> gfxObject )
        {
            SpinRWMutex::ScopedLock lock( ListMutex );
            // m_registeredGfxObjects.push_back(gfxObject.get());
        }

        bool CGraphicsSceneOgre::unregisteredForUpdates( SmartPtr<IGraphicsObject> gfxObject )
        {
            SpinRWMutex::ScopedLock lock( ListMutex );
            // return m_registeredGfxObjects.erase_element(gfxObject.get());

            return false;
        }

        bool CGraphicsSceneOgre::unregisteredForUpdates( IGraphicsObject *gfxObject )
        {
            SpinRWMutex::ScopedLock lock( ListMutex );
            // return m_registeredGfxObjects.erase_element(gfxObject);

            return false;
        }

        Ogre::SceneManager *CGraphicsSceneOgre::getSceneManager() const
        {
            return m_sceneManager;
        }

        void CGraphicsSceneOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_sceneManager;
        }

        void CGraphicsSceneOgre::clearQueues()
        {
            EntityDeleteQueue entityDeleteQueue;

            {
                SharedMutex::ScopedLock lock( EntityDeleteQueueMutex );
                entityDeleteQueue = m_entityDeleteQueue;
                m_entityDeleteQueue.clear();
            }

            SceneNodeDeleteQueue sceneNodeDeleteQueue;

            {
                SharedMutex::ScopedLock lock( SceneNodeDeleteQueueMutex );
                sceneNodeDeleteQueue = m_sceneNodeDeleteQueue;
                m_sceneNodeDeleteQueue.clear();
            }

            ParticleSysDeleteQueue particleSysDeleteQueue;

            {
                SharedMutex::ScopedLock lock( SceneNodeDeleteQueueMutex );
                particleSysDeleteQueue = m_particleSysDeleteQueue;
                m_particleSysDeleteQueue.clear();
            }

            for( u32 i = 0; i < entityDeleteQueue.size(); ++i )
            {
                Ogre::Entity *entity = entityDeleteQueue[i];
                m_sceneManager->destroyEntity( entity );
            }

            for( u32 i = 0; i < sceneNodeDeleteQueue.size(); ++i )
            {
                Ogre::SceneNode *sceneNode = sceneNodeDeleteQueue[i];
                m_sceneManager->destroySceneNode( sceneNode->getName() );
            }

            for( u32 i = 0; i < particleSysDeleteQueue.size(); ++i )
            {
                Ogre::ParticleSystem *particleSystem = particleSysDeleteQueue[i];
                m_sceneManager->destroyParticleSystem( particleSystem );
            }
        }

        String CGraphicsSceneOgre::getUniqueName( const String &baseName ) const
        {
            return baseName + StringUtil::toString( m_nextGeneratedNameExt++ );
        }

        SmartPtr<ITerrain> CGraphicsSceneOgre::createTerrain()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto terrain = factoryManager->make_ptr<CTerrainOgre>();
            FB_ASSERT( terrain );

            terrain->setSceneManager( this );

            SpinRWMutex::ScopedLock lock( m_terrainsMutex );
            m_terrains.push_back( terrain );

            graphicsSystem->loadObject( terrain );

            return terrain;
        }

        void CGraphicsSceneOgre::destroyTerrain( SmartPtr<ITerrain> terrain )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            if( terrain )
            {
                graphicsSystem->unloadObject( terrain );

                SpinRWMutex::ScopedLock lock( m_terrainsMutex );
                m_terrains.erase( std::remove( m_terrains.begin(), m_terrains.end(), terrain ),
                                  m_terrains.end() );
            }
        }

        SmartPtr<IDecalCursor> CGraphicsSceneOgre::addDecalCursor( const String &terrainMaterial,
                                                                   const String &decalTextureName,
                                                                   const Vector2F &size )
        {
            auto decalCursor = SmartPtr<CDecalCursor>( new CDecalCursor );
            // decalCursor->initialise(this, terrainMaterial, decalTextureName, size);
            return decalCursor;
        }

        // raycast from a point in to the scene.
        // returns success or failure.
        // on success the point is returned in the result.
        bool CGraphicsSceneOgre::castRay( const Ray3F &ray, Vector3F &result )
        {
            // create the ray to test
            Vector3F start = ray.getOrigin();
            Vector3F normal = ray.getDirection();

            Ogre::Ray ogreRay( Ogre::Vector3( start.X(), start.Y(), start.Z() ),
                               Ogre::Vector3( normal.X(), normal.Y(), normal.Z() ) );

            if( !m_pRaySceneQuery )
            {
                // create the ray scene query object
                m_pRaySceneQuery = m_sceneManager->createRayQuery(
                    Ogre::Ray(), Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK );
                if( nullptr == m_pRaySceneQuery )
                {
                    FB_LOG_MESSAGE( "SceneManager", "Failed to create Ogre::RaySceneQuery instance" );
                    return ( false );
                }

                m_pRaySceneQuery->setSortByDistance( true );
            }

            // check we are initialised
            if( m_pRaySceneQuery != nullptr )
            {
                // create a query object
                m_pRaySceneQuery->setRay( ogreRay );

                // execute the query, returns a vector of hits
                if( m_pRaySceneQuery->execute().size() <= 0 )
                {
                    // raycast did not hit an objects bounding box
                    return ( false );
                }
            }
            else
            {
                FB_LOG_MESSAGE( "SceneManager", "Cannot raycast without RaySceneQuery instance" );
                return ( false );
            }

            // at this point we have raycast to a series of different objects bounding boxes.
            // we need to test these different objects to see which is the first polygon hit.
            // there are some minor optimizations (distance based) that mean we wont have to
            // check all of the objects most of the time, but the worst case scenario is that
            // we need to test every triangle of every object.
            Ogre::Real closest_distance = -1.0f;
            Ogre::Vector3 closest_result;
            Ogre::RaySceneQueryResult &query_result = m_pRaySceneQuery->getLastResults();
            for( size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++ )
            {
                // stop checking if we have found a raycast hit that is closer
                // than all remaining entities
                if( ( closest_distance >= 0.0f ) &&
                    ( closest_distance < query_result[qr_idx].distance ) )
                {
                    break;
                }

                // only check this result if its a hit against an entity
                if( ( query_result[qr_idx].movable != nullptr ) &&
                    ( query_result[qr_idx].movable->getMovableType().compare( "Entity" ) == 0 ) )
                {
                    // get the entity to check
                    auto pentity = static_cast<Ogre::Entity *>( query_result[qr_idx].movable );

                    // mesh data to retrieve
                    size_t vertex_count;
                    size_t index_count;
                    Ogre::Vector3 *vertices;
                    unsigned long *indices;

                    // get the mesh information
                    GetMeshInformation( pentity->getMesh(), vertex_count, vertices, index_count, indices,
                                        pentity->getParentNode()->_getDerivedPosition(),
                                        pentity->getParentNode()->_getDerivedOrientation(),
                                        pentity->getParentNode()->_getDerivedScale() );

                    // test for hitting individual triangles on the mesh
                    bool new_closest_found = false;
                    for( int i = 0; i < static_cast<int>( index_count ); i += 3 )
                    {
                        // check for a hit against this triangle
                        std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(
                            ogreRay, vertices[indices[i]], vertices[indices[i + 1]],
                            vertices[indices[i + 2]], true, false );

                        // if it was a hit check if its the closest
                        if( hit.first )
                        {
                            if( ( closest_distance < 0.0f ) || ( hit.second < closest_distance ) )
                            {
                                // this is the closest so far, save it off
                                closest_distance = hit.second;
                                new_closest_found = true;
                            }
                        }
                    }

                    // free the verticies and indicies memory
                    delete[] vertices;
                    delete[] indices;

                    // if we found a new closest raycast for this object, update the
                    // closest_result before moving on to the next object.
                    if( new_closest_found )
                    {
                        closest_result = ogreRay.getPoint( closest_distance );
                    }
                }
            }

            // return the result
            if( closest_distance >= 0.0f )
            {
                // raycast success
                result = Vector3F( closest_result.x, closest_result.y, closest_result.z );
                return ( true );
            }
            // raycast failed
            return ( false );
        }

        // Get the mesh information for the given mesh.
        // Code found in Wiki: www.ogre3d.org/wiki/index.php/RetrieveVertexData
        void CGraphicsSceneOgre::GetMeshInformation( const Ogre::MeshPtr mesh, size_t &vertex_count,
                                                     Ogre::Vector3 *&vertices, size_t &index_count,
                                                     unsigned long *&indices,
                                                     const Ogre::Vector3 &position,
                                                     const Ogre::Quaternion &orient,
                                                     const Ogre::Vector3 &scale )
        {
            bool added_shared = false;
            size_t current_offset = 0;
            size_t shared_offset = 0;
            size_t next_offset = 0;
            size_t index_offset = 0;

            vertex_count = index_count = 0;

            // Calculate how many vertices and indices we're going to need
            for( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
            {
                Ogre::SubMesh *submesh = mesh->getSubMesh( i );

                // We only need to add the shared vertices once
                if( submesh->useSharedVertices )
                {
                    if( !added_shared )
                    {
                        vertex_count += mesh->sharedVertexData->vertexCount;
                        added_shared = true;
                    }
                }
                else
                {
                    vertex_count += submesh->vertexData->vertexCount;
                }

                // Add the indices
                index_count += submesh->indexData->indexCount;
            }

            // Allocate space for the vertices and indices
            vertices = new Ogre::Vector3[vertex_count];
            indices = new unsigned long[index_count];

            added_shared = false;

            // Run through the submeshes again, adding the data into the arrays
            for( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
            {
                Ogre::SubMesh *submesh = mesh->getSubMesh( i );

                Ogre::VertexData *vertex_data =
                    submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

                if( ( !submesh->useSharedVertices ) || ( submesh->useSharedVertices && !added_shared ) )
                {
                    if( submesh->useSharedVertices )
                    {
                        added_shared = true;
                        shared_offset = current_offset;
                    }

                    const Ogre::VertexElement *posElem =
                        vertex_data->vertexDeclaration->findElementBySemantic( Ogre::VES_POSITION );

                    Ogre::HardwareVertexBufferSharedPtr vbuf =
                        vertex_data->vertexBufferBinding->getBuffer( posElem->getSource() );

                    auto vertex = static_cast<unsigned char *>(
                        vbuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );

                    // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                    //  as second argument. So make it float, to avoid trouble when Ogre::Real will
                    //  be comiled/typedefed as double:
                    //      Ogre::Real* pReal;
                    float *pReal;

                    for( size_t j = 0; j < vertex_data->vertexCount;
                         ++j, vertex += vbuf->getVertexSize() )
                    {
                        posElem->baseVertexPointerToElement( vertex, &pReal );

                        Ogre::Vector3 pt( pReal[0], pReal[1], pReal[2] );

                        vertices[current_offset + j] = ( orient * ( pt * scale ) ) + position;
                    }

                    vbuf->unlock();
                    next_offset += vertex_data->vertexCount;
                }

                Ogre::IndexData *index_data = submesh->indexData;
                size_t numTris = index_data->indexCount / 3;
                Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
                if( ibuf.isNull() )
                    continue;  // need to check if index buffer is valid (which will be not if the mesh
                // doesn't have triangles like a pointcloud)

                bool use32bitindexes = ( ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );

                auto pLong =
                    static_cast<unsigned long *>( ibuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );
                auto pShort = reinterpret_cast<unsigned short *>( pLong );

                size_t offset = ( submesh->useSharedVertices ) ? shared_offset : current_offset;
                size_t index_start = index_data->indexStart;
                size_t last_index = numTris * 3 + index_start;

                if( use32bitindexes )
                    for( size_t k = index_start; k < last_index; ++k )
                    {
                        indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset );
                    }

                else
                    for( size_t k = index_start; k < last_index; ++k )
                    {
                        indices[index_offset++] = static_cast<unsigned long>( pShort[k] ) +
                                                  static_cast<unsigned long>( offset );
                    }

                ibuf->unlock();
                current_offset = next_offset;
            }
        }

        void CGraphicsSceneOgre::GetMeshInformation( const Ogre::Entity *entity, size_t &vertex_count,
                                                     Ogre::Vector3 *&vertices, size_t &index_count,
                                                     unsigned long *&indices,
                                                     const Ogre::Vector3 &position,
                                                     const Ogre::Quaternion &orient,
                                                     const Ogre::Vector3 &scale )
        {
            bool added_shared = false;
            size_t current_offset = 0;
            size_t shared_offset = 0;
            size_t next_offset = 0;
            size_t index_offset = 0;
            vertex_count = index_count = 0;

            Ogre::MeshPtr mesh = entity->getMesh();

            bool useSoftwareBlendingVertices = entity->hasSkeleton();

            if( useSoftwareBlendingVertices )
            {
                // entity->_updateAnimation();
            }

            // Calculate how many vertices and indices we're going to need
            for( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
            {
                Ogre::SubMesh *submesh = mesh->getSubMesh( i );

                // We only need to add the shared vertices once
                if( submesh->useSharedVertices )
                {
                    if( !added_shared )
                    {
                        vertex_count += mesh->sharedVertexData->vertexCount;
                        added_shared = true;
                    }
                }
                else
                {
                    vertex_count += submesh->vertexData->vertexCount;
                }

                // Add the indices
                index_count += submesh->indexData->indexCount;
            }

            // Allocate space for the vertices and indices
            vertices = new Ogre::Vector3[vertex_count];
            indices = new unsigned long[index_count];

            added_shared = false;

            // Run through the submeshes again, adding the data into the arrays
            for( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i )
            {
                Ogre::SubMesh *submesh = mesh->getSubMesh( i );

                // GET VERTEXDATA

                // Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData :
                // submesh->vertexData;
                Ogre::VertexData *vertex_data;

                // When there is animation:
                if( useSoftwareBlendingVertices )
                    vertex_data = submesh->useSharedVertices
                                      ? entity->_getSkelAnimVertexData()
                                      : entity->getSubEntity( i )->_getSkelAnimVertexData();
                else
                    vertex_data =
                        submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

                if( ( !submesh->useSharedVertices ) || ( submesh->useSharedVertices && !added_shared ) )
                {
                    if( submesh->useSharedVertices )
                    {
                        added_shared = true;
                        shared_offset = current_offset;
                    }

                    const Ogre::VertexElement *posElem =
                        vertex_data->vertexDeclaration->findElementBySemantic( Ogre::VES_POSITION );

                    Ogre::HardwareVertexBufferSharedPtr vbuf =
                        vertex_data->vertexBufferBinding->getBuffer( posElem->getSource() );

                    auto vertex = static_cast<unsigned char *>(
                        vbuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );

                    // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
                    //  as second argument. So make it float, to avoid trouble when Ogre::Real will
                    //  be comiled/typedefed as double:
                    //      Ogre::Real* pReal;
                    float *pReal;

                    for( size_t j = 0; j < vertex_data->vertexCount;
                         ++j, vertex += vbuf->getVertexSize() )
                    {
                        posElem->baseVertexPointerToElement( vertex, &pReal );

                        Ogre::Vector3 pt( pReal[0], pReal[1], pReal[2] );

                        vertices[current_offset + j] = ( orient * ( pt * scale ) ) + position;
                    }

                    vbuf->unlock();
                    next_offset += vertex_data->vertexCount;
                }

                Ogre::IndexData *index_data = submesh->indexData;
                size_t numTris = index_data->indexCount / 3;
                Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

                bool use32bitindexes = ( ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT );

                auto pLong =
                    static_cast<unsigned long *>( ibuf->lock( Ogre::HardwareBuffer::HBL_READ_ONLY ) );
                auto pShort = reinterpret_cast<unsigned short *>( pLong );

                size_t offset = ( submesh->useSharedVertices ) ? shared_offset : current_offset;
                size_t index_start = index_data->indexStart;
                size_t last_index = numTris * 3 + index_start;

                if( use32bitindexes )
                    for( size_t k = index_start; k < last_index; ++k )
                    {
                        indices[index_offset++] = pLong[k] + static_cast<unsigned long>( offset );
                    }

                else
                    for( size_t k = index_start; k < last_index; ++k )
                    {
                        indices[index_offset++] = static_cast<unsigned long>( pShort[k] ) +
                                                  static_cast<unsigned long>( offset );
                    }

                ibuf->unlock();
                current_offset = next_offset;
            }
        }

        SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> CGraphicsSceneOgre::getSceneNodes() const
        {
            return m_sceneNodes;
        }

        void CGraphicsSceneOgre::setSceneNodes(
            SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> sceneNodes )
        {
            m_sceneNodes = sceneNodes;
        }

        SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> CGraphicsSceneOgre::getRegisteredSceneNodes()
            const
        {
            return m_registeredSceneNodes;
        }

        void CGraphicsSceneOgre::setRegisteredSceneNodes(
            SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> sceneNodes )
        {
            m_registeredSceneNodes = sceneNodes;
        }

        unsigned int getMeshByIndex( const Array<unsigned int> &viIndexOffsets,
                                     unsigned int iIndexIndex )
        {
            // subset search
            auto iRangeMin = 0;
            auto iRangeMax = static_cast<s32>( viIndexOffsets.size() ) - 2;
            while( iRangeMin != iRangeMax )
            {
                auto iSamplePoint = ( iRangeMax + iRangeMin ) >> 1;
                if( iIndexIndex < viIndexOffsets[iSamplePoint + 1] )
                    iRangeMax = iSamplePoint;
                else  //>=
                    iRangeMin = iSamplePoint + 1;
            }
            return iRangeMax;
        }

        Array<SmartPtr<IGraphicsMesh>> CGraphicsSceneOgre::splitMesh(
            SmartPtr<IGraphicsMesh> mesh, const SmartPtr<Properties> &properties )
        {
            Array<SmartPtr<IGraphicsMesh>> meshes;

#ifdef FB_OGRE_USE_MESH_SPLITTERS
            Ogre::Entity *entity = nullptr;
            mesh->_getObject( (void **)&entity );

            Ogre::MeshPtr inMesh = entity->getMesh();

            std::string strSourceName = inMesh->getName();
            std::string strDestFolder = "../media/meshsplitoutput/";

            float fMaxSize = 1.0f;
            float fRoughness = 0.0f;
            float fResolution = 256;
            bool bSmooth = true;
            u32 nRecoveryAttempts = 100;
            bool bCutSurface = true;
            String strCutMaterial = "BaseWhiteMaterial";

            if( properties )
            {
                if( !properties->getPropertyValue( "maxSize", fMaxSize ) )
                    LOG_MESSAGE( "Graphics", "Warning: no \"maxSize\" property found. " );

                if( !properties->getPropertyValue( "roughness", fRoughness ) )
                    LOG_MESSAGE( "Graphics", "Warning: no \"roughness\" property found. " );

                if( !properties->getPropertyValue( "recoveryAttempts", nRecoveryAttempts ) )
                    LOG_MESSAGE( "Graphics", "Warning: no \"recoveryAttempts\" property found. " );

                if( !properties->getPropertyValue( "cutMaterial", strCutMaterial ) )
                    LOG_MESSAGE( "Graphics", "Warning: no \"cutMaterial\" property found. " );
            }

            Array<Ogre::MeshPtr> vSplinters = DestructibleMeshSplitter::SplitMesh(
                inMesh, fMaxSize, fRoughness, fResolution, bSmooth, nRecoveryAttempts, bCutSurface,
                strCutMaterial.c_str() );

            // find out which fragments are connected
            Array<Ogre::Vector3> vOverallVertices;
            Array<unsigned int> viOverallVertexOffsets, viOverallMatchingVertices;

            Ogre::MeshSerializer *pSerializer = new Ogre::MeshSerializer();
            Array<Ogre::String> vstrFiles;

            for( unsigned int iSplinter = 0; iSplinter < vSplinters.size(); iSplinter++ )
            {
                // write fragment to disk
                Ogre::String strFileName = strSourceName + Ogre::String( "_" ) +
                                           Ogre::StringConverter::toString( iSplinter ) +
                                           Ogre::String( ".mesh" );
                printf( "writing mesh fragment %s\n", strFileName.c_str() );
                pSerializer->exportMesh( vSplinters[iSplinter].get(), strDestFolder + strFileName );
                vstrFiles.push_back( strFileName );

                Array<Ogre::Vector3> vVertices;
                Array<unsigned int> viSubmeshVertexOffsets, viIndices, viSubmeshIndexOffsets,
                    viMatchingVertices;

                OgreMeshExtractor::Extract( vSplinters[iSplinter], vVertices, viSubmeshVertexOffsets,
                                            viIndices, viSubmeshIndexOffsets );

                DestructibleMeshSplitter::unloadMesh( vSplinters[iSplinter] );

                viOverallVertexOffsets.push_back( vOverallVertices.size() );
                vOverallVertices.insert( vOverallVertices.end(), vVertices.begin(), vVertices.end() );
            }
            delete pSerializer;

            viOverallMatchingVertices = VertexMerger::GetMatchingIndices( vOverallVertices );
            viOverallVertexOffsets.push_back( vOverallVertices.size() );

            Array<std::map<unsigned int, std::set<int>>> vmConnections( vSplinters.size() );
            for( unsigned int iMesh = 0; iMesh < vSplinters.size(); iMesh++ )
            {
                for( unsigned int iVert = viOverallVertexOffsets[iMesh];
                     iVert < viOverallVertexOffsets[iMesh + 1]; iVert++ )
                {
                    // get sub-mesh
                    int iMatchingVert = viOverallMatchingVertices[iVert];
                    unsigned int iOtherMesh = getMeshByIndex( viOverallVertexOffsets, iMatchingVert );
                    if( iOtherMesh != iMesh )
                    {
                        std::map<unsigned int, std::set<int>>::iterator it;
                        if( ( it = vmConnections[iMesh].find( iOtherMesh ) ) !=
                            vmConnections[iMesh].end() )
                            it->second.insert( iMatchingVert );
                        else
                        {
                            std::set<int> mySet;
                            mySet.insert( iMatchingVert );
                            vmConnections[iMesh][iOtherMesh] = mySet;
                        }
                    }
                }
            }
            // write XML output
            std::string strXMLFile = strDestFolder + strSourceName + Ogre::String( ".xml" );
            printf( "writing XML file to %s\n", strXMLFile.c_str() );
            std::ofstream ofs( strXMLFile.c_str() );
            ofs << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n";
            ofs << "<mesh_fragments>\n";
            for( unsigned int iFragment = 0; iFragment < vmConnections.size(); iFragment++ )
            {
                ofs << "\t<mesh name=\"" << vstrFiles[iFragment].c_str() << "\">\n";
                //;
                for( std::map<unsigned int, std::set<int>>::iterator it =
                         vmConnections[iFragment].begin();
                     it != vmConnections[iFragment].end(); it++ )
                {
                    ofs << "\t\t<connection target=\"" << vstrFiles[it->first].c_str() << "\">\n";
                    const std::set<int> &siVertices = it->second;

                    Ogre::Vector3 vMin( 1.0e+10f, 1.0e+10f, 1.0e+10f ),
                        vMax( -1.0e+10f, -1.0e+10f, -1.0e+10f );
                    for( std::set<int>::const_iterator itVert = siVertices.begin();
                         itVert != siVertices.end(); itVert++ )
                    {
                        Ogre::Vector3 v = vOverallVertices[*itVert];
                        vMin.makeFloor( v );
                        vMax.makeCeil( v );
                    }
                    float fSize = ( vMax - vMin ).length();
                    ofs << "\t\t\t<rel_force value=\"" << fSize * fSize << "\"/>\n";
                    ofs << "\t\t\t<rel_torque value=\"" << fSize * fSize * fSize << "\"/>\n";
                    Ogre::Vector3 vJointCenter = vMin + ( vMax - vMin ) * .5f;
                    ofs << "\t\t\t<joint_center x=\"" << vJointCenter.x << "\" y=\"" << vJointCenter.y
                        << "\" z=\"" << vJointCenter.z << "\"/>\n";

                    ofs << "\t\t</connection>\n";
                }
                ofs << "\t</mesh>\n";
            }
            ofs << "</mesh_fragments>";
            ofs.close();

            for( size_t i = 0; i < vSplinters.size(); ++i )
            {
                Ogre::MeshPtr mesh_ = vSplinters[i];
                mesh_->load();

                Ogre::Entity *entity_ = m_sceneManager->createEntity( mesh_ );

                String name = entity_->getName().c_str();
                SmartPtr<CGraphicsMeshOgre> graphicsObject( new CGraphicsMeshOgre( this ), this );
                graphicsObject->initialise( entity );
                graphicsObject->getHandle()->setName( name );
                m_graphicsObjects[name] = graphicsObject.get();
                meshes.push_back( graphicsObject );
            }
#endif

            return meshes;
        }

        bool CGraphicsSceneOgre::getEnableShadows() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<SceneManagerState>() )
                {
                    return state->getEnableShadows();
                }
            }

            return false;
        }

        void CGraphicsSceneOgre::setEnableShadows( bool enableShadows, bool depthShadows /*= true*/ )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<SceneManagerState>() )
                {
                    return state->setEnableShadows( enableShadows );
                }
            }

            return;

            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            using namespace Ogre;

            if( enableShadows )
            {
                // General scene setup
                m_sceneManager->setShadowTechnique( SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED );
                m_sceneManager->setShadowFarDistance( 10 );

                // 3 textures per directional light (PSSM)
                m_sceneManager->setShadowTextureCountPerLightType( Light::LT_DIRECTIONAL, 3 );

                // shadow camera setup
                auto pssmSetup = new PSSMShadowCameraSetup();
                pssmSetup->setSplitPadding( MathF::epsilon() );
                pssmSetup->calculateSplitPoints( 3, MathF::epsilon(),
                                                 m_sceneManager->getShadowFarDistance(), 0.5f );
                pssmSetup->setOptimalAdjustFactor( 0, 2 );
                pssmSetup->setOptimalAdjustFactor( 1, 1 );
                pssmSetup->setOptimalAdjustFactor( 2, 0.5 );
                pssmSetup->setUseAggressiveFocusRegion( true );
                pssmSetup->setUseSimpleOptimalAdjust( false );

                const PSSMShadowCameraSetup::SplitPointList &splitList = pssmSetup->getSplitPoints();
                for( size_t i = 0; i < splitList.size(); ++i )
                {
                    Ogre::Real splitValue = splitList[i];
                    splitValue = splitValue;
                }

                m_sceneManager->setShadowCameraSetup( ShadowCameraSetupPtr( pssmSetup ) );

                if( depthShadows )
                {
                    m_sceneManager->setShadowTextureCount( 3 );

                    auto shadowTextureSize = 1024;

                    m_sceneManager->setShadowTextureConfig( 0, shadowTextureSize, shadowTextureSize,
                                                            PF_FLOAT32_GR );
                    m_sceneManager->setShadowTextureConfig( 1, shadowTextureSize * 0.5,
                                                            shadowTextureSize * 0.5, PF_FLOAT32_GR );
                    m_sceneManager->setShadowTextureConfig( 2, shadowTextureSize * 0.25,
                                                            shadowTextureSize * 0.25, PF_FLOAT32_GR );

                    m_sceneManager->setShadowTextureSelfShadow( true );
                    m_sceneManager->setShadowCasterRenderBackFaces( true );
                    // m_sceneManager->setShadowTextureCasterMaterial("PSSM/shadow_caster");
                }
                else
                {
                    m_sceneManager->setShadowTextureCount( 3 );
                    m_sceneManager->setShadowTextureConfig( 0, 2048, 2048, PF_X8B8G8R8 );
                    m_sceneManager->setShadowTextureConfig( 1, 1024, 1024, PF_X8B8G8R8 );
                    m_sceneManager->setShadowTextureConfig( 2, 1024, 1024, PF_X8B8G8R8 );
                    m_sceneManager->setShadowTextureSelfShadow( false );
                    m_sceneManager->setShadowCasterRenderBackFaces( false );
                    // m_sceneManager->setShadowTextureCasterMaterial(Ogre::StringUtil::BLANK);
                }

                // addTextureShadowDebugOverlay(TL_RIGHT, 3);

                u32 maskVal = VIEWPORT_MASK_SHADOW;
                /*
                for (unsigned i = 0; i < numShadowRTTs; ++i)
                {
                    Ogre::TexturePtr tex = m_sceneManager->getShadowTexture(i);
                    Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);

                    std::string compositorName = "shadow_clear_" +
                Ogre::StringConverter::toString((int)i);

                    Ogre::TextureManager* textureMgr =  Ogre::TextureManager::getSingletonPtr();

                    Ogre::TexturePtr t =  textureMgr->createManual("shadow_" +
                Ogre::StringConverter::toString((int)i),
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 2048, 2048, 0,
                PF_FLOAT32_GR, TU_DYNAMIC);

                    t->load(true);

                    Ogre::PixelFormat format = t->getFormat();
                    if (format != Ogre::PF_FLOAT32_GR)
                    {
                        Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Pixel format is not
                PF_FLOAT32_GR which is what was requested!");
                    }

                    Ogre::CompositorManager::getSingleton().addCompositor(vp, compositorName);
                    Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositorName,
                true);
                }*/

                /*
                for (unsigned i = 0; i < numShadowRTTs; ++i)
                {
                    Ogre::TexturePtr tex = m_sceneManager->getShadowTexture(i);
                    Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);

                    std::string compositorName = "shadow_clear_" +
                Ogre::StringConverter::toString((int)i);

                    Ogre::TextureManager* textureMgr =  Ogre::TextureManager::getSingletonPtr();

                    Ogre::TexturePtr t =  textureMgr->createManual("shadow_" +
                Ogre::StringConverter::toString((int)i),
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 2048, 2048, 0,
                PF_FLOAT32_GR, TU_DYNAMIC);

                    t->load(true);

                    Ogre::PixelFormat format = t->getFormat();
                    if (format != Ogre::PF_FLOAT32_GR)
                    {
                        Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: Pixel format is not
                PF_FLOAT32_GR which is what was requested!");
                    }

                    Ogre::CompositorManager::getSingleton().addCompositor(vp, compositorName);
                    Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositorName,
                true);
                }
                */

                // addTextureShadowDebugOverlay(0, 3);

                // Ogre::OverlayManager* overlayMgr = Ogre::OverlayManager::getSingletonPtr();
                // Ogre::Overlay* overlay = overlayMgr->getByName("ShadowDebug");
                // overlay->show();
            }
            else
            {
                m_sceneManager->setShadowTechnique( SHADOWTYPE_NONE );
            }

            const unsigned numShadowRTTs = m_sceneManager->getShadowTextureCount();
            for( unsigned i = 0; i < numShadowRTTs; ++i )
            {
                TexturePtr tex = m_sceneManager->getShadowTexture( i );
                Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport( 0 );
                vp->setBackgroundColour( ColourValue::Black );

                vp->setAutoUpdated( true );
                vp->setClearEveryFrame( false );
                vp->setSkiesEnabled( false );
                vp->setVisibilityMask( VIEWPORT_MASK_SHADOW );

                // tex->getBuffer()->getRenderTarget()->addListener(new ShadowRenderTargetListener(tex));

                auto renderer = new SSEffectRenderer( m_sceneManager, vp );
                renderer->quad()->setVisibilityFlags( VIEWPORT_MASK_SHADOW );
                renderer->quad()->setVisible( false );

                auto effectClear = new SSEffectClear( renderer );
                effectClear->create( tex.get() );

                Root::getSingletonPtr()->addFrameListener(
                    new ShadowFrameListener( effectClear, tex, 0 ) );
            }

            /*
            for (unsigned i = 0; i < numShadowRTTs; ++i)
            {
                Ogre::TexturePtr tex = m_sceneManager->getShadowTexture(i);
                Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);

                Ogre::CompositorManager::getSingleton().addCompositor(vp, "blur_shadow");
                Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, "blur_shadow", true);
            }
            */

            /*
            u32 numShadowRTTs = m_sceneManager->getShadowTextureCount();
            for (size_t i = 0; i < numShadowRTTs; ++i)
            {
                Ogre::TexturePtr shadowTex = m_sceneManager->getShadowTexture(i);
                addTextureDebugOverlay(loc, shadowTex->getName(), i);
            }
            */

            // LightShafts* lightShafts = new LightShafts(this, nullptr);
            // lightShafts->createScene();
        }

        void CGraphicsSceneOgre::addExistingGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject )
        {
            m_graphicsObjects.push_back( graphicsObject );
        }

        void CGraphicsSceneOgre::addExistingSceneNode( SmartPtr<ISceneNode> sceneNode )
        {
            m_sceneNodes->push_back( sceneNode );
        }

        SmartPtr<IInstanceManager> CGraphicsSceneOgre::createInstanceManager(
            const String &customName, const String &meshName, const String &groupName, u32 technique,
            u32 numInstancesPerBatch, u16 flags /*=0*/, u16 subMeshIdx /*=0 */ )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );

            Ogre::InstanceManager *mgr = m_sceneManager->createInstanceManager(
                customName.c_str(), meshName.c_str(),
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                static_cast<Ogre::InstanceManager::InstancingTechnique>( technique ), 80,
                Ogre::IM_USE16BIT, subMeshIdx );

            SmartPtr<CInstanceManager> instanceManager;  // (new CInstanceManager, true);
            m_instanceManagers.push_back( instanceManager );
            instanceManager->setInstanceManager( mgr );
            return instanceManager;
        }

        SmartPtr<IInstancedObject> CGraphicsSceneOgre::createInstancedObject( const String &materialName,
                                                                              const String &managerName )
        {
            SmartPtr<CInstancedObject>
                instancedObject;  // (new CInstancedObject(this, materialName, managerName), true);
            _addGraphicsObject( instancedObject );
            return instancedObject;
        }

        void CGraphicsSceneOgre::destroyInstancedObject( SmartPtr<IInstancedObject> instancedObject )
        {
        }

        String CGraphicsSceneOgre::getType() const
        {
            return m_type;
        }

        void CGraphicsSceneOgre::_addGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject )
        {
            //SpinRWMutex::ScopedLock lock( GraphicsObjectMutex );
            m_graphicsObjects.push_back( graphicsObject );
        }

        Array<SmartPtr<IGraphicsObject>> CGraphicsSceneOgre::_getGraphicsObjects() const
        {
            //SpinRWMutex::ScopedLock lock( GraphicsObjectMutex );
            return m_graphicsObjects;
        }

        Array<SmartPtr<IBillboardSet>> CGraphicsSceneOgre::getBillboardSets() const
        {
            Array<SmartPtr<IBillboardSet>> billboardSets;
            return billboardSets;
        }

        SmartPtr<ITerrain> CGraphicsSceneOgre::getTerrain( u32 id ) const
        {
            return m_terrains[id];
        }

        void CGraphicsSceneOgre::setAnimationNameSuffix( const String &animationNameSuffix )
        {
            m_animationNameSuffix = animationNameSuffix;
        }

        String CGraphicsSceneOgre::getAnimationNameSuffix() const
        {
            return m_animationNameSuffix;
        }

        void CGraphicsSceneOgre::setAnimationNamePrefix( const String &animationNamePrefix )
        {
            m_animationNamePrefix = animationNamePrefix;
        }

        String CGraphicsSceneOgre::getAnimationNamePrefix() const
        {
            return m_animationNamePrefix;
        }

        CGraphicsSceneOgre::SceneManagerStateListener::SceneManagerStateListener()
        {
        }

        CGraphicsSceneOgre::SceneManagerStateListener::~SceneManagerStateListener()
        {
        }

        void CGraphicsSceneOgre::SceneManagerStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            Ogre::SceneManager *smgr = nullptr;
            m_owner->_getObject( (void **)&smgr );

            if( smgr )
            {
                try
                {
#if _DEBUG
                    auto task = Thread::getCurrentTask();
                    auto renderTask =
                        core::IApplicationManager::instance()->getGraphicsSystem()->getRenderTask();
                    FB_ASSERT( task == renderTask );
#endif

                    auto sceneManagerState = fb::static_pointer_cast<SceneManagerState>( state );
                    if( sceneManagerState )
                    {
                        if( sceneManagerState->getEnableSkybox() )
                        {
                            auto skyboxMaterial = m_owner->getSkyboxMaterial();
                            //FB_ASSERT(skyboxMaterial);

                            if( auto pMaterial =
                                    fb::static_pointer_cast<CMaterialOgre>( skyboxMaterial ) )
                            {
                                auto ogreMaterial = pMaterial->getMaterial();
                                //FB_ASSERT(ogreMaterial);

                                if( ogreMaterial )
                                {
                                    ogreMaterial->load();

                                    auto materialName = ogreMaterial->getName();
                                    smgr->setSkyBox( true, materialName );
                                }
                            }
                        }
                        else
                        {
                            smgr->setSkyBox( false, "" );
                        }

                        auto ambientLight =
                            OgreUtil::convertToOgre( sceneManagerState->getAmbientLight() );
                        if( !OgreUtil::equals( ambientLight, smgr->getAmbientLight() ) )
                        {
                            smgr->setAmbientLight( ambientLight );
                        }

                        auto enableShadows = sceneManagerState->getEnableShadows();
                        if( enableShadows )
                        {
                            smgr->setShadowTechnique( Ogre::SHADOWTYPE_NONE );
                            smgr->setShadowFarDistance( 100.0f );
                        }
                        else
                        {
                            smgr->setShadowTechnique( Ogre::SHADOWTYPE_NONE );
                        }
                    }

                    state->setDirty( false );
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }
        }

        void CGraphicsSceneOgre::SceneManagerStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( message->isDerived<StateMessageSkyBox>() )
            {
                auto messageSkyBox = fb::static_pointer_cast<StateMessageSkyBox>( message );
                auto enable = messageSkyBox->getEnable();
                auto material = messageSkyBox->getMaterial();

                m_owner->setSkyBox( enable, material );
            }
        }

        void CGraphicsSceneOgre::SceneManagerStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CGraphicsSceneOgre *CGraphicsSceneOgre::SceneManagerStateListener::getOwner() const
        {
            return m_owner;
        }

        void CGraphicsSceneOgre::SceneManagerStateListener::setOwner( CGraphicsSceneOgre *owner )
        {
            m_owner = owner;
        }

        CGraphicsSceneOgre::RootSceneNode::~RootSceneNode()
        {
            unload( nullptr );
            destroyStateContext();
        }

        void CGraphicsSceneOgre::RootSceneNode::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loaded );
        }

        void CGraphicsSceneOgre::RootSceneNode::unload( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Unloading );
            m_sceneNode = nullptr;
            setLoadingState( LoadingState::Unloaded );
        }

        void CGraphicsSceneOgre::MaterialSharedListener::unload( SmartPtr<ISharedObject> data )
        {
            m_owner = nullptr;
        }

        Parameter CGraphicsSceneOgre::MaterialSharedListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            return Parameter();
        }

        void CGraphicsSceneOgre::MaterialSharedListener::loadingStateChanged(
            ISharedObject *sharedObject, LoadingState oldState, LoadingState newState )
        {
            if( newState == LoadingState::Loaded )
            {
                m_owner->setSkyBox( m_owner->getEnableSkybox(), m_owner->m_skyboxMaterial );
            }
        }

        bool CGraphicsSceneOgre::MaterialSharedListener::destroy( void *ptr )
        {
            return false;
        }

        SmartPtr<CGraphicsSceneOgre> CGraphicsSceneOgre::MaterialSharedListener::getOwner() const
        {
            return m_owner.lock();
        }

        void CGraphicsSceneOgre::MaterialSharedListener::setOwner( SmartPtr<CGraphicsSceneOgre> owner )
        {
            m_owner = owner;
        }

        CGraphicsSceneOgre::SkyboxStateListener::SkyboxStateListener()
        {
        }

        CGraphicsSceneOgre::SkyboxStateListener::~SkyboxStateListener()
        {
        }

        void CGraphicsSceneOgre::SkyboxStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void CGraphicsSceneOgre::SkyboxStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto stateObject = m_owner->getStateObject() )
            {
                stateObject->setDirty( true );
            }
        }

        void CGraphicsSceneOgre::SkyboxStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CGraphicsSceneOgre *CGraphicsSceneOgre::SkyboxStateListener::getOwner() const
        {
            return m_owner;
        }

        void CGraphicsSceneOgre::SkyboxStateListener::setOwner( CGraphicsSceneOgre *owner )
        {
            m_owner = owner;
        }
    }  // end namespace render
}  // end namespace fb
