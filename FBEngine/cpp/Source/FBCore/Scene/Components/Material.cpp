#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/Scene/Components/Skybox.h>
#include <FBCore/Scene/Components/MeshRenderer.h>
#include <FBCore/Scene/Components/UI/Image.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IMaterialManager.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITaskLock.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/State/Messages/StateMessageLoad.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/FileInfo.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Material, Component );

        Material::Material()
        {
            auto materialListener = fb::make_ptr<MaterialStateListener>();
            materialListener->setOwner( this );
            m_materialListener = materialListener;
        }

        Material::~Material()
        {
            unload( nullptr );
        }

        void Material::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                Component::load( data );

                auto materialObjectListener = fb::make_ptr<MaterialStateObjectListener>();
                materialObjectListener->setOwner( this );
                setMaterialObjectListener( materialObjectListener );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Material::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    m_material = nullptr;
                    m_materialListener = nullptr;
                    Component::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Material::updateDirty( u32 flags, u32 oldFlags )
        {
            if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
            {
                updateMaterial();
                updateImageComponent();
                updateDependentComponents();
                updateDirty();
            }
            else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                     BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
            {
                updateMaterial();
                updateImageComponent();
                updateDependentComponents();
                updateDirty();
            }
        }

        IFSM::ReturnType Material::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            Component::handleComponentEvent( state, eventType );

            switch( eventType )
            {
            case IFSM::Event::Change:
                break;
            case IFSM::Event::Enter:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Edit:
                case State::Play:
                {
                    updateMaterial();
                    updateImageComponent();
                    updateDependentComponents();
                    updateDirty();
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
                case State::Play:
                {
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

        SmartPtr<Properties> Material::getProperties() const
        {
            auto properties = Component::getProperties();
            FB_ASSERT( properties );

            static const auto mainTextureStr = String( "Main Texture" );
            static const auto materialPathStr = String( "Material" );

            properties->setProperty( materialPathStr, m_material );
            properties->setProperty( "index", m_index );

            return properties;
        }

        void Material::setProperties( SmartPtr<Properties> properties )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto taskManager = applicationManager->getTaskManager();

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                static const auto materialPathStr = String( "Material" );

                SmartPtr<render::IMaterial> material;
                properties->getPropertyValue( materialPathStr, material );
                properties->getPropertyValue( "index", m_index );

                if( material )
                {
                    auto renderLock = taskManager->lockTask( Thread::Task::Render );

                    auto filePath = String();

                    auto fileSystemId = material->getFileSystemId();
                    if( fileSystemId != 0 )
                    {
                        FileInfo fileInfo;
                        if( fileSystem->findFileInfo( fileSystemId, fileInfo ) )
                        {
                            filePath = fileInfo.filePath;
                        }
                    }

                    if( StringUtil::isNullOrEmpty( filePath ) )
                    {
                        filePath = material->getFilePath();
                    }

                    if( !material->isLoaded() )
                    {
                        material->loadFromFile( filePath );
                    }

                    if( !material->isLoaded() )
                    {
                        ScopedLock lock (graphicsSystem);
                        material->load( nullptr);
                    }
                }

                if( m_material != material )
                {
                    m_material = material;

                    //if( material )
                    {
                        //if( material->isLoaded() )
                        {
                            updateImageComponent();
                            updateDependentComponents();
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Material::updateDirty()
        {
            updateMaterial();

            if( auto actor = getActor() )
            {
                auto meshRenderer = actor->getComponent<MeshRenderer>();
                if( meshRenderer )
                {
                    meshRenderer->updateMaterials();
                }

                auto uiComponent = actor->getComponent<UIComponent>();
                if( uiComponent )
                {
                    uiComponent->updateMaterials();
                }

                auto skybox = actor->getComponent<Skybox>();
                if( skybox )
                {
                    skybox->updateMaterials();
                }
            }
        }

        Array<SmartPtr<ISharedObject>> Material::getChildObjects() const
        {
            Array<SmartPtr<ISharedObject>> objects;

            if( m_material )
            {
                objects.push_back( m_material );
            }

            return objects;
        }

        String Material::getMaterialPath() const
        {
            if( auto material = getMaterial() )
            {
                if( auto materialHandle = material->getHandle() )
                {
                    return materialHandle->getName();
                }
            }

            return "";
        }

        void Material::setMaterialPath( const String &val )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            if( resourceDatabase )
            {
                auto material = resourceDatabase->createOrRetrieveByType<render::IMaterial>( val );
                setMaterial( material.first );
            }
        }

        SmartPtr<render::IMaterial> Material::getMaterial() const
        {
            return m_material;
        }

        void Material::setMaterial( SmartPtr<render::IMaterial> material )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            if( m_material != material )
            {
                if( m_material )
                {
                    FB_ASSERT( getMaterial() && getMaterial()->isValid() );

                    auto materialObjectListener = getMaterialObjectListener();
                    m_material->removeObjectListener( materialObjectListener );

                    auto materialStateObject = m_material->getStateObject();
                    if( materialStateObject )
                    {
                        materialStateObject->removeStateListener( m_materialListener );
                    }
                }

                m_material = material;

                if( m_material )
                {
                    FB_ASSERT( getMaterial() && getMaterial()->isValid() );

                    auto materialObjectListener = getMaterialObjectListener();
                    m_material->addObjectListener( materialObjectListener );

                    auto materialStateObject = m_material->getStateObject();
                    if( materialStateObject )
                    {
                        materialStateObject->addStateListener( m_materialListener );
                    }

                    graphicsSystem->loadObject( m_material );
                }

                updateMaterial();
                updateImageComponent();
                updateDependentComponents();
            }
        }

        void Material::updateMaterial()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();

            auto material = getMaterial();
            if( !material )
            {
                if( graphicsSystem )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( graphicsSystem );

                    auto materialManager = graphicsSystem->getMaterialManager();
                    FB_ASSERT( materialManager );

                    if( materialManager )
                    {
                        auto materialPath = getMaterialPath();
                        if( !StringUtil::isNullOrEmpty( materialPath ) )
                        {
                            auto newMaterial = materialManager->loadFromFile( materialPath );
                            if( !newMaterial )
                            {
                                FB_LOG_ERROR( "Could not load material: " + materialPath );
                            }

                            setMaterial( newMaterial );
                        }
                    }
                }
            }
            else
            {
                if( !material->isLoaded() )
                {
                    auto materialPath = getMaterialPath();
                    if( !StringUtil::isNullOrEmpty( materialPath ) )
                    {
                        material->loadFromFile( materialPath );
                    }
                }
            }
        }

        void Material::updateDependentComponents()
        {
            auto actor = getActor();
            if( actor )
            {
                auto components = actor->getComponents();
                for( auto component : components )
                {
                    component->updateMaterials();
                }
            }
        }

        void Material::updateImageComponent()
        {
            if( auto material = getMaterial() )
            {
                if( auto actor = getActor() )
                {
                    auto imageComponent = actor->getComponent<Image>();
                    if( imageComponent )
                    {
                        if( auto imageTexture = imageComponent->getTexture() )
                        {
                            material->setTexture( imageTexture, 0 );
                        }
                    }
                }
            }
        }

        u32 Material::getIndex() const
        {
            return m_index;
        }

        void Material::setIndex( u32 index )
        {
            m_index = index;
        }

        SmartPtr<IEventListener> Material::getMaterialObjectListener() const
        {
            return m_materialObjectListener;
        }

        void Material::setMaterialObjectListener( SmartPtr<IEventListener> materialObjectListener )
        {
            m_materialObjectListener = materialObjectListener;
        }

        Material::MaterialStateObjectListener::MaterialStateObjectListener()
        {
        }

        Material::MaterialStateObjectListener::~MaterialStateObjectListener()
        {
        }

        SmartPtr<Material> Material::MaterialStateObjectListener::getOwner() const
        {
            return m_owner;
        }

        void Material::MaterialStateObjectListener::setOwner( SmartPtr<Material> owner )
        {
            m_owner = owner;
        }

        Material::MaterialStateListener::MaterialStateListener( Material *owner )
        {
            m_owner = owner;
        }

        Material::MaterialStateListener::~MaterialStateListener()
        {
        }

        Parameter Material::MaterialStateObjectListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto task = Thread::getCurrentTask();
            if( task == Thread::Task::Application )
            {
                if( eventValue == IEvent::loadingStateChanged )
                {
                    if( arguments[1].getS32() == (u32)LoadingState::Loaded )
                    {
                        auto owner = getOwner();
                        if( owner )
                        {
                            if( owner->getMaterial() == sender )
                            {
                                if( owner->getLoadingState() == LoadingState::Loaded )
                                {
                                    //owner->updateMaterial();
                                    owner->updateImageComponent();
                                    owner->updateDependentComponents();
                                }
                            }
                        }
                    }
                }
            }

            return Parameter();
        }

        void Material::MaterialStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            auto owner = getOwner();
            if( owner )
            {
                if( owner->isLoaded() )
                {
                    //owner->updateMaterial();
                    //owner->updateImageComponent();
                    //owner->updateDependentComponents();
                }
            }
        }

        void Material::MaterialStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( message->isExactly<StateMessageLoad>() )
            {
                auto messageLoad = fb::static_pointer_cast<StateMessageLoad>( message );
                auto messageType = messageLoad->getType();

                if( messageType == StateMessageLoad::LOADED_HASH )
                {
                    auto owner = getOwner();
                    if( owner )
                    {
                        if( owner->getLoadingState() == LoadingState::Loaded )
                        {
                            owner->updateImageComponent();
                            owner->updateDependentComponents();
                        }
                    }
                }
            }
        }

        void Material::MaterialStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        SmartPtr<Material> Material::MaterialStateListener::getOwner() const
        {
            return m_owner;
        }

        void Material::MaterialStateListener::setOwner( SmartPtr<Material> owner )
        {
            m_owner = owner;
        }
    }  // namespace scene
}  // end namespace fb
