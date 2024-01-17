#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/MeshRenderer.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Scene/Components/Material.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsMesh.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/ISceneNode.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/State/Messages/StateMessageLoad.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, MeshRenderer, IComponent );

    MeshRenderer::MeshRenderer() = default;

    MeshRenderer::~MeshRenderer()
    {
        unload( nullptr );
    }

    void MeshRenderer::updateFlags( u32 flags, u32 oldFlags )
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
                    if( !m_meshObject )
                    {
                        updateMesh();
                        updateMaterials();
                        updateTransform();
                    }

                    auto visible = isEnabled() && actor->isEnabledInScene();
                    if( visible )
                    {
                        if( m_graphicsNode )
                        {
                            auto parent = m_graphicsNode->getParent();
                            if( !parent )
                            {
                                rootNode->addChild( m_graphicsNode );
                            }

                            m_graphicsNode->setVisible( true );
                        }
                    }
                    else
                    {
                        if( m_graphicsNode )
                        {
                            auto parent = m_graphicsNode->getParent();
                            if( parent )
                            {
                                parent->removeChild( m_graphicsNode );
                            }

                            m_graphicsNode->setVisible( false );
                        }
                    }
                }
                else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                         BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                {
                    auto visible = isEnabled() && actor->isEnabledInScene();
                    if( visible )
                    {
                        if( m_graphicsNode )
                        {
                            auto parent = m_graphicsNode->getParent();
                            if( !parent )
                            {
                                rootNode->addChild( m_graphicsNode );
                            }

                            m_graphicsNode->setVisible( true );
                        }
                    }
                    else
                    {
                        if( m_graphicsNode )
                        {
                            auto parent = m_graphicsNode->getParent();
                            if( parent )
                            {
                                parent->removeChild( m_graphicsNode );
                            }

                            m_graphicsNode->setVisible( false );
                        }
                    }
                }
            }
        }

        Renderer::updateFlags( flags, oldFlags );
    }

    void MeshRenderer::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            auto sceneManager = applicationManager->getSceneManager();

            Renderer::load( data );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshRenderer::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto sceneManager = applicationManager->getSceneManager();

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                if( graphicsSystem )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    if( auto meshNode = getMeshNode() )
                    {
                        if( auto meshObject = getMeshObject() )
                        {
                            meshNode->detachObject( meshObject );
                            smgr->removeGraphicsObject( meshObject );
                            setMeshObject( nullptr );
                        }

                        meshNode->detachAllObjects();
                        smgr->removeSceneNode( meshNode );
                        setMeshNode( nullptr );
                        setGraphicsObject( nullptr );
                    }
                    else
                    {
                        setMeshObject( nullptr );
                        setMeshNode( nullptr );
                        setGraphicsObject( nullptr );
                    }
                }

                if( sceneManager )
                {
                    sceneManager->unregisterAllComponent( this );
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

    auto MeshRenderer::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        auto objects = Array<SmartPtr<ISharedObject>>();
        objects.reserve( 4 );

        objects.emplace_back( m_meshObject );
        objects.emplace_back( m_graphicsNode );
        objects.emplace_back( m_sharedMaterial );
        return objects;
    }

    auto MeshRenderer::getProperties() const -> SmartPtr<Properties>
    {
        if( auto properties = Renderer::getProperties() )
        {
            properties->setProperty( "castShadows", static_cast<s32>( m_castShadows ) );

            return properties;
        }

        return nullptr;
    }

    void MeshRenderer::setProperties( SmartPtr<Properties> properties )
    {
        Renderer::setProperties( properties );

        updateVisibility();
    }

    auto MeshRenderer::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
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
            case State::Destroyed:
            {
            }
            break;
            case State::Edit:
            case State::Play:
            {
                if( !m_meshObject )
                {
                    updateMesh();
                    updateMaterials();
                    updateTransform();
                }

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
                // if (!m_meshObject)
                //{
                //	updateMesh();
                //	updateMaterials();
                //	updateTransform();
                // }
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

    void MeshRenderer::updateMesh()
    {
        try
        {
            if( !getMeshObject() )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                if( graphicsSystem )
                {
                    auto smgr = graphicsSystem->getGraphicsScene();
                    FB_ASSERT( smgr );

                    if( auto actor = getActor() )
                    {
                        auto meshComponent = actor->getComponent<Mesh>();
                        if( meshComponent )
                        {
                            auto meshPath = meshComponent->getMeshPath();
                            if( !StringUtil::isNullOrEmpty( meshPath ) )
                            {
                                auto meshObject = smgr->addMesh( meshPath );
                                if( !meshObject )
                                {
                                    FB_LOG( "Mesh not loaded: " + meshPath );
                                }

                                setMeshObject( meshObject );
                                setGraphicsObject( meshObject );
                                ;

                                if( meshObject )
                                {
                                    auto actorName = actor->getName();
                                    auto nodeName = actorName + String( "_MeshComponent" ) +
                                                    StringUtil::toString( m_idExt++ );

                                    auto rootNode = smgr->getRootSceneNode();
                                    auto meshNode = rootNode->addChildSceneNode( nodeName );

                                    if( meshNode )
                                    {
                                        meshNode->attachObject( meshObject );
                                    }

                                    meshObject->setVisibilityFlags( render::IGraphicsObject::SceneFlag );

                                    setMeshNode( meshNode );
                                }
                            }
                        }
                    }
                }
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void MeshRenderer::updateMaterials()
    {
        if( auto actor = getActor() )
        {
            auto materials = actor->getComponentsByType<Material>();
            for( auto material : materials )
            {
                auto materialResource = material->getMaterial();
                if( materialResource )
                {
                    if( m_meshObject )
                    {
                        auto index = material->getIndex();
                        m_meshObject->setMaterial( materialResource, index );
                    }
                }
            }
        }
    }

    void MeshRenderer::updateTransform()
    {
        if( auto actor = getActor() )
        {
            if( auto actorTransform = actor->getTransform() )
            {
                auto actorPosition = actorTransform->getPosition();
                auto actorOrientation = actorTransform->getOrientation();
                auto actorScale = actorTransform->getScale();

                FB_ASSERT( actorPosition.isValid() );
                FB_ASSERT( actorOrientation.isSane() );
                FB_ASSERT( actorScale.isValid() );

                if( m_graphicsNode )
                {
                    m_graphicsNode->setPosition( actorPosition );
                    m_graphicsNode->setOrientation( actorOrientation );
                    m_graphicsNode->setScale( actorScale );
                }
            }
        }
    }

    auto MeshRenderer::getMeshObject() const -> SmartPtr<render::IGraphicsMesh>
    {
        return m_meshObject;
    }

    void MeshRenderer::setMeshObject( SmartPtr<render::IGraphicsMesh> meshObject )
    {
        m_meshObject = meshObject;
    }

    auto MeshRenderer::getMeshNode() const -> SmartPtr<render::ISceneNode>
    {
        return m_graphicsNode;
    }

    void MeshRenderer::setMeshNode( SmartPtr<render::ISceneNode> meshNode )
    {
        m_graphicsNode = meshNode;
    }
}  // namespace fb::scene
