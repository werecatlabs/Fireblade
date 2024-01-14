#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneNodeOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsMeshOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CLightOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CParticleSystem.h>
#include <FBGraphicsOgreNext/Wrapper/CAnimationStateController.h>
#include <FBGraphicsOgreNext/Wrapper/CAnimationTextureControl.h>
#include <FBGraphicsOgreNext/Wrapper/CBillboardSetOgre.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CDecalCursor.h>
#include <FBGraphicsOgreNext/Wrapper/CDynamicLines.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSystemOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTerrainOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CViewportOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSkyboxOgreNext.h>
#include <FBGraphicsOgreNext/ShadowFrameListener.h>
#include <FBGraphicsOgreNext/MeshLoader.h>
#include <FBGraphicsOgreNext/OgreUtil.h>
#include <FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h>
#include <FBGraphicsOgreNext/CompositorManager.h>
#include <FBGraphicsOgreNext/Compositor.h>
#include <FBCore/FBCore.h>

#ifdef FB_OGRE_USE_MESH_SPLITTERS
#    include "DestructibleMeshSplitter.h"
#    include "OgreMeshExtractor.h"
#    include "VertexMerger.h"
#endif

#include <Ogre.h>
#include <OgreImage2.h>
#include <OgreItem.h>
#include <OgreMeshManager.h>
#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include <OgreSceneManager.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CSceneManagerOgreNext, IGraphicsScene );

    u32 CSceneManagerOgreNext::m_nextGeneratedNameExt = 0;

    CSceneManagerOgreNext::CSceneManagerOgreNext() :

        nextRenderQueueUpdate( 0 ),
        m_isClearing( false )
    {
        try
        {
            if( auto handle = getHandle() )
            {
                handle->setName( "CSceneManagerOgreNext" );
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto rootSceneNode = fb::make_ptr<RootSceneNode>();
            rootSceneNode->setCreator( this );
            m_rootSceneNode = rootSceneNode;

            // m_registeredSceneNodes.reserve(500);
            // m_registeredGfxObjects.reserve(500);

            m_animationNamePrefix = StringUtil::EmptyString;
            m_animationNameSuffix = StringUtil::EmptyString;

            m_sceneNodes = boost::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>();
            m_registeredSceneNodes = boost::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>();

            m_state = fb::make_ptr<SceneManagerState>();

            m_stateContext = stateManager->addStateObject();
            m_stateContext->addState( m_state );

            auto sceneManagerStateListener = factoryManager->make_ptr<SceneManagerStateListener>();
            sceneManagerStateListener->setOwner( this );
            m_stateListener = sceneManagerStateListener;
            m_stateContext->addStateListener( m_stateListener );

            //m_state->setStateContext( m_stateContext );

            auto renderTask =
                applicationManager->hasTasks() ? Thread::Task::Render : Thread::Task::Primary;
            m_state->setTaskId( renderTask );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    CSceneManagerOgreNext::CSceneManagerOgreNext( Ogre::SceneManager *sceneManager ) :

        nextRenderQueueUpdate( 0 ),
        m_isClearing( false )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto stateManager = applicationManager->getStateManager();
            auto threadPool = applicationManager->getThreadPool();

            m_sceneManager = sceneManager;

            auto rootSceneNode = fb::make_ptr<RootSceneNode>();
            rootSceneNode->setCreator( this );
            m_rootSceneNode = rootSceneNode;

            // m_registeredSceneNodes.reserve(500);
            // m_registeredGfxObjects.reserve(500);

            m_animationNamePrefix = StringUtil::EmptyString;
            m_animationNameSuffix = StringUtil::EmptyString;

            m_state = fb::make_ptr<SceneManagerState>();

            m_stateContext = stateManager->addStateObject();
            m_stateContext->addState( m_state );

            auto sceneManagerStateListener = factoryManager->make_ptr<SceneManagerStateListener>();
            sceneManagerStateListener->setOwner( this );
            m_stateListener = sceneManagerStateListener;
            m_stateContext->addStateListener( m_stateListener );

            //m_state->setStateContext( m_stateContext );

            auto renderTask =
                applicationManager->hasTasks() ? Thread::Task::Render : Thread::Task::Primary;
            m_state->setTaskId( renderTask );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    CSceneManagerOgreNext::~CSceneManagerOgreNext()
    {
        unload( nullptr );

        if( m_rootSceneNode )
        {
            m_rootSceneNode->unload( nullptr );
            m_rootSceneNode = nullptr;
        }
    }

    void CSceneManagerOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            //#if _DEBUG
            //				auto task = Thread::getCurrentTask();
            //				auto renderTask =
            // IApplicationManager::instance()->getGraphicsSystem()->getRenderTask();
            // FB_ASSERT(task
            //== renderTask); #endif

            setLoadingState( LoadingState::Loading );

            auto root = Ogre::Root::getSingletonPtr();
            FB_ASSERT( root );

            auto type = getType();
            auto name = getName();

            FB_ASSERT( !StringUtil::isNullOrEmpty( type ) );
            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

            // auto id = StringUtil::getHash(name);

            auto numThreads = 0;
            auto sceneMgr = root->createSceneManager( Ogre::ST_GENERIC, numThreads, name );
            FB_ASSERT( sceneMgr );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto graphicsSystem = fb::static_pointer_cast<CGraphicsSystemOgreNext>(
                applicationManager->getGraphicsSystem() );
            FB_ASSERT( graphicsSystem );

            auto overlaySystem = graphicsSystem->getOverlaySystem();
            auto overlayManager = Ogre::v1::OverlayManager::getSingletonPtr();

            FB_ASSERT( overlaySystem );
            FB_ASSERT( overlayManager );

            sceneMgr->addRenderQueueListener( overlaySystem );
            auto sceneManagerRenderQueue = sceneMgr->getRenderQueue();
            FB_ASSERT( sceneManagerRenderQueue );

            sceneManagerRenderQueue->setSortRenderQueue( overlayManager->mDefaultRenderQueueId,
                                                         Ogre::RenderQueue::StableSort );

            m_sceneManager = sceneMgr;

            //m_sceneManager->setAmbientLight( Ogre::ColourValue::White * 0.4f,
            //                                 Ogre::ColourValue::White * 0.4f,
            //                                 Ogre::Vector3::UNIT_Y );

            m_sceneManager->setAmbientLight( Ogre::ColourValue::White * 0.85f,
                                             Ogre::ColourValue::White * 0.3f, Ogre::Vector3::UNIT_Y,
                                             1.0f );

            auto rootNode = sceneMgr->getRootSceneNode();
            FB_ASSERT( rootNode );

            auto rootSceneNode = fb::static_pointer_cast<CSceneNodeOgreNext>( m_rootSceneNode );
            FB_ASSERT( rootSceneNode );
            rootSceneNode->initialise( rootNode );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CSceneManagerOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            auto stateManager = applicationManager->getStateManager();

            if( m_skybox )
            {
                m_skybox->unload( nullptr );
                m_skybox = nullptr;
            }

            if( m_stateContext )
            {
                if( m_stateListener )
                {
                    m_stateContext->removeStateListener( m_stateListener );
                }

                stateManager->removeStateObject( m_stateContext );

                m_stateContext->unload( nullptr );
                m_stateContext = nullptr;
            }

            if( m_stateListener )
            {
                m_stateListener->unload( nullptr );
                m_stateListener = nullptr;
            }

            if( m_state )
            {
                m_state->setOwner( nullptr );
                m_state->unload( nullptr );
                m_state = nullptr;
            }

            m_camera = nullptr;

            for( auto obj : m_graphicsObjects )
            {
                obj->unload( nullptr );
            }

            m_graphicsObjects.clear();

            if( m_rootSceneNode )
            {
                m_rootSceneNode->removeChildren();
            }

            m_registeredSceneNodes = nullptr;
            m_registeredGfxObjects.clear();

            if( m_sceneNodes )
            {
                auto pSceneNodes = m_sceneNodes;
                auto &sceneNodes = *pSceneNodes;
                for( auto sceneNode : sceneNodes )
                {
                    sceneNode->detachAllObjects();
                }

                for( auto sceneNode : sceneNodes )
                {
                    sceneNode->unload( nullptr );
                }

                m_sceneNodes = nullptr;
            }

            if( m_rootSceneNode )
            {
                m_rootSceneNode->removeChildren();
            }

            setSceneNodes( nullptr );

            if( m_rootSceneNode )
            {
                m_rootSceneNode->unload( nullptr );
                m_rootSceneNode = nullptr;
            }

            if( m_sceneManager )
            {
                m_sceneManager->clearScene( true );
                m_sceneManager = nullptr;
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CSceneManagerOgreNext::update()
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

    void CSceneManagerOgreNext::postUpdate()
    {
        for( auto terrain : m_terrains )
        {
            if( terrain )
            {
                terrain->postUpdate();
            }
        }
    }

    void CSceneManagerOgreNext::setType( const String &val )
    {
        m_type = val;
    }

    auto CSceneManagerOgreNext::getName() const -> String
    {
        return m_name;
    }

    void CSceneManagerOgreNext::setName( const String &val )
    {
        m_name = val;
    }

    auto CSceneManagerOgreNext::getStateContext() const -> SmartPtr<IStateContext>
    {
        return m_stateContext;
    }

    void CSceneManagerOgreNext::setStateContext( SmartPtr<IStateContext> stateContext )
    {
        m_stateContext = stateContext;
    }

    auto CSceneManagerOgreNext::addGraphicsObjectByTypeId( u32 id )
        -> fb::SmartPtr<fb::render::IGraphicsObject>
    {
        return nullptr;
    }

    auto CSceneManagerOgreNext::addGraphicsObject( const String &name, const String &type )
        -> SmartPtr<IGraphicsObject>
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
            SmartPtr<CDynamicLines> dynamicLines;  // (new CDynamicLines(this), true);
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

    auto CSceneManagerOgreNext::addGraphicsObject( const String &type ) -> SmartPtr<IGraphicsObject>
    {
        auto name = getUniqueName( type );
        return addGraphicsObject( name, type );
    }

    void CSceneManagerOgreNext::clearScene()
    {
        try
        {
            m_isClearing = true;

            m_registeredSceneNodes = nullptr;
            m_registeredGfxObjects.clear();

            auto pSceneNodes = m_sceneNodes;
            auto &sceneNodes = *pSceneNodes;
            for( auto sceneNode : sceneNodes )
            {
                sceneNode->unload( nullptr );
            }

            m_rootSceneNode->removeChildren();

            m_sceneNodes = nullptr;

            if( m_sceneManager )
            {
                m_sceneManager->clearScene( true );
            }

            m_isClearing = false;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CSceneManagerOgreNext::hasAnimation( const String &animationName ) -> bool
    {
        if( m_sceneManager )
        {
            return m_sceneManager->hasAnimation( animationName.c_str() );
        }

        return false;
    }

    auto CSceneManagerOgreNext::destroyAnimation( const String &animationName ) -> bool
    {
        if( m_sceneManager->hasAnimation( animationName.c_str() ) )
        {
            m_sceneManager->destroyAnimation( animationName.c_str() );
            return true;
        }

        return false;
    }

    void CSceneManagerOgreNext::setAmbientLight( const ColourF &colour )
    {
        m_state->setAmbientLight( colour );
    }

    auto CSceneManagerOgreNext::validateGfxObjName( const String &name ) const -> bool
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

    auto CSceneManagerOgreNext::addCamera( const String &name ) -> SmartPtr<ICamera>
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );
            FB_ASSERT( validateGfxObjName( name ) );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto camera = factoryManager->make_ptr<CCameraOgreNext>();
            camera->setCreator( this );

            auto handle = camera->getHandle();
            FB_ASSERT( handle );

            handle->setName( name );

            m_graphicsObjects.emplace_back( camera );

            graphicsSystem->loadObject( camera );

            if( !m_camera )
            {
                m_camera = camera;
            }

            return camera;
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_ERROR( e.getFullDescription() );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CSceneManagerOgreNext::getCamera( const String &name ) -> SmartPtr<ICamera>
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

    auto CSceneManagerOgreNext::getCamera() const -> SmartPtr<ICamera>
    {
        return m_camera;
    }

    auto CSceneManagerOgreNext::getActiveCamera() const -> SmartPtr<ICamera>
    {
        return m_activeCamera;
    }

    void CSceneManagerOgreNext::setActiveCamera( SmartPtr<ICamera> camera )
    {
        m_activeCamera = camera;
    }

    auto CSceneManagerOgreNext::hasCamera( const String &name ) -> bool
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

    auto CSceneManagerOgreNext::addLight( const String &name ) -> SmartPtr<ILight>
    {
        FB_ASSERT( validateGfxObjName( name ) );

        auto light = SmartPtr<CLightOgreNext>( new CLightOgreNext( this ) );

        auto handle = light->getHandle();
        handle->setName( name );

        m_graphicsObjects.emplace_back( light );

        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        graphicsSystem->loadObject( light );

        return light;
    }

    auto CSceneManagerOgreNext::getLight( const String &name ) const -> SmartPtr<ILight>
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

    auto CSceneManagerOgreNext::getSceneNode( const String &name ) const -> SmartPtr<ISceneNode>
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

    auto CSceneManagerOgreNext::getSceneNodeById( hash32 id ) const -> SmartPtr<ISceneNode>
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

    auto CSceneManagerOgreNext::getRootSceneNode() const -> SmartPtr<ISceneNode>
    {
        return m_rootSceneNode;
    }

    auto CSceneManagerOgreNext::addMesh( const String &name, const String &meshName )
        -> SmartPtr<IGraphicsMesh>
    {
        try
        {
#ifndef _FINAL_
            validateGfxObjName( name );
#endif
            // Ogre::Entity* entity = m_sceneManager->createEntity(name.c_str(), meshName.c_str());
            SmartPtr<CGraphicsMeshOgreNext> graphicsObject( new CGraphicsMeshOgreNext( this ) );
            // graphicsObject->initialise(entity);
            graphicsObject->getHandle()->setName( name );
            // m_graphicsObjects[name] = graphicsObject.get();
            graphicsObject->setMeshName( meshName );

            _addGraphicsObject( graphicsObject );

            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            graphicsSystem->loadObject( graphicsObject );

            return graphicsObject;
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_MESSAGE( "Graphics", e.getFullDescription().c_str() );
        }

        return nullptr;
    }

    auto CSceneManagerOgreNext::addMesh( const String &meshName ) -> SmartPtr<IGraphicsMesh>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto name = getUniqueName( "Mesh" );

            auto graphicsObject = factoryManager->make_ptr<CGraphicsMeshOgreNext>();
            graphicsObject->setCreator( this );

            auto handle = graphicsObject->getHandle();
            if( handle )
            {
                handle->setName( name );
            }

            graphicsObject->setMeshName( meshName );

            _addGraphicsObject( graphicsObject );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            graphicsSystem->loadObject( graphicsObject );

            return graphicsObject;
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_ERROR( e.getFullDescription().c_str() );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto CSceneManagerOgreNext::createV1Mesh( const String &meshName ) -> SmartPtr<IGraphicsMesh>
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            auto meshExtention = Path::getFileExtension( meshName );
            static const auto engineMeshExtention = String( ".fbmeshbin" );
            if( meshExtention == engineMeshExtention )
            {
                auto resourceGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

                auto meshManagerV2 = Ogre::MeshManager::getSingletonPtr();
                auto meshResult = meshManagerV2->createOrRetrieve( meshName, resourceGroupName );
                if( meshResult.second )
                {
                    auto newMesh = meshResult.first.dynamicCast<Ogre::Mesh>();

                    auto name = getUniqueName( "Mesh" );
                    auto entity = m_sceneManager->createItem( newMesh );

                    SmartPtr<CGraphicsMeshOgreNext> graphicsObject( new CGraphicsMeshOgreNext( this ) );
                    graphicsObject->setItem( entity );

                    _addGraphicsObject( graphicsObject );
                    return graphicsObject;
                }
                auto newMesh = meshManagerV2->createManual( meshName, resourceGroupName );
                MeshLoader::loadFBMesh( newMesh, meshName );

                auto name = getUniqueName( "Mesh" );
                auto entity = m_sceneManager->createItem( newMesh );

                SmartPtr<CGraphicsMeshOgreNext> graphicsObject( new CGraphicsMeshOgreNext( this ) );
                graphicsObject->setItem( entity );

                _addGraphicsObject( graphicsObject );
                return graphicsObject;
            }
            auto meshManagerV1 = Ogre::v1::MeshManager::getSingletonPtr();
            auto resourceGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

            auto meshResult = meshManagerV1->createOrRetrieve( meshName, resourceGroupName );
            if( meshResult.second )
            {
                auto planeMeshV1 = meshResult.first.dynamicCast<Ogre::v1::Mesh>();

                auto meshManagerV2 = Ogre::MeshManager::getSingletonPtr();
                auto planeMesh = meshManagerV2->createManual( meshName, resourceGroupName );
                planeMesh->importV1( planeMeshV1.get(), true, true, true );

                auto name = getUniqueName( "Mesh" );
                auto entity = m_sceneManager->createItem( planeMesh );
                entity->setDatablock( "Marble" );

                SmartPtr<CGraphicsMeshOgreNext> graphicsObject( new CGraphicsMeshOgreNext( this ) );
                // graphicsObject->initialise(entity);
                graphicsObject->setItem( entity );

                // String entityName = entity->getName().c_str();
                // graphicsObject->getHandle()->setName(entityName);
                _addGraphicsObject( graphicsObject );
                return graphicsObject;
            }
            auto meshManagerV2 = Ogre::MeshManager::getSingletonPtr();
            auto mesh = meshManagerV2->load( meshName, resourceGroupName );

            auto name = getUniqueName( "Mesh" );
            auto entity = m_sceneManager->createItem( mesh );

            SmartPtr<CGraphicsMeshOgreNext> graphicsObject( new CGraphicsMeshOgreNext( this ) );
            graphicsObject->setItem( entity );

            // String entityName = entity->getName().c_str();
            // graphicsObject->getHandle()->setName(entityName);
            _addGraphicsObject( graphicsObject );
            return graphicsObject;
        }
        catch( Ogre::Exception &e )
        {
            FB_LOG_ERROR( e.getFullDescription().c_str() );
        }

        return nullptr;
    }

    auto CSceneManagerOgreNext::getMesh( const String &name ) const -> SmartPtr<IGraphicsMesh>
    {
        // GraphicsObjects::const_iterator it = m_graphicsObjects.find(name);
        // if( it != m_graphicsObjects.end() )
        //{
        //	return it->second;
        // }

        return nullptr;
    }

    auto CSceneManagerOgreNext::addBillboardSet( const String &name, u32 poolSize )
        -> SmartPtr<IBillboardSet>
    {
#ifndef _FINAL_
        validateGfxObjName( name );
#endif

        Ogre::v1::BillboardSet *bbSet;  // = m_sceneManager->createBillboardSet(name.c_str(), poolSize);

        auto billboardSet = SmartPtr<CBillboardSetOgre>( new CBillboardSetOgre( this ) );
        billboardSet->initialise( bbSet );
        billboardSet->getHandle()->setName( name );
        // m_graphicsObjects[name] = billboardSet.get();
        return billboardSet;
    }

    auto CSceneManagerOgreNext::getBillboardSet( const String &name ) -> SmartPtr<IBillboardSet>
    {
        // GraphicsObjects::const_iterator it = m_graphicsObjects.find(name);
        // if( it != m_graphicsObjects.end() )
        //{
        //	return it->second;
        // }

        return nullptr;
    }

    auto CSceneManagerOgreNext::addParticleSystem( const String &name, const String &templateName )
        -> SmartPtr<IParticleSystem>
    {
#ifndef _FINAL_
        validateGfxObjName( name );
#endif

#if FB_OGRE_USE_PARTICLE_UNIVERSE
        PUParticleSystemPtr particleSystem;  // (new PUParticleSystem(this), true);
        particleSystem->setName( name );
        particleSystem->setTemplateName( templateName );
        // m_graphicsObjects[name] = particleSystem.get();

        return particleSystem;
#endif

        return nullptr;
    }

    auto CSceneManagerOgreNext::addParticleSystem( const String &name,
                                                   SmartPtr<IParticleSystemBuilder> &builder )
        -> SmartPtr<IParticleSystem>
    {
        return nullptr;
    }

    auto CSceneManagerOgreNext::getParticleSystem( const String &name ) const
        -> SmartPtr<IParticleSystem>
    {
        // GraphicsObjects::const_iterator it = m_graphicsObjects.find(name);
        // if( it != m_graphicsObjects.end() )
        //{
        //	return it->second;
        // }

        return nullptr;
    }

    auto CSceneManagerOgreNext::createAnimationStateController() -> SmartPtr<IAnimationStateController>
    {
        SmartPtr<IAnimationStateController>
            animStateCtrl;  // (new CAnimationStateController(this), true);
        return animStateCtrl;
    }

    auto CSceneManagerOgreNext::createAnimationTextureCtrl( SmartPtr<IMaterialTexture> textureUnit,
                                                            bool clone,
                                                            const String &clonedMaterialName )
        -> SmartPtr<IAnimationTextureControl>
    {
        Ogre::TextureUnitState *textureUnitState = nullptr;
        // textureUnit->_getObject((void**)&textureUnitState);

        auto textureCtrl = new CAnimationTextureControl;
        textureCtrl->initialise( textureUnitState );

        SmartPtr<IAnimationTextureControl> animTextureCtrl;  // (textureCtrl, true);
        return animTextureCtrl;
    }

    auto CSceneManagerOgreNext::removeSceneNode( SmartPtr<ISceneNode> sceneNode ) -> bool
    {
        if( m_isClearing )
        {
            return false;
        }

        if( m_sceneNodes )
        {
            auto pSceneNodes = m_sceneNodes;
            auto &sceneNodes = *pSceneNodes;
            auto sceneNodesArray = Array<SmartPtr<ISceneNode>>( sceneNodes.begin(), sceneNodes.end() );

            auto it = std::find( sceneNodesArray.begin(), sceneNodesArray.end(), sceneNode );
            if( it != sceneNodesArray.end() )
            {
                sceneNodesArray.erase( it );

                auto newSceneNodes = fb::make_shared<ConcurrentArray<SmartPtr<ISceneNode>>>();
                *newSceneNodes = ConcurrentArray<SmartPtr<ISceneNode>>( sceneNodesArray.begin(),
                                                                        sceneNodesArray.end() );
                setSceneNodes( newSceneNodes );

                return true;
            }
        }

        return false;
    }

    auto CSceneManagerOgreNext::removeGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject ) -> bool
    {
        if( m_isClearing )
        {
            return false;
        }

        FB_ASSERT( graphicsObject );

        if( graphicsObject )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            graphicsSystem->unloadObject( graphicsObject );

            auto it = std::find( m_graphicsObjects.begin(), m_graphicsObjects.end(), graphicsObject );
            if( it != m_graphicsObjects.end() )
            {
                m_graphicsObjects.erase( it );
                return true;
            }
        }

        return false;
    }

    auto CSceneManagerOgreNext::getGraphicsObjects() const -> Array<SmartPtr<IGraphicsObject>>
    {
        return m_graphicsObjects;
    }

    void CSceneManagerOgreNext::setSkyBox( bool enable, SmartPtr<IMaterial> material, f32 distance,
                                           bool drawFirst )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
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
                if( enable )
                {
                    if( m_sceneManager )
                    {
                        if( material )
                        {
                            auto pMaterial = fb::static_pointer_cast<CMaterialOgreNext>( material );

                            auto texName = String( "" );

                            if( auto cubeTexture = pMaterial->getCubeTexture() )
                            {
                                if( auto handle = cubeTexture->getHandle() )
                                {
                                    texName = handle->getName();
                                }
                            }

                            if( !StringUtil::isNullOrEmpty( texName ) )
                            {
                                m_sceneManager->setSky(
                                    enable, Ogre::SceneManager::SkyCubemap, texName,
                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
                            }
                        }
                    }
                }
                else
                {
                    m_sceneManager->setSky( enable, Ogre::SceneManager::SkyCubemap, "",
                                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
                }
            }
            else
            {
                auto message = factoryManager->make_ptr<StateMessageSkyBox>();
                FB_ASSERT( message );

                message->setEnable( enable );
                message->setMaterial( material );

                auto stateContext = getStateContext();
                if( stateContext )
                {
                    stateContext->addMessage( stateTask, message );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CSceneManagerOgreNext::setFog( u32 fogMode, const ColourF &colour, f32 expDensity,
                                        f32 linearStart, f32 linearEnd )
    {
        if( m_sceneManager )
        {
            auto fogColour = Ogre::ColourValue( colour.r, colour.g, colour.b, colour.a );
            auto ogreFogMode = static_cast<Ogre::FogMode>( fogMode );

            m_sceneManager->setFog( ogreFogMode, fogColour, expDensity, linearStart, linearEnd );
        }
    }

    auto CSceneManagerOgreNext::getEnableSkybox() const -> bool
    {
        return false;
    }

    auto CSceneManagerOgreNext::addSceneNode() -> SmartPtr<ISceneNode>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto sceneNode = factoryManager->make_ptr<CSceneNodeOgreNext>();
        FB_ASSERT( sceneNode );

        sceneNode->setCreator( this );

        auto handle = sceneNode->getHandle();
        auto name = handle->getName();
        auto hash = handle->getHash();

        m_sceneNodes->push_back( sceneNode );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        graphicsSystem->loadObject( sceneNode );

        return sceneNode;
    }

    auto CSceneManagerOgreNext::addSceneNode( const String &name ) -> SmartPtr<ISceneNode>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto sceneNode = factoryManager->make_ptr<CSceneNodeOgreNext>();
        FB_ASSERT( sceneNode );

        sceneNode->setCreator( this );

        auto handle = sceneNode->getHandle();
        handle->setName( name );

        m_sceneNodes->push_back( sceneNode );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        graphicsSystem->loadObject( sceneNode );

        return sceneNode;
    }

    auto CSceneManagerOgreNext::addStaticNode() -> SmartPtr<ISceneNode>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto sceneNode = factoryManager->make_ptr<CSceneNodeOgreNext>();
        FB_ASSERT( sceneNode );

        sceneNode->setCreator( this );

        m_sceneNodes->push_back( sceneNode );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        graphicsSystem->loadObject( sceneNode );

        return sceneNode;
    }

    void CSceneManagerOgreNext::registerSceneNodeForUpdates( SmartPtr<ISceneNode> sceneNode )
    {
        m_registeredSceneNodes->push_back( sceneNode );
    }

    auto CSceneManagerOgreNext::unregisteredForUpdates( SmartPtr<ISceneNode> sceneNode ) -> bool
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

    auto CSceneManagerOgreNext::unregisteredForUpdates( ISceneNode *sceneNode ) -> bool
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

    void CSceneManagerOgreNext::registerForUpdates( SmartPtr<IGraphicsObject> gfxObject )
    {
        SpinRWMutex::ScopedLock lock( ListMutex );
        // m_registeredGfxObjects.push_back(gfxObject.get());
    }

    auto CSceneManagerOgreNext::unregisteredForUpdates( SmartPtr<IGraphicsObject> gfxObject ) -> bool
    {
        SpinRWMutex::ScopedLock lock( ListMutex );
        // return m_registeredGfxObjects.erase_element(gfxObject.get());

        return false;
    }

    auto CSceneManagerOgreNext::unregisteredForUpdates( IGraphicsObject *gfxObject ) -> bool
    {
        SpinRWMutex::ScopedLock lock( ListMutex );
        // return m_registeredGfxObjects.erase_element(gfxObject);

        return false;
    }

    auto CSceneManagerOgreNext::getSceneManager() const -> Ogre::SceneManager *
    {
        return m_sceneManager;
    }

    void CSceneManagerOgreNext::_getObject( void **ppObject ) const
    {
        *ppObject = m_sceneManager;
    }

    void CSceneManagerOgreNext::clearQueues()
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

        // for (u32 i = 0; i < entityDeleteQueue.size(); ++i)
        //{
        //	Ogre::Entity* entity = entityDeleteQueue[i];
        //	m_sceneManager->destroyEntity(entity);
        // }

        for( auto sceneNode : sceneNodeDeleteQueue )
        {
            m_sceneManager->destroySceneNode( sceneNode );
        }

        for( auto particleSystem : particleSysDeleteQueue )
        {
            m_sceneManager->destroyParticleSystem( particleSystem );
        }
    }

    auto CSceneManagerOgreNext::getUniqueName( const String &baseName ) const -> String
    {
        return baseName + StringUtil::toString( m_nextGeneratedNameExt++ );
    }

    auto CSceneManagerOgreNext::createTerrain() -> SmartPtr<ITerrain>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto terrain = factoryManager->make_ptr<CTerrainOgreNext>();
        FB_ASSERT( terrain );

        terrain->setSceneManager( this );
        graphicsSystem->loadObject( terrain );

        m_terrains.emplace_back( terrain );

        return terrain;
    }

    void CSceneManagerOgreNext::destroyTerrain( SmartPtr<ITerrain> terrain )
    {
    }

    auto CSceneManagerOgreNext::addDecalCursor( const String &terrainMaterial,
                                                const String &decalTextureName, const Vector2F &size )
        -> SmartPtr<IDecalCursor>
    {
        SmartPtr<CDecalCursor> decalCursor;  // (new CDecalCursor, true);
        // decalCursor->initialise(this, terrainMaterial, decalTextureName, size);
        return decalCursor;
    }

    // raycast from a point in to the scene.
    // returns success or failure.
    // on success the point is returned in the result.
    auto CSceneManagerOgreNext::castRay( const Ray3F &ray, Vector3F &result ) -> bool
    {
        // create the ray to test
        Vector3F start = ray.getOrigin();
        Vector3F normal = ray.getDirection();

        Ogre::Ray ogreRay( Ogre::Vector3( start.X(), start.Y(), start.Z() ),
                           Ogre::Vector3( normal.X(), normal.Y(), normal.Z() ) );

        if( !m_pRaySceneQuery )
        {
            // create the ray scene query object
            // m_pRaySceneQuery = m_sceneManager->createRayQuery(Ogre::Ray(),
            // Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK); if (NULL == m_pRaySceneQuery)
            //{
            //	LOG_MESSAGE("SceneManager", "Failed to create Ogre::RaySceneQuery instance");
            //	return (false);
            //}

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
        for( auto &qr_idx : query_result )
        {
            // stop checking if we have found a raycast hit that is closer
            // than all remaining entities
            if( ( closest_distance >= 0.0f ) && ( closest_distance < qr_idx.distance ) )
            {
                break;
            }

            // only check this result if its a hit against an entity
            // if ((query_result[qr_idx].movable != NULL) &&
            //	(query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
            //{
            //	// get the entity to check
            //	Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);

            //	// mesh data to retrieve
            //	size_t vertex_count;
            //	size_t index_count;
            //	Ogre::Vector3 *vertices;
            //	unsigned long *indices;

            //	// get the mesh information
            //	GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,
            //		pentity->getParentNode()->_getDerivedPosition(),
            //		pentity->getParentNode()->_getDerivedOrientation(),
            //		pentity->getParentNode()->_getDerivedScale());

            //	// test for hitting individual triangles on the mesh
            //	bool new_closest_found = false;
            //	for (int i = 0; i < static_cast<int>(index_count); i += 3)
            //	{
            //		// check for a hit against this triangle
            //		std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ogreRay,
            // vertices[indices[i]], 			vertices[indices[i+1]], vertices[indices[i+2]], true,
            // false);

            //		// if it was a hit check if its the closest
            //		if (hit.first)
            //		{
            //			if ((closest_distance < 0.0f) ||
            //				(hit.second < closest_distance))
            //			{
            //				// this is the closest so far, save it off
            //				closest_distance = hit.second;
            //				new_closest_found = true;
            //			}
            //		}
            //	}

            //	// free the verticies and indicies memory
            //	delete[] vertices;
            //	delete[] indices;

            //	// if we found a new closest raycast for this object, update the
            //	// closest_result before moving on to the next object.
            //	if (new_closest_found)
            //	{
            //		closest_result = ogreRay.getPoint(closest_distance);
            //	}
            //}
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

    auto CSceneManagerOgreNext::getSceneNodes() const -> SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>>
    {
        return m_sceneNodes;
    }

    void CSceneManagerOgreNext::setSceneNodes(
        SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> sceneNodes )
    {
        m_sceneNodes = sceneNodes;
    }

    auto CSceneManagerOgreNext::getRegisteredSceneNodes() const
        -> SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>>
    {
        return m_registeredSceneNodes;
    }

    void CSceneManagerOgreNext::setRegisteredSceneNodes(
        SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> sceneNodes )
    {
        m_registeredSceneNodes = sceneNodes;
    }

    auto getMeshByIndex( const Array<unsigned int> &viIndexOffsets, unsigned int iIndexIndex ) -> u32
    {
        // subset search
        int iRangeMin = 0;
        int iRangeMax = static_cast<s32>( viIndexOffsets.size() ) - 2;
        while( iRangeMin != iRangeMax )
        {
            int iSamplePoint = ( iRangeMax + iRangeMin ) >> 1;
            if( iIndexIndex < viIndexOffsets[iSamplePoint + 1] )
            {
                iRangeMax = iSamplePoint;
            }
            else  //>=
            {
                iRangeMin = iSamplePoint + 1;
            }
        }

        return iRangeMax;
    }

    auto CSceneManagerOgreNext::splitMesh( SmartPtr<IGraphicsMesh> mesh,
                                           const SmartPtr<Properties> &properties )
        -> Array<SmartPtr<IGraphicsMesh>>
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
                FB_LOG_MESSAGE( "Graphics", "Warning: no \"maxSize\" property found. " );

            if( !properties->getPropertyValue( "roughness", fRoughness ) )
                FB_LOG_MESSAGE( "Graphics", "Warning: no \"roughness\" property found. " );

            if( !properties->getPropertyValue( "recoveryAttempts", nRecoveryAttempts ) )
                FB_LOG_MESSAGE( "Graphics", "Warning: no \"recoveryAttempts\" property found. " );

            if( !properties->getPropertyValue( "cutMaterial", strCutMaterial ) )
                FB_LOG_MESSAGE( "Graphics", "Warning: no \"cutMaterial\" property found. " );
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
                    if( ( it = vmConnections[iMesh].find( iOtherMesh ) ) != vmConnections[iMesh].end() )
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
            for( std::map<unsigned int, std::set<int>>::iterator it = vmConnections[iFragment].begin();
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
            mesh_->load( nullptr );

            Ogre::Entity *entity_ = m_sceneManager->createEntity( mesh_ );

            String name = entity_->getName().c_str();
            SmartPtr<CGraphicsMeshOgreNext> graphicsObject( new CGraphicsMeshOgreNext( this ), this );
            graphicsObject->initialise( entity );
            graphicsObject->getHandle()->setName( name );
            m_graphicsObjects[name] = graphicsObject.get();
            meshes.push_back( graphicsObject );
        }
#endif

        return meshes;
    }

    auto CSceneManagerOgreNext::getEnableShadows() const -> bool
    {
        return false;
    }

    void CSceneManagerOgreNext::setEnableShadows( bool enableShadows, bool depthShadows /*= true*/ )
    {
        using namespace Ogre;

        if( enableShadows )
        {
            //// General scene setup
            // m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
            // m_sceneManager->setShadowFarDistance(10);

            // 3 textures per directional light (PSSM)
            // m_sceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);

            //// shadow camera setup
            // PSSMShadowCameraSetup* pssmSetup = new PSSMShadowCameraSetup();
            // pssmSetup->setSplitPadding(MathF::epsilon());
            // pssmSetup->calculateSplitPoints(3, MathF::epsilon(),
            // m_sceneManager->getShadowFarDistance(), 0.5f); pssmSetup->setOptimalAdjustFactor(0,
            // 2); pssmSetup->setOptimalAdjustFactor(1, 1); pssmSetup->setOptimalAdjustFactor(2,
            // 0.5); pssmSetup->setUseAggressiveFocusRegion(true);
            // pssmSetup->setUseSimpleOptimalAdjust(false);

            // const Ogre::PSSMShadowCameraSetup::SplitPointList& splitList =
            // pssmSetup->getSplitPoints(); for(size_t i=0; i<splitList.size(); ++i)
            //{
            //	Ogre::Real splitValue = splitList[i];
            //	splitValue = splitValue;
            // }

            // m_sceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));

            // if (depthShadows)
            //{
            //	m_sceneManager->setShadowTextureCount(3);

            //	u32 shadowTextureSize = 1024;
            //
            //	m_sceneManager->setShadowTextureConfig(0, shadowTextureSize, shadowTextureSize,
            // Ogre::PF_FLOAT32_GR); 	m_sceneManager->setShadowTextureConfig(1,
            // shadowTextureSize*0.5, shadowTextureSize*0.5, Ogre::PF_FLOAT32_GR);
            // m_sceneManager->setShadowTextureConfig(2, shadowTextureSize*0.25,
            // shadowTextureSize*0.25, Ogre::PF_FLOAT32_GR);

            //	m_sceneManager->setShadowTextureSelfShadow(true);
            //	m_sceneManager->setShadowCasterRenderBackFaces(true);
            //	//m_sceneManager->setShadowTextureCasterMaterial("PSSM/shadow_caster");
            //}
            // else
            //{
            //	m_sceneManager->setShadowTextureCount(3);
            //	m_sceneManager->setShadowTextureConfig(0, 2048, 2048, PF_X8B8G8R8);
            //	m_sceneManager->setShadowTextureConfig(1, 1024, 1024, PF_X8B8G8R8);
            //	m_sceneManager->setShadowTextureConfig(2, 1024, 1024, PF_X8B8G8R8);
            //	m_sceneManager->setShadowTextureSelfShadow(false);
            //	m_sceneManager->setShadowCasterRenderBackFaces(false);
            //	//m_sceneManager->setShadowTextureCasterMaterial(Ogre::StringUtil::BLANK);
            //}

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
            // m_sceneManager->setShadowTechnique(SHADOWTYPE_NONE);
        }

        // const unsigned numShadowRTTs = m_sceneManager->getShadowTextureCount();
        // for (unsigned i = 0; i < numShadowRTTs; ++i)
        //{
        //	//Ogre::TexturePtr tex = m_sceneManager->getShadowTexture(i);
        //	Ogre::Viewport *vp = tex->getBuffer()->getRenderTarget()->getViewport(0);
        //	//vp->setBackgroundColour(Ogre::ColourValue::Black);

        //	//vp->setAutoUpdated(true);
        //	//vp->setClearEveryFrame(false);
        //	//vp->setSkiesEnabled(false);
        //	//vp->setVisibilityMask(IGraphicsSceneManager::VIEWPORT_MASK_SHADOW);

        //	//tex->getBuffer()->getRenderTarget()->addListener(new ShadowRenderTargetListener(tex));

        //	SSEffectRenderer* renderer = new SSEffectRenderer(m_sceneManager, vp);
        //	//renderer->quad()->setVisibilityFlags(IGraphicsSceneManager::VIEWPORT_MASK_SHADOW);
        //	//renderer->quad()->setVisible(false);

        //	SSEffectClear* effectClear  = new SSEffectClear(renderer);
        //	effectClear->create(tex.get());

        //	Ogre::Root::getSingletonPtr()->addFrameListener(new ShadowFrameListener(effectClear, tex,
        // 0));
        //}

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

    void CSceneManagerOgreNext::addExistingGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject )
    {
        m_graphicsObjects.push_back( graphicsObject );
    }

    void CSceneManagerOgreNext::addExistingSceneNode( SmartPtr<ISceneNode> sceneNode )
    {
        m_sceneNodes->push_back( sceneNode );
    }

    auto CSceneManagerOgreNext::createInstanceManager( const String &customName, const String &meshName,
                                                       const String &groupName, u32 technique,
                                                       u32 numInstancesPerBatch, u16 flags /*=0*/,
                                                       u16 subMeshIdx /*=0 */ )
        -> SmartPtr<IInstanceManager>
    {
        // Ogre::InstanceManager* mgr = m_sceneManager->createInstanceManager(
        //	customName.c_str(), meshName.c_str(),
        // Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
        //	(Ogre::InstanceManager::InstancingTechnique)technique, 80, Ogre::IM_USE16BIT, subMeshIdx
        //);

        //SmartPtr<CInstanceManager> instanceManager;  // (new CInstanceManager, true);
        //m_instanceManagers.push_back( instanceManager );
        //// instanceManager->setInstanceManager(mgr);
        //return instanceManager;

        return nullptr;
    }

    auto CSceneManagerOgreNext::createInstancedObject( const String &materialName,
                                                       const String &managerName )
        -> SmartPtr<IInstancedObject>
    {
        //SmartPtr<CInstancedObject>
        //    instancedObject;  // (new CInstancedObject(this, materialName, managerName), true);
        //_addGraphicsObject( instancedObject );
        //return instancedObject;

        return nullptr;
    }

    void CSceneManagerOgreNext::destroyInstancedObject( SmartPtr<IInstancedObject> instancedObject )
    {
    }

    auto CSceneManagerOgreNext::getType() const -> String
    {
        return m_type;
    }

    void CSceneManagerOgreNext::_addGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject )
    {
        SpinRWMutex::ScopedLock lock( GraphicsObjectMutex );
        m_graphicsObjects.push_back( graphicsObject );
    }

    auto CSceneManagerOgreNext::getBillboardSets() const -> Array<SmartPtr<IBillboardSet>>
    {
        Array<SmartPtr<IBillboardSet>> billboardSets;
        return billboardSets;
    }

    auto CSceneManagerOgreNext::getTerrain( u32 id ) const -> SmartPtr<ITerrain>
    {
        return m_terrains[id];
    }

    void CSceneManagerOgreNext::setAnimationNameSuffix( const String &val )
    {
        m_animationNameSuffix = val;
    }

    auto CSceneManagerOgreNext::getAnimationNameSuffix() const -> String
    {
        return m_animationNameSuffix;
    }

    void CSceneManagerOgreNext::setAnimationNamePrefix( const String &val )
    {
        m_animationNamePrefix = val;
    }

    auto CSceneManagerOgreNext::getAnimationNamePrefix() const -> String
    {
        return m_animationNamePrefix;
    }

    CSceneManagerOgreNext::RootSceneNode::~RootSceneNode()
    {
        unload( nullptr );
        destroyStateContext();
    }

    void CSceneManagerOgreNext::RootSceneNode::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loaded );
    }

    void CSceneManagerOgreNext::RootSceneNode::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );
        m_sceneNode = nullptr;
        CSceneNodeOgreNext::unload( data );
        setLoadingState( LoadingState::Unloaded );
    }

    CSceneManagerOgreNext::SceneManagerStateListener::SceneManagerStateListener() = default;

    CSceneManagerOgreNext::SceneManagerStateListener::~SceneManagerStateListener() = default;

    void CSceneManagerOgreNext::SceneManagerStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        Ogre::SceneManager *smgr = nullptr;
        m_owner->_getObject( reinterpret_cast<void **>( &smgr ) );

        if( smgr )
        {
            try
            {
#if _DEBUG
                auto task = Thread::getCurrentTask();
                auto renderTask =
                    core::ApplicationManager::instance()->getGraphicsSystem()->getRenderTask();
                FB_ASSERT( task == renderTask );
#endif

                auto sceneManagerState = fb::static_pointer_cast<SceneManagerState>( state );
                if( sceneManagerState )
                {
                    // if (sceneManagerState->getEnableSkybox())
                    //{
                    //	auto skyboxMaterialName = sceneManagerState->getSkyboxMaterialName();
                    //	smgr->setSkyBox(true, skyboxMaterialName);
                    // }
                    // else
                    //{
                    //	smgr->setSkyBox(false, "");
                    // }

                    auto ambientLight = OgreUtil::convertToOgre( sceneManagerState->getAmbientLight() );
                    //if( !OgreUtil::equals( ambientLight, smgr->getAmbientLight() ) )
                    {
                        //smgr->setAmbientLight( ambientLight, ambientLight, -Ogre::Vector3::UNIT_Y );
                    }

                    smgr->setAmbientLight( ambientLight * 0.995f, ambientLight * 0.5f,
                                           Ogre::Vector3::UNIT_Y, 1.0f );
                }

                state->setDirty( false );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
    }

    void CSceneManagerOgreNext::SceneManagerStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        if( message->isExactly<StateMessageSkyBox>() )
        {
            auto pMessage = fb::static_pointer_cast<StateMessageSkyBox>( message );
            auto enabled = pMessage->getEnable();
            auto material = pMessage->getMaterial();

            m_owner->setSkyBox( enabled, material );
        }
    }

    void CSceneManagerOgreNext::SceneManagerStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto CSceneManagerOgreNext::SceneManagerStateListener::getOwner() const -> CSceneManagerOgreNext *
    {
        return m_owner;
    }

    void CSceneManagerOgreNext::SceneManagerStateListener::setOwner( CSceneManagerOgreNext *val )
    {
        m_owner = val;
    }
}  // namespace fb::render
