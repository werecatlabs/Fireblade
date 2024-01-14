#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Terrain/TerrainSystem.h>
#include <FBCore/Scene/Components/Terrain/TerrainLayer.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Graphics/ITerrain.h>
#include <FBCore/Interface/Graphics/ITerrainBlendMap.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, TerrainSystem, Component );

    TerrainSystem::TerrainSystem() = default;

    TerrainSystem::~TerrainSystem()
    {
        unload( nullptr );
    }

    void TerrainSystem::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Component::load( data );

            auto terrainLayer = addSubComponentByType<SubComponent>();
            auto treesLayer = addSubComponentByType<SubComponent>();
            auto grassLayer = addSubComponentByType<SubComponent>();

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TerrainSystem::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    if( m_terrain )
                    {
                        m_terrain->setVisible( false );

                        if( m_terrainListener )
                        {
                            m_terrain->removeObjectListener( m_terrainListener.get() );
                            m_terrainListener = nullptr;
                        }

                        smgr->destroyTerrain( m_terrain );
                    }
                }

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void TerrainSystem::updateFlags( u32 flags, u32 oldFlags )
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
                auto visible = isEnabled() && actor->isEnabledInScene();

                if( BitUtil::getFlagValue( flags, IActor::ActorFlagInScene ) !=
                    BitUtil::getFlagValue( oldFlags, IActor::ActorFlagInScene ) )
                {
                    if( m_terrain )
                    {
                        m_terrain->setVisible( visible );
                    }
                }
                else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                         BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                {
                    if( m_terrain )
                    {
                        m_terrain->setVisible( visible );
                    }
                }
            }
        }
    }

    auto TerrainSystem::getProperties() const -> SmartPtr<Properties>
    {
        if( auto properties = Component::getProperties() )
        {
            properties->setProperty( "layers", 0 );
            properties->setProperty( "heightMap", m_heightMap );

            return properties;
        }

        return nullptr;
    }

    void TerrainSystem::setProperties( SmartPtr<Properties> properties )
    {
        properties->getPropertyValue( "heightMap", m_heightMap );

        // auto diffuseColour = getDiffuseColour();
        // properties->getPropertyValue( "diffuseColour", diffuseColour );

        // if( m_diffuseColour != diffuseColour )
        //{
        //     if( m_light )
        //     {
        //         m_light->setDiffuseColour( diffuseColour );
        //     }

        //    m_diffuseColour = diffuseColour;
        //}

        updateVisibility();

        if( m_terrain )
        {
            m_terrain->setHeightMap( m_heightMap );
        }
    }

    void TerrainSystem::updateTransform()
    {
        if( auto actor = getActor() )
        {
#if _DEBUG
            auto handle = actor->getHandle();
            if( handle )
            {
                auto name = handle->getName();
                if( name == "F40f40" )
                {
                    int stop = 0;
                    stop = 0;
                }
            }
#endif

            if( auto actorTransform = actor->getTransform() )
            {
                auto worldTransform = actorTransform->getWorldTransform();

                if( m_terrain )
                {
                    m_terrain->setWorldTransform( worldTransform );
                }
            }
        }
    }

    auto TerrainSystem::calculateNumLayers() const -> s32
    {
        auto terrainLayers = getSubComponentsByType<TerrainLayer>();
        return static_cast<s32>( terrainLayers.size() );
    }

    auto TerrainSystem::getNumLayers() const -> s32
    {
        return m_numLayers;
    }

    auto TerrainSystem::addLayer() -> SmartPtr<TerrainLayer>
    {
        auto numLayers = calculateNumLayers();
        auto newNumLayers = numLayers + 1;

        setNumLayers( newNumLayers );
        resizeLayermap();

        auto layers = getSubComponentsByType<TerrainLayer>();
        return layers[numLayers];
    }

    void TerrainSystem::removeLayer( s32 index )
    {
        if( index < 0 || index >= m_numLayers )
        {
            return;
        }

        m_layers.erase( m_layers.begin() + index );
    }

    void TerrainSystem::removeLayer( SmartPtr<TerrainLayer> layer )
    {
        auto layers = getSubComponentsByType<TerrainLayer>();
        auto it = std::find( layers.begin(), layers.end(), layer );
        if( it != layers.end() )
        {
            layers.erase( it );
        }
    }

    void TerrainSystem::setNumLayers( s32 numLayers )
    {
        if( m_numLayers != numLayers )
        {
            m_numLayers = numLayers;
            resizeLayermap();
        }
    }

    void TerrainSystem::createTerrain()
    {
        auto terrainListener = fb::make_ptr<TerrainSharedObjectListener>();
        terrainListener->setOwner( this );
        m_terrainListener = terrainListener;

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto smgr = graphicsSystem->getGraphicsScene();
        FB_ASSERT( smgr );

        m_terrain = smgr->createTerrain();
        FB_ASSERT( m_terrain );

        m_terrain->addObjectListener( m_terrainListener );
    }

    void TerrainSystem::updateVisibility()
    {
        if( auto actor = getActor() )
        {
            auto visible = isEnabled() && actor->isEnabledInScene();

            if( m_terrain )
            {
                m_terrain->setVisible( visible );
            }
        }
    }

    auto TerrainSystem::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
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
                createTerrain();
                updateVisibility();
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

    void TerrainSystem::resizeLayermap()
    {
        //m_layermap.resize( m_num_layers );
        m_layers.resize( getNumLayers() );

        auto numLayers = getNumLayers();
        auto subComponents = getSubComponentsByType<TerrainLayer>();
        if( numLayers >= subComponents.size() )
        {
            auto diff = numLayers - subComponents.size();
            for( size_t i = 0; i < diff; ++i )
            {
                auto terrainLayer = addSubComponentByType<TerrainLayer>();
            }
        }
        else if( numLayers < subComponents.size() )
        {
            auto diff = subComponents.size() - numLayers;
            for( size_t i = 0; i < diff; ++i )
            {
                auto index = getNumSubComponents() - 1;
                removeSubComponentByIndex( index );
            }
        }

        auto index = 0;
        auto layers = getSubComponentsByType<TerrainLayer>();
        for( auto &layer : layers )
        {
            layer->setIndex( index );
            ++index;
        }
    }

    void TerrainSystem::updateLayers()
    {
        if( m_terrain )
        {
            auto layers = getSubComponentsByType<TerrainLayer>();
            for( auto layer : layers )
            {
                auto index = layer->getIndex();
                if( auto tex = layer->getBaseTexture() )
                {
                    auto textureFilePath = tex->getFilePath();
                    m_terrain->setTextureLayer( index, textureFilePath );
                }
            }
        }
    }

    auto TerrainSystem::getHeightMap() const -> SmartPtr<render::ITexture>
    {
        return m_heightMap;
    }

    void TerrainSystem::setHeightMap( SmartPtr<render::ITexture> heightMap )
    {
        m_heightMap = heightMap;

        if( m_terrain )
        {
            m_terrain->setHeightMap( m_heightMap );
        }
    }

    TerrainSystem::TerrainSharedObjectListener::TerrainSharedObjectListener() = default;

    TerrainSystem::TerrainSharedObjectListener::~TerrainSharedObjectListener() = default;

    auto TerrainSystem::TerrainSharedObjectListener::handleEvent( IEvent::Type eventType,
                                                                  hash_type eventValue,
                                                                  const Array<Parameter> &arguments,
                                                                  SmartPtr<ISharedObject> sender,
                                                                  SmartPtr<ISharedObject> object,
                                                                  SmartPtr<IEvent> event ) -> Parameter
    {
        auto newState = static_cast<LoadingState>( arguments[1].getS32() );
        if( newState == LoadingState::Loaded )
        {
            if( auto owner = getOwner() )
            {
                owner->updateLayers();
            }
        }

        return {};
    }

    void TerrainSystem::TerrainSharedObjectListener::loadingStateChanged( ISharedObject *sharedObject,
                                                                          LoadingState oldState,
                                                                          LoadingState newState )
    {
        if( newState == LoadingState::Loaded )
        {
            if( auto owner = getOwner() )
            {
                owner->updateLayers();
            }
        }
    }

    auto TerrainSystem::TerrainSharedObjectListener::destroy( void *ptr ) -> bool
    {
        return false;
    }

    auto TerrainSystem::TerrainSharedObjectListener::getOwner() const -> TerrainSystem *
    {
        return m_owner;
    }

    void TerrainSystem::TerrainSharedObjectListener::setOwner( TerrainSystem *owner )
    {
        m_owner = owner;
    }
}  // namespace fb::scene
