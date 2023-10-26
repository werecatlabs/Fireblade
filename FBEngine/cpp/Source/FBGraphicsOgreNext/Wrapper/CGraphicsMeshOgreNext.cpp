#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsMeshOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneNodeOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CAnimationControllerOgre.h>
#include <FBGraphicsOgreNext/ShadowFrameListener.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreItem.h>
#include <OgreMeshManager.h>
#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreSceneManager.h>
#include <MeshLoader.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CGraphicsMeshOgreNext, IGraphicsMesh );

        CGraphicsMeshOgreNext::CGraphicsMeshOgreNext()
        {
            setupStateObject();
        }

        CGraphicsMeshOgreNext::CGraphicsMeshOgreNext( SmartPtr<IGraphicsScene> creator ) :
            m_checkVertProcessing( false ),
            m_hardwareAnimationEnabled( false )
        {
            m_creator = creator;
            setupStateObject();
        }

        CGraphicsMeshOgreNext::~CGraphicsMeshOgreNext()
        {
            destroyStateContext();

            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                unload( nullptr );
            }
        }

        void CGraphicsMeshOgreNext::initialise( Ogre::v1::Entity *entity )
        {
            m_entity = entity;

            //if( m_entity->hasSkeleton() || m_entity->hasVertexAnimation() )
            //{
            //    registerForUpdates( true );
            //}
        }

        void CGraphicsMeshOgreNext::setMaterialName( const String &materialName, s32 index )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto materialManager = graphicsSystem->getMaterialManager();

                auto factoryManager = applicationManager->getFactoryManager();
                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( index == -1 )
                    {
                        if( m_entity )
                        {
                            m_entity->setMaterialName( materialName.c_str() );
                        }

                        if( auto item = getItem() )
                        {
                            auto material = materialManager->getByName( materialName );
                            if( material )
                            {
                                auto pMaterial = fb::static_pointer_cast<CMaterialOgreNext>( material );
                                if( pMaterial )
                                {
                                    auto datablock = pMaterial->getHlmsDatablock();
                                    if( datablock )
                                    {
                                        item->setDatablock( datablock );
                                    }
                                }

                                m_material = material;

                                auto materialStateObject = m_material->getStateObject();
                                if( materialStateObject )
                                {
                                    materialStateObject->addStateListener( m_materialStateListener );
                                }
                            }
                        }
                    }
                    else
                    {
                        if( m_entity )
                        {
                            auto subEntity = m_entity->getSubEntity( index );
                            if( subEntity )
                            {
                                subEntity->setMaterialName(
                                    materialName.c_str(),
                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
                            }
                        }

                        if( auto item = getItem() )
                        {
                            auto subItem = item->getSubItem( index );
                            if( subItem )
                            {
                                auto material = materialManager->getByName( materialName );
                                if( material )
                                {
                                    auto pMaterial =
                                        fb::static_pointer_cast<CMaterialOgreNext>( material );
                                    if( pMaterial )
                                    {
                                        auto datablock = pMaterial->getHlmsDatablock();
                                        if( datablock )
                                        {
                                            subItem->setDatablock( datablock );
                                        }
                                    }

                                    m_materials[index] = material;

                                    auto materialStateObject = material->getStateObject();
                                    if( materialStateObject )
                                    {
                                        materialStateObject->addStateListener( m_materialStateListener );
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageMaterialName>();
                    message->setMaterialName( materialName );
                    message->setIndex( index );

                    if( auto stateObject = getStateObject() )
                    {
                        stateObject->addMessage( stateTask, message );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        String CGraphicsMeshOgreNext::getMaterialName( s32 index ) const
        {
            return StringUtil::EmptyString;
        }

        void CGraphicsMeshOgreNext::setMaterial( SmartPtr<IMaterial> material, s32 index )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                auto materialManager = graphicsSystem->getMaterialManager();

                auto factoryManager = applicationManager->getFactoryManager();
                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                if( isThreadSafe() )
                {
                    if( index == -1 )
                    {
                        //if( m_entity )
                        //{
                        //    m_entity->setMaterialName( materialName.c_str() );
                        //}

                        if( auto item = getItem() )
                        {
                            if( m_material )
                            {
                                auto materialStateObject = m_material->getStateObject();
                                if( materialStateObject )
                                {
                                    materialStateObject->removeStateListener( m_materialStateListener );
                                }
                            }

                            auto pMaterial = fb::static_pointer_cast<CMaterialOgreNext>( material );
                            if( pMaterial )
                            {
                                auto datablock = pMaterial->getHlmsDatablock();
                                if( datablock )
                                {
                                    item->setDatablock( datablock );
                                }
                            }

                            m_material = material;

                            if( m_material )
                            {
                                auto materialStateObject = m_material->getStateObject();
                                if( materialStateObject )
                                {
                                    materialStateObject->addStateListener( m_materialStateListener );
                                }
                            }
                        }
                    }
                    else
                    {
                        if( auto item = getItem() )
                        {
                            auto subItem = item->getSubItem( index );
                            if( subItem )
                            {
                                auto currentMaterial = m_materials[index];
                                if( currentMaterial )
                                {
                                    auto materialStateObject = currentMaterial->getStateObject();
                                    if( materialStateObject )
                                    {
                                        materialStateObject->removeStateListener(
                                            m_materialStateListener );
                                    }
                                }

                                auto pMaterial = fb::static_pointer_cast<CMaterialOgreNext>( material );
                                if( pMaterial )
                                {
                                    auto datablock = pMaterial->getHlmsDatablock();
                                    if( datablock )
                                    {
                                        subItem->setDatablock( datablock );
                                    }
                                }

                                m_materials[index] = material;

                                auto materialStateObject = material->getStateObject();
                                if( materialStateObject )
                                {
                                    materialStateObject->addStateListener( m_materialStateListener );
                                }
                            }
                        }
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageMaterial>();
                    message->setMaterial( material );
                    message->setIndex( index );

                    if( auto stateObject = getStateObject() )
                    {
                        stateObject->addMessage( stateTask, message );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IMaterial> CGraphicsMeshOgreNext::getMaterial( s32 index ) const
        {
            return nullptr;
        }

        SmartPtr<IGraphicsObject> CGraphicsMeshOgreNext::clone( const String &name ) const
        {
            auto mesh = fb::make_ptr<CGraphicsMeshOgreNext>();
            auto data = toData();
            mesh->fromData( data );
            return mesh;
        }

        void CGraphicsMeshOgreNext::_getObject( void **ppObject ) const
        {
            if( m_entity )
            {
                *ppObject = m_entity;
            }
            else if( m_item )
            {
                *ppObject = m_item;
            }
            else
            {
                *ppObject = nullptr;
            }
        }

        void CGraphicsMeshOgreNext::checkVertexProcessing()
        {
            m_checkVertProcessing = true;
        }

        SmartPtr<IAnimationController> CGraphicsMeshOgreNext::getAnimationController()
        {
            if( !m_animationController )
            {
                CAnimationControllerPtr animationController( new CAnimationController );
                animationController->Initialize( m_entity );
                m_animationController = animationController;
                // m_animationController->getStateObject()->add();
            }

            return m_animationController;
        }

        Ogre::v1::Entity *CGraphicsMeshOgreNext::getEntity() const
        {
            return m_entity;
        }

        void CGraphicsMeshOgreNext::setHardwareAnimationEnabled( bool enabled )
        {
            m_hardwareAnimationEnabled = enabled;
        }

        Ogre::MeshPtr CGraphicsMeshOgreNext::createMesh( const String &meshName )
        {
            try
            {
                Ogre::SceneManager *smgr = nullptr;
                m_creator->_getObject( (void **)&smgr );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();

                auto meshManagerV2 = Ogre::MeshManager::getSingletonPtr();

                auto meshExtention = Path::getFileExtension( meshName );
                static const auto engineMeshExtention = String( ".fbmeshbin" );
                if( meshExtention == engineMeshExtention )
                {
                    auto meshNameWithoutExtention = Path::getFileNameWithoutExtension( meshName );
                    auto resourceGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

                    auto meshResult = meshManagerV2->getByName( meshName, resourceGroupName );
                    if( !meshResult )
                    {
                        auto mesh = meshManagerV2->createManual( meshName, resourceGroupName );
                        if( mesh )
                        {
                            MeshLoader::loadFBMesh( mesh, meshName );
                            return mesh;
                        }
                    }
                    else
                    {
                        auto mesh = meshManagerV2->load( meshName, resourceGroupName );
                        return mesh;
                    }
                }
                else
                {
                    auto meshManagerV1 = Ogre::v1::MeshManager::getSingletonPtr();
                    auto resourceGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

                    auto meshResult = meshManagerV1->createOrRetrieve( meshName, resourceGroupName );
                    if( meshResult.second )
                    {
                        auto planeMeshV1 = meshResult.first.dynamicCast<Ogre::v1::Mesh>();

                        auto planeMesh = meshManagerV2->createManual( meshName, resourceGroupName );
                        planeMesh->importV1( planeMeshV1.get(), false, false, false );
                        return planeMesh;
                    }
                    auto mesh = meshManagerV2->load( meshName, resourceGroupName );
                    return mesh;
                }
            }
            catch( Ogre::Exception &e )
            {
                auto error = e.getFullDescription();
                FB_LOG_ERROR( error );
            }

            return Ogre::MeshPtr();
        }

        void CGraphicsMeshOgreNext::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

                auto resourceGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

                Ogre::SceneManager *smgr = nullptr;

                if( auto creator = getCreator() )
                {
                    creator->_getObject( (void **)&smgr );
                }

                auto meshName = getMeshName();
                if( !StringUtil::isNullOrEmpty( meshName ) )
                {
                    auto mesh = createMesh( meshName );
                    if( !mesh )
                    {
                        FB_LOG_ERROR( "Unable to create mesh: " + meshName );
                    }

                    if( mesh )
                    {
                        auto item = smgr->createItem( mesh );
                        setItem( item );
                    }
                }

                if( auto item = getItem() )
                {
                    auto numSubItems = item->getNumSubItems();
                    m_materials.resize( numSubItems );
                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( Ogre::Exception &e )
            {
                FB_LOG_ERROR( e.getFullDescription().c_str() );
            }
        }

        void CGraphicsMeshOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( isLoaded() )
                {
                    setLoadingState( LoadingState::Unloading );

                    destroyStateContext();

                    if( m_creator )
                    {
                        SmartPtr<CSceneManagerOgreNext> smgr = m_creator.lock();
                        Ogre::SceneManager *ogreSmgr = smgr->getSceneManager();

                        if( m_entity )
                        {
                            m_entity->detachFromParent();
                            ogreSmgr->destroyEntity( m_entity );
                        }
                    }

                    //registerForUpdates( false );

                    if( m_materialStateListener )
                    {
                        m_materialStateListener->unload( data );
                        m_materialStateListener = nullptr;
                    }

                    CGraphicsObjectOgreNext<IGraphicsMesh>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        hash32 CGraphicsMeshOgreNext::getRenderTechnique() const
        {
            return 0;
        }

        void CGraphicsMeshOgreNext::setRenderTechnique( hash32 val )
        {
        }

        Ogre::Item *CGraphicsMeshOgreNext::getItem() const
        {
            return m_item;
        }

        void CGraphicsMeshOgreNext::setItem( Ogre::Item *val )
        {
            m_item = val;
        }

        String CGraphicsMeshOgreNext::getMeshName() const
        {
            return m_meshName;
        }

        void CGraphicsMeshOgreNext::setMeshName( const String &meshName )
        {
            m_meshName = meshName;
        }

        SmartPtr<Properties> CGraphicsMeshOgreNext::getProperties() const
        {
            auto properties = CGraphicsObjectOgreNext<IGraphicsMesh>::getProperties();

            properties->setProperty( "MeshName", m_meshName );

            return properties;
        }

        void CGraphicsMeshOgreNext::setProperties( SmartPtr<Properties> properties )
        {
            properties->getPropertyValue( "MeshName", m_meshName );
        }

        Array<SmartPtr<ISharedObject>> CGraphicsMeshOgreNext::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> childObjects;
            childObjects.reserve( 4 );

            if( m_material )
            {
                childObjects.push_back( m_material );
            }

            if( !m_materials.empty() )
            {
                for( auto material : m_materials )
                {
                    childObjects.push_back( material );
                }
            }

            childObjects.push_back( getStateObject() );
            childObjects.push_back( getStateListener() );
            childObjects.push_back( getOwner() );

            return childObjects;
        }

        void CGraphicsMeshOgreNext::materialLoaded( SmartPtr<IMaterial> material )
        {
            try
            {
                if( material )
                {
                    if( material == m_material )
                    {
                        if( auto item = getItem() )
                        {
                            auto pMaterial = fb::static_pointer_cast<CMaterialOgreNext>( material );
                            auto dataBlock = pMaterial->getHlmsDatablock();
                            if( dataBlock )
                            {
                                item->setDatablock( dataBlock );
                            }
                        }
                    }
                }

                for( size_t i = 0; i < m_materials.size(); ++i )
                {
                    auto currentMaterial = m_materials[i];

                    if( material == currentMaterial )
                    {
                        if( auto item = getItem() )
                        {
                            FB_ASSERT( i < item->getNumSubItems() );
                            auto subItem = item->getSubItem( i );
                            if( subItem )
                            {
                                auto pMaterial = fb::static_pointer_cast<CMaterialOgreNext>( material );
                                auto dataBlock = pMaterial->getHlmsDatablock();
                                if( dataBlock )
                                {
                                    subItem->setDatablock( dataBlock );
                                }
                            }
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CGraphicsMeshOgreNext::setupStateObject()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();
            setStateObject( stateObject );

            auto stateListener = factoryManager->make_ptr<MeshStateListener>();
            stateListener->setOwner( this );
            setStateListener( stateListener );
            stateObject->addStateListener( stateListener );

            auto materialStateListener = factoryManager->make_ptr<MaterialStateListener>();
            materialStateListener->setOwner( this );
            m_materialStateListener = materialStateListener;

            auto state = factoryManager->make_ptr<GraphicsMeshState>();
            stateObject->setState( state );
            stateObject->setOwner( this );

            setGraphicsObjectState( state );
        }

        void CGraphicsMeshOgreNext::MeshStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            //IStateListener::handleStateChanged( message );

            FB_ASSERT( m_owner );

            if( message->isExactly<StateMessageMaterialName>() )
            {
                auto materialMessage = fb::static_pointer_cast<StateMessageMaterialName>( message );
                auto materialName = materialMessage->getMaterialName();
                auto materialIndex = materialMessage->getIndex();
                m_owner->setMaterialName( materialName, materialIndex );
            }
            else if( message->isExactly<StateMessageMaterial>() )
            {
                auto materialMessage = fb::static_pointer_cast<StateMessageMaterial>( message );
                auto material = materialMessage->getMaterial();
                auto materialIndex = materialMessage->getIndex();
                m_owner->setMaterial( material, materialIndex );
            }
            else if( message->isExactly<StateMessageVisible>() )
            {
                auto visibleMessage = fb::static_pointer_cast<StateMessageVisible>( message );
                auto bIsVisible = visibleMessage->isVisible();
                m_owner->setVisible( bIsVisible );
            }
            else if( message->isExactly<StateMessageUIntValue>() )
            {
                auto intValueMessage = fb::static_pointer_cast<StateMessageUIntValue>( message );
                auto type = intValueMessage->getType();
                auto value = intValueMessage->getValue();

                if( type == RENDER_QUEUE_HASH )
                {
                    m_owner->setRenderQueueGroup( static_cast<u8>( value ) );
                }
                else if( type == VISIBILITY_FLAGS_HASH )
                {
                    m_owner->setVisibilityFlags( value );
                }
            }
        }

        void CGraphicsMeshOgreNext::MeshStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            CStateListener::handleStateChanged( state );
        }

        void CGraphicsMeshOgreNext::MeshStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CGraphicsMeshOgreNext *CGraphicsMeshOgreNext::MeshStateListener::getOwner() const
        {
            return m_owner;
        }

        void CGraphicsMeshOgreNext::MeshStateListener::setOwner( CGraphicsMeshOgreNext *owner )
        {
            m_owner = owner;
        }

        CGraphicsMeshOgreNext::MeshStateListener::~MeshStateListener()
        {
        }

        CGraphicsMeshOgreNext::MeshStateListener::MeshStateListener( CGraphicsMeshOgreNext *owner ) :
            m_owner( owner )
        {
        }

        CGraphicsMeshOgreNext::MaterialStateListener::MaterialStateListener(
            CGraphicsMeshOgreNext *owner )
        {
        }

        CGraphicsMeshOgreNext::MaterialStateListener::~MaterialStateListener()
        {
        }

        void CGraphicsMeshOgreNext::MaterialStateListener::unload( SmartPtr<ISharedObject> data )
        {
            m_owner = nullptr;
        }

        void CGraphicsMeshOgreNext::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto owner = getOwner() )
            {
                if( owner->isLoaded() )
                {
                    auto materialObject = owner->getMaterial();
                    owner->materialLoaded( materialObject );

                    auto materials = owner->m_materials;
                    for( auto m : materials )
                    {
                        owner->materialLoaded( m );
                    }
                }
            }
        }

        void CGraphicsMeshOgreNext::MaterialStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = getOwner() )
            {
                if( message->isExactly<StateMessageLoad>() )
                {
                    auto stateMessageLoad = fb::static_pointer_cast<StateMessageLoad>( message );
                    auto eventType = stateMessageLoad->getType();

                    if( eventType == StateMessageLoad::LOADED_HASH )
                    {
                        if( owner->isLoaded() )
                        {
                            auto materialObject = stateMessageLoad->getObject();
                            //owner->materialLoaded( materialObject );
                        }
                    }
                }
            }
        }

        void CGraphicsMeshOgreNext::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        SmartPtr<CGraphicsMeshOgreNext> CGraphicsMeshOgreNext::MaterialStateListener::getOwner() const
        {
            auto p = m_owner.load();
            return p.lock();
        }

        void CGraphicsMeshOgreNext::MaterialStateListener::setOwner(
            SmartPtr<CGraphicsMeshOgreNext> owner )
        {
            m_owner = owner;
        }
    }  // end namespace render
}  // end namespace fb
