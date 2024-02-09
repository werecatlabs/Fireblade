#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsMeshOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneNodeOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CAnimationControllerOgre.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreItem.h>
#include <OgreMeshManager.h>
#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <MeshLoader.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, CGraphicsMeshOgreNext, IGraphicsMesh );

    CGraphicsMeshOgreNext::CGraphicsMeshOgreNext()
    {
        setupStateObject();

        m_materialSharedObjectListener = fb::make_ptr<MaterialEventListener>();
        m_materialSharedObjectListener->setOwner( this );
    }

    CGraphicsMeshOgreNext::CGraphicsMeshOgreNext( SmartPtr<IGraphicsScene> creator )
    {
        m_creator = creator;
        setupStateObject();

        m_materialSharedObjectListener = fb::make_ptr<MaterialEventListener>();
        m_materialSharedObjectListener->setOwner( this );
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

    void CGraphicsMeshOgreNext::setMaterialName( const String &materialName, s32 index )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
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

                            auto materialStateObject = m_material->getStateContext();
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
                        if( index < item->getNumSubItems() )
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

                                    auto materialStateObject = material->getStateContext();
                                    if( materialStateObject )
                                    {
                                        materialStateObject->addStateListener( m_materialStateListener );
                                    }
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

                if( auto stateContext = getStateContext() )
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

    auto CGraphicsMeshOgreNext::getMaterialName( s32 index ) const -> String
    {
        if( auto material = getMaterial( index ) )
        {
            if( auto handle = material->getHandle() )
            {
                return handle->getName();
            }
        }

        return StringUtil::EmptyString;
    }

    void CGraphicsMeshOgreNext::setMaterial( SmartPtr<IMaterial> material, s32 index )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto materialManager = graphicsSystem->getMaterialManager();

            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();
            auto stateTask = graphicsSystem->getStateTask();
            auto task = Thread::getCurrentTask();

            if( isThreadSafe() )
            {
                if( material )
                {
                    if( !material->isLoaded() )
                    {
                        material->load( nullptr );
                    }
                }

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
                            auto materialStateObject = m_material->getStateContext();
                            if( materialStateObject )
                            {
                                materialStateObject->removeStateListener( m_materialStateListener );
                            }

                            m_material->removeObjectListener( m_materialSharedObjectListener );
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
                            auto materialStateObject = m_material->getStateContext();
                            if( materialStateObject )
                            {
                                materialStateObject->addStateListener( m_materialStateListener );
                            }

                            m_material->addObjectListener( m_materialSharedObjectListener );
                        }
                    }
                }
                else
                {
                    if( auto item = getItem() )
                    {
                        if( index < item->getNumSubItems() )
                        {
                            auto subItem = item->getSubItem( index );
                            if( subItem )
                            {
                                auto currentMaterial = m_materials[index];
                                if( currentMaterial )
                                {
                                    auto materialStateObject = currentMaterial->getStateContext();
                                    if( materialStateObject )
                                    {
                                        materialStateObject->removeStateListener(
                                            m_materialStateListener );
                                    }

                                    currentMaterial->removeObjectListener(
                                        m_materialSharedObjectListener );
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

                                auto materialStateObject = material->getStateContext();
                                if( materialStateObject )
                                {
                                    materialStateObject->addStateListener( m_materialStateListener );
                                }

                                material->addObjectListener( m_materialSharedObjectListener );
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

                if( auto stateContext = getStateContext() )
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

    auto CGraphicsMeshOgreNext::getMaterial( s32 index ) const -> SmartPtr<IMaterial>
    {
        if( index < m_materials.size() )
        {
            return m_materials[index];
        }

        return nullptr;
    }

    auto CGraphicsMeshOgreNext::clone( const String &name ) const -> SmartPtr<IGraphicsObject>
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

    auto CGraphicsMeshOgreNext::getAnimationController() -> SmartPtr<IAnimationController>
    {
        if( !m_animationController )
        {
            CAnimationControllerPtr animationController( new CAnimationController );
            animationController->Initialize( m_entity );
            m_animationController = animationController;
            // m_animationController->getStateContext()->add();
        }

        return m_animationController;
    }

    auto CGraphicsMeshOgreNext::getEntity() const -> Ogre::v1::Entity *
    {
        return m_entity;
    }

    void CGraphicsMeshOgreNext::setHardwareAnimationEnabled( bool enabled )
    {
        m_hardwareAnimationEnabled = enabled;
    }

    auto CGraphicsMeshOgreNext::createMesh( const String &meshName ) -> Ogre::MeshPtr
    {
        try
        {
            Ogre::SceneManager *smgr = nullptr;
            m_creator->_getObject( reinterpret_cast<void **>( &smgr ) );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();

            auto meshManagerV2 = Ogre::MeshManager::getSingletonPtr();

            auto meshExtention = Path::getFileExtension( meshName );
            static const auto engineMeshExtention = String( ".fbmeshbin" );
            static const auto ogreMeshExtention = String( ".mesh" );

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
                    if( !mesh )
                    {
                        auto fileName = Path::getFileNameWithoutExtension( meshName );
                        mesh = meshManagerV2->load( fileName, resourceGroupName );
                    }

                    return mesh;
                }
            }
            else if( meshExtention == ogreMeshExtention )
            {
                auto meshManagerV1 = Ogre::v1::MeshManager::getSingletonPtr();
                auto resourceGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

                auto meshResult = meshManagerV1->createOrRetrieve( meshName, resourceGroupName );
                if( meshResult.second )
                {
                    auto meshV1 = meshResult.first.dynamicCast<Ogre::v1::Mesh>();

                    auto meshV2 = meshManagerV2->createManual( meshName, resourceGroupName );
                    meshV2->importV1( meshV1.get(), false, false, false );
                    return meshV2;
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

        return {};
    }

    void CGraphicsMeshOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ScopedLock lock( graphicsSystem );

            auto resourceGroupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

            Ogre::SceneManager *smgr = nullptr;

            if( auto creator = getCreator() )
            {
                creator->_getObject( reinterpret_cast<void **>( &smgr ) );
            }

            auto meshName = getMeshName();
            if( !StringUtil::isNullOrEmpty( meshName ) )
            {
                if( StringUtil::contains( meshName, "LOD0_5460296883734790039.fbmeshbin" ) )
                {
                    int i = 0;
                    i = 0;
                }

                auto mesh = createMesh( meshName );
                if( !mesh )
                {
                    FB_LOG_ERROR( "Unable to create mesh: " + meshName );
                }

                if( mesh )
                {
                    auto item = smgr->createItem( mesh );
                    setItem( item );
                    setMovableObject( item );

                    item->setName( meshName );
                    item->setCastShadows( true );
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

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                ScopedLock lock( graphicsSystem );

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

                    if( auto item = getItem() )
                    {
                        item->detachFromParent();
                        ogreSmgr->destroyItem( item );
                        setItem( nullptr );
                    }
                }

                //registerForUpdates( false );

                if( m_materialStateListener )
                {
                    m_materialStateListener->unload( data );
                    m_materialStateListener = nullptr;
                }

                setItem( nullptr );
                setMovableObject( nullptr );

                CGraphicsObjectOgreNext<IGraphicsMesh>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CGraphicsMeshOgreNext::getItem() const -> Ogre::Item *
    {
        return m_item;
    }

    void CGraphicsMeshOgreNext::setItem( Ogre::Item *item )
    {
        m_item = item;
    }

    auto CGraphicsMeshOgreNext::getMeshName() const -> String
    {
        return m_meshName;
    }

    void CGraphicsMeshOgreNext::setMeshName( const String &meshName )
    {
        m_meshName = meshName;
    }

    auto CGraphicsMeshOgreNext::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = CGraphicsObjectOgreNext<IGraphicsMesh>::getProperties();

        properties->setProperty( "MeshName", m_meshName );

        return properties;
    }

    void CGraphicsMeshOgreNext::setProperties( SmartPtr<Properties> properties )
    {
        CGraphicsObjectOgreNext<IGraphicsMesh>::setProperties( properties );

        properties->getPropertyValue( "MeshName", m_meshName );
    }

    auto CGraphicsMeshOgreNext::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> childObjects;
        childObjects.reserve( 4 );

        if( m_material )
        {
            childObjects.emplace_back( m_material );
        }

        if( !m_materials.empty() )
        {
            for( auto material : m_materials )
            {
                childObjects.emplace_back( material );
            }
        }

        childObjects.emplace_back( getStateContext() );
        childObjects.emplace_back( getStateListener() );
        childObjects.emplace_back( getOwner() );

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
                        if( i < item->getNumSubItems() )
                        {
                            auto subItem = item->getSubItem( i );
                            if( subItem )
                            {
                                if( material )
                                {
                                    auto pMaterial =
                                        fb::static_pointer_cast<CMaterialOgreNext>( material );
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
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CGraphicsMeshOgreNext::setupStateObject()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();
        setStateContext( stateContext );

        auto stateListener = factoryManager->make_ptr<MeshStateListener>();
        stateListener->setOwner( this );
        setStateListener( stateListener );
        stateContext->addStateListener( stateListener );

        auto materialStateListener = factoryManager->make_ptr<MaterialStateListener>();
        materialStateListener->setOwner( this );
        m_materialStateListener = materialStateListener;

        auto state = factoryManager->make_ptr<GraphicsMeshState>();
        stateContext->addState( state );
        stateContext->setOwner( this );

        setGraphicsObjectState( state );
    }

    void CGraphicsMeshOgreNext::setSkeleton( SmartPtr<ISkeleton> skeleton )
    {
        m_skeleton = skeleton;
    }

    auto CGraphicsMeshOgreNext::getSkeleton() const -> SmartPtr<ISkeleton>
    {
        return m_skeleton;
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

        state->setDirty( false );
    }

    void CGraphicsMeshOgreNext::MeshStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    CGraphicsMeshOgreNext::MeshStateListener::~MeshStateListener() = default;

    CGraphicsMeshOgreNext::MeshStateListener::MeshStateListener( CGraphicsMeshOgreNext *owner )
    {
        setOwner( owner );
    }

    CGraphicsMeshOgreNext::MaterialStateListener::MaterialStateListener( CGraphicsMeshOgreNext *owner )
    {
    }

    CGraphicsMeshOgreNext::MaterialStateListener::~MaterialStateListener() = default;

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

    auto CGraphicsMeshOgreNext::MaterialStateListener::getOwner() const
        -> SmartPtr<CGraphicsMeshOgreNext>
    {
        auto p = m_owner.load();
        return p.lock();
    }

    void CGraphicsMeshOgreNext::MaterialStateListener::setOwner( SmartPtr<CGraphicsMeshOgreNext> owner )
    {
        m_owner = owner;
    }

    CGraphicsMeshOgreNext::MaterialEventListener::~MaterialEventListener() = default;

    CGraphicsMeshOgreNext::MaterialEventListener::MaterialEventListener() = default;

    void CGraphicsMeshOgreNext::MaterialEventListener::setOwner( SmartPtr<CGraphicsMeshOgreNext> owner )
    {
        m_owner = owner;
    }

    auto CGraphicsMeshOgreNext::MaterialEventListener::getOwner() const
        -> SmartPtr<CGraphicsMeshOgreNext>
    {
        return m_owner;
    }

    auto CGraphicsMeshOgreNext::MaterialEventListener::handleEvent( IEvent::Type eventType,
                                                                    hash_type eventValue,
                                                                    const Array<Parameter> &arguments,
                                                                    SmartPtr<ISharedObject> sender,
                                                                    SmartPtr<ISharedObject> object,
                                                                    SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::loadingStateChanged )
        {
            if( auto owner = getOwner() )
            {
                if( owner->isLoaded() )
                {
                    auto materialObject = fb::static_pointer_cast<IMaterial>( sender );
                    owner->materialLoaded( materialObject );
                }
            }
        }

        return {};
    }
}  // namespace fb::render
