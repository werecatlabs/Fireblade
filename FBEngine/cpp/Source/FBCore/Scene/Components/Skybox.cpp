#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Skybox.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Skybox, Component );

    Skybox::Skybox()
    {
        m_materialSharedListener = fb::make_ptr<MaterialSharedListener>();
        m_materialSharedListener->setOwner( this );
    }

    Skybox::~Skybox()
    {
        unload( nullptr );
    }

    void Skybox::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Component::load( data );

            m_textures.resize( 6 );

            setupMaterial();

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( graphicsSystem )
            {
                if( auto actor = getActor() )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    auto visible = actor->isEnabledInScene();

                    auto skyboxMaterial = getMaterial();
                    smgr->setSkyBox( visible, skyboxMaterial, 50000.0f );
                }
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Skybox::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                if( graphicsSystem )
                {
                    if( auto actor = getActor() )
                    {
                        auto smgr = graphicsSystem->getGraphicsScene();
                        FB_ASSERT( smgr );

                        auto distance = getDistance();

                        auto skyboxMaterial = getMaterial();
                        smgr->setSkyBox( false, skyboxMaterial, distance );
                    }
                }

                if( m_materialSharedListener )
                {
                    if( m_material )
                    {
                        m_material->removeObjectListener( m_materialSharedListener.get() );
                    }
                }

                m_material = nullptr;
                m_textures.clear();
                m_materialSharedListener = nullptr;

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Skybox::updateFlags( u32 flags, u32 oldFlags )
    {
        if( auto actor = getActor() )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( graphicsSystem )
            {
                auto smgr = graphicsSystem->getGraphicsScene();
                FB_ASSERT( smgr );

                auto rootNode = smgr->getRootSceneNode();

                if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                    BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
                {
                    switch( auto state = actor->getState() )
                    {
                    case IActor::State::Play:
                    case IActor::State::Edit:
                    {
                        auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled );

                        if( graphicsSystem )
                        {
                            auto smgr = graphicsSystem->getGraphicsScene();
                            FB_ASSERT( smgr );

                            auto distance = getDistance();

                            auto skyboxMaterial = getMaterial();
                            smgr->setSkyBox( visible, skyboxMaterial, distance );
                        }
                    }
                    break;
                    default:
                    {
                    }
                    }
                }

                if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                    BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                {
                    switch( auto state = actor->getState() )
                    {
                    case IActor::State::Play:
                    case IActor::State::Edit:
                    {
                        setupMaterial();

                        auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled );

                        if( graphicsSystem )
                        {
                            auto smgr = graphicsSystem->getGraphicsScene();
                            FB_ASSERT( smgr );

                            auto distance = getDistance();

                            auto skyboxMaterial = getMaterial();
                            smgr->setSkyBox( visible, skyboxMaterial, distance );
                        }
                    }
                    break;
                    default:
                    {
                    }
                    }
                }

                if( BitUtil::getFlagValue( flags, IActor::ActorFlagDirty ) !=
                    BitUtil::getFlagValue( oldFlags, IActor::ActorFlagDirty ) )
                {
                    switch( auto state = actor->getState() )
                    {
                    case IActor::State::Play:
                    case IActor::State::Edit:
                    {
                        setupMaterial();

                        auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled );

                        if( graphicsSystem )
                        {
                            auto smgr = graphicsSystem->getGraphicsScene();
                            FB_ASSERT( smgr );

                            auto distance = getDistance();

                            auto skyboxMaterial = getMaterial();
                            smgr->setSkyBox( visible, skyboxMaterial, distance );
                        }
                    }
                    break;
                    default:
                    {
                    }
                    }
                }
            }
        }
    }

    auto Skybox::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();

        properties->setProperty(
            "Front", m_textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Front )] );
        properties->setProperty(
            "Back", m_textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Back )] );
        properties->setProperty(
            "Up", m_textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Up )] );
        properties->setProperty(
            "Down", m_textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Down )] );
        properties->setProperty(
            "Right", m_textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Right )] );
        properties->setProperty(
            "Left", m_textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Left )] );

        properties->setProperty( "swapLeftRight", m_swapLeftRight );

        return properties;
    }

    void Skybox::setProperties( SmartPtr<Properties> properties )
    {
        auto actor = getActor();
        if( !actor )
        {
            return;
        }

        auto wasEnabled = isEnabled() && actor->isEnabledInScene();

        Component::setProperties( properties );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        Array<SmartPtr<render::ITexture>> textures;
        textures.resize( 6 );

        properties->getPropertyValue(
            "Front", textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Front )] );

        SmartPtr<render::ITexture> backTexture;
        properties->getPropertyValue(
            "Back", textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Back )] );
        properties->getPropertyValue(
            "Up", textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Up )] );
        properties->getPropertyValue(
            "Down", textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Down )] );
        properties->getPropertyValue(
            "Right", textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Right )] );
        properties->getPropertyValue(
            "Left", textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Left )] );

        auto swapLeftRight = getSwapLeftRight();

        properties->getPropertyValue( "swapLeftRight", swapLeftRight );

        auto dirty = false;

        if( swapLeftRight != getSwapLeftRight() )
        {
            dirty = true;
            setSwapLeftRight( swapLeftRight );
        }

        if( m_textures.size() != textures.size() )
        {
            m_textures.resize( 6 );
        }

        for( size_t i = 0; i < textures.size(); ++i )
        {
            if( m_textures[i] != textures[i] )
            {
                m_textures[i] = textures[i];
                dirty = true;
            }
        }

        auto enabled = isEnabled() && actor->isEnabledInScene();
        if( !dirty )
        {
            dirty = enabled != wasEnabled;
        }

        if( dirty )
        {
            setupMaterial();

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            if( graphicsSystem )
            {
                auto smgr = graphicsSystem->getGraphicsScene();
                FB_ASSERT( smgr );

                auto distance = getDistance();

                auto skyboxMaterial = getMaterial();
                smgr->setSkyBox( enabled, skyboxMaterial, distance );
            }
        }
    }

    auto Skybox::getMaterial() const -> SmartPtr<render::IMaterial>
    {
        return m_material;
    }

    void Skybox::updateMaterials()
    {
        try
        {
            if( auto actor = getActor() )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto smgr = graphicsSystem->getGraphicsScene();
                FB_ASSERT( smgr );

                auto visible = actor->isVisible();
                auto skyboxMaterial = getMaterial();
                auto distance = getDistance();

                //smgr->setSkyBox( visible, skyboxMaterial, distance );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Skybox::getSwapLeftRight() const -> bool
    {
        return m_swapLeftRight;
    }

    void Skybox::setSwapLeftRight( bool swapLeftRight )
    {
        m_swapLeftRight = swapLeftRight;
    }

    auto Skybox::getDistance() const -> f32
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_distance;
    }

    void Skybox::setDistance( f32 distance )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_distance = distance;
    }

    auto Skybox::getTextures() const -> Array<SmartPtr<render::ITexture>>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_textures;
    }

    void Skybox::setTextures( Array<SmartPtr<render::ITexture>> textures )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_textures = textures;
    }

    auto Skybox::getTexture( u8 index ) const -> SmartPtr<render::ITexture>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_textures[index];
    }

    void Skybox::setTexture( SmartPtr<render::ITexture> texture, u8 index )
    {
        if( texture )
        {
            if( index < m_textures.size() )
            {
                RecursiveMutex::ScopedLock lock( m_mutex );
                m_textures[index] = texture;
            }
        }

        setupMaterial();
    }

    void Skybox::setTextureByName( const String &textureName, u8 index )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto texture = resourceDatabase->loadResourceByType<render::ITexture>( textureName );
        if( texture )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_textures[index] = texture;
        }

        setupMaterial();
    }

    auto Skybox::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
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
            case State::Play:
            {
                if( auto actor = getActor() )
                {
                    setupMaterial();

                    auto applicationManager = core::ApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    if( graphicsSystem )
                    {
                        auto smgr = graphicsSystem->getGraphicsScene();
                        FB_ASSERT( smgr );

                        auto visible = actor->isEnabledInScene();
                        auto distance = getDistance();

                        if( auto skyboxMaterial = getMaterial() )
                        {
                            smgr->setSkyBox( visible, skyboxMaterial, distance );
                        }
                    }
                }
            }
            break;
            case State::Destroyed:
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                if( graphicsSystem )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    auto distance = getDistance();

                    auto skyboxMaterial = getMaterial();
                    //smgr->setSkyBox( false, skyboxMaterial, distance );
                }
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
            //auto eState = static_cast<State>( state );
            //switch( eState )
            //{
            //case State::Play:
            //{
            //}
            //break;
            //default:
            //{
            //}
            //}
        }
        break;
        case IFSM::Event::Pending:
        {
        }
        break;
        case IFSM::Event::Complete:
        {
        }
        break;
        case IFSM::Event::NewState:
        {
        }
        break;
        case IFSM::Event::WaitForChange:
        {
        }
        break;
        default:
        {
        }
        break;
        }

        return IFSM::ReturnType::Ok;
    }

    void Skybox::setupMaterial()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();
        FB_ASSERT( resourceDatabase );

        if( auto handle = getHandle() )
        {
            auto material = getMaterial();
            if( !material )
            {
                if( auto actor = getActor() )
                {
                    if( auto materialComponent = actor->getComponent<Material>() )
                    {
                        material = materialComponent->getMaterial();
                    }
                }
            }

            if( !material )
            {
                auto uuid = handle->getUUID();
                auto materialResult =
                    resourceDatabase->createOrRetrieveByType<render::IMaterial>( uuid );
                if( materialResult.first )
                {
                    material = materialResult.first;
                }

                if( materialResult.second )
                {
                    if( !material->hasObjectListener( m_materialSharedListener ) )
                    {
                        material->addObjectListener( m_materialSharedListener );
                    }
                }

                if( auto actor = getActor() )
                {
                    if( auto materialComponent = actor->getComponent<Material>() )
                    {
                        materialComponent->setMaterial( material );
                    }
                }
            }

            if( material )
            {
                material->setMaterialType( render::IMaterial::MaterialType::SkyboxCubemap );
                material->setLightingEnabled( false );

                auto textures = getTextures();
                if( getSwapLeftRight() )
                {
                    std::swap(
                        textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Left )],
                        textures[static_cast<u32>( render::IMaterial::SkyboxTextureTypes::Right )] );
                }

                material->setCubicTexture( textures );
            }

            m_material = material;
        }
    }

    auto Skybox::MaterialSharedListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                      const Array<Parameter> &arguments,
                                                      SmartPtr<ISharedObject> sender,
                                                      SmartPtr<ISharedObject> object,
                                                      SmartPtr<IEvent> event ) -> Parameter
    {
        return {};
    }

    void Skybox::MaterialSharedListener::loadingStateChanged( ISharedObject *sharedObject,
                                                              LoadingState oldState,
                                                              LoadingState newState )
    {
        if( newState == LoadingState::Loaded )
        {
            if( auto actor = m_owner->getActor() )
            {
                actor->setDirty( true );
            }
        }
    }

    auto Skybox::MaterialSharedListener::destroy( void *ptr ) -> bool
    {
        return false;
    }

    auto Skybox::MaterialSharedListener::getOwner() const -> SmartPtr<Skybox>
    {
        return m_owner.lock();
    }

    void Skybox::MaterialSharedListener::setOwner( SmartPtr<Skybox> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::scene
