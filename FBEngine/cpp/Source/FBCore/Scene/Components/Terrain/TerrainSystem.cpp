#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Terrain/TerrainSystem.h>
#include <FBCore/Scene/Components/Terrain/TerrainLayer.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, TerrainSystem, Component );

        TerrainSystem::TerrainSystem()
        {
        }

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
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
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

        void TerrainSystem::updateDirty( u32 flags, u32 oldFlags )
        {
            if( auto actor = getActor() )
            {
                auto applicationManager = core::IApplicationManager::instance();
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
                        auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagInScene );

                        auto applicationManager = core::IApplicationManager::instance();
                        FB_ASSERT( applicationManager );

                        auto graphicsSystem = applicationManager->getGraphicsSystem();
                        if( graphicsSystem )
                        {
                            auto smgr = graphicsSystem->getGraphicsScene();
                            FB_ASSERT( smgr );

                            if( m_terrain )
                            {
                                m_terrain->setVisible( visible );
                            }
                        }
                    }
                    else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                             BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                    {
                        auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled );

                        auto applicationManager = core::IApplicationManager::instance();
                        FB_ASSERT( applicationManager );

                        auto graphicsSystem = applicationManager->getGraphicsSystem();
                        if( graphicsSystem )
                        {
                            auto smgr = graphicsSystem->getGraphicsScene();
                            FB_ASSERT( smgr );

                            if( m_terrain )
                            {
                                m_terrain->setVisible( visible );
                            }
                        }
                    }
                }
            }
        }

        SmartPtr<Properties> TerrainSystem::getProperties() const
        {
            if( auto properties = Component::getProperties() )
            {
                properties->setProperty( "layers", 0 );

                return properties;
            }

            return nullptr;
        }

        void TerrainSystem::setProperties( SmartPtr<Properties> properties )
        {
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

                auto actorTransform = actor->getTransform();
                if( actorTransform )
                {
                    auto actorPosition = actorTransform->getPosition();
                    auto actorOrientation = actorTransform->getOrientation();
                    auto actorScale = actorTransform->getScale();                    

                    auto worldTransform = actorTransform->getWorldTransform();

                    if( m_terrain )
                    {
                        m_terrain->setWorldTransform( worldTransform );
                    }
                }
            }
        }

        s32 TerrainSystem::calculateNumLayers() const
        {
            auto terrainLayers = getSubComponentsByType<TerrainLayer>();
            return (s32)terrainLayers.size();
        }

        s32 TerrainSystem::getNumLayers() const
        {
            return m_numLayers;
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

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto smgr = graphicsSystem->getGraphicsScene();
            FB_ASSERT( smgr );

            m_terrain = smgr->createTerrain();
            FB_ASSERT( m_terrain );

            m_terrain->addObjectListener( m_terrainListener );

            graphicsSystem->loadObject( m_terrain );
        }

        IFSM::ReturnType TerrainSystem::handleComponentEvent( u32 state, IFSM::Event eventType )
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

        TerrainSystem::TerrainSharedObjectListener::TerrainSharedObjectListener()
        {
        }

        TerrainSystem::TerrainSharedObjectListener::~TerrainSharedObjectListener()
        {
        }

        Parameter TerrainSystem::TerrainSharedObjectListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto newState = (LoadingState)arguments[1].getS32();
            if( newState == LoadingState::Loaded )
            {
                if( auto owner = getOwner() )
                {
                    owner->updateLayers();
                }
            }

            return Parameter();
        }

        void TerrainSystem::TerrainSharedObjectListener::loadingStateChanged(
            ISharedObject *sharedObject, LoadingState oldState, LoadingState newState )
        {
            if( newState == LoadingState::Loaded )
            {
                if( auto owner = getOwner() )
                {
                    owner->updateLayers();
                }
            }
        }

        bool TerrainSystem::TerrainSharedObjectListener::destroy( void *ptr )
        {
            return false;
        }

        TerrainSystem *TerrainSystem::TerrainSharedObjectListener::getOwner() const
        {
            return m_owner;
        }

        void TerrainSystem::TerrainSharedObjectListener::setOwner( TerrainSystem *owner )
        {
            m_owner = owner;
        }
    }  // namespace scene
}  // end namespace fb
