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

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, MeshRenderer, IComponent );

        MeshRenderer::MeshRenderer()
        {
        }

        MeshRenderer::~MeshRenderer()
        {
            unload( nullptr );
        }

        void MeshRenderer::updateDirty( u32 flags, u32 oldFlags )
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
                        if( !m_meshObject )
                        {
                            updateMesh();
                            updateMaterials();
                            updateTransform();
                        }

                        auto visible = isEnabled() && actor->isEnabledInScene();
                        if( visible )
                        {
                            if( m_graphicshNode )
                            {
                                auto parent = m_graphicshNode->getParent();
                                if( !parent )
                                {
                                    rootNode->addChild( m_graphicshNode );
                                }

                                m_graphicshNode->setVisible( true );
                            }
                        }
                        else
                        {
                            if( m_graphicshNode )
                            {
                                auto parent = m_graphicshNode->getParent();
                                if( parent )
                                {
                                    parent->removeChild( m_graphicshNode );
                                }

                                m_graphicshNode->setVisible( false );
                            }
                        }
                    }
                    else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                             BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                    {
                        auto visible = isEnabled() && actor->isEnabledInScene();
                        if( visible )
                        {
                            if( m_graphicshNode )
                            {
                                auto parent = m_graphicshNode->getParent();
                                if( !parent )
                                {
                                    rootNode->addChild( m_graphicshNode );
                                }

                                m_graphicshNode->setVisible( true );
                            }
                        }
                        else
                        {
                            if( m_graphicshNode )
                            {
                                auto parent = m_graphicshNode->getParent();
                                if( parent )
                                {
                                    parent->removeChild( m_graphicshNode );
                                }

                                m_graphicshNode->setVisible( false );
                            }
                        }
                    }
                }
            }
        }

        void MeshRenderer::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                auto sceneManager = applicationManager->getSceneManager();

                Component::load( data );

                sceneManager->registerComponentUpdate( Thread::Task::Render,
                                                       Thread::UpdateState::Transform, this );

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

                    auto applicationManager = core::IApplicationManager::instance();
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
                        }
                        else
                        {
                            setMeshObject( nullptr );
                            setMeshNode( nullptr );
                        }
                    }

                    sceneManager->unregisterAllComponent( this );

                    Component::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Array<SmartPtr<ISharedObject>> MeshRenderer::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( 4 );

            objects.push_back( m_meshObject );
            objects.push_back( m_graphicshNode );
            objects.push_back( m_sharedMaterial );
            return objects;
        }

        SmartPtr<Properties> MeshRenderer::getProperties() const
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
            s32 castShadows = 0;
            properties->getPropertyValue( "castShadows", castShadows );
            m_castShadows = static_cast<CastShadows>( castShadows );
        }

        SmartPtr<render::IMaterial> MeshRenderer::getSharedMaterial() const
        {
            return m_sharedMaterial;
        }

        void MeshRenderer::setSharedMaterial( SmartPtr<render::IMaterial> sharedMaterial )
        {
            m_sharedMaterial = sharedMaterial;
        }

        String MeshRenderer::getMaterialName() const
        {
            return m_materialName;
        }

        void MeshRenderer::setMaterialName( const String &materialName )
        {
            m_materialName = materialName;
        }

        IFSM::ReturnType MeshRenderer::handleComponentEvent( u32 state, IFSM::Event eventType )
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

                    if( auto actor = getActor() )
                    {
                        if( actor->getName().find( "poly" ) != String::npos )
                        {
                            int stop = 0;
                            stop = 0;
                        }

                        auto visible = isEnabled() && actor->isEnabledInScene();

                        if( auto meshNode = getMeshNode() )
                        {
                            meshNode->setVisible( visible );
                        }
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
                auto applicationManager = core::IApplicationManager::instance();
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

                                //// todo refactor
                                // auto materialComponent =
                                // actor->getComponent<scene::MaterialComponent>(); if
                                // (materialComponent)
                                //{
                                //	auto material = materialComponent->getMaterial();
                                //	if (material)
                                //	{
                                //		if (m_meshObject)
                                //		{
                                //			m_meshObject->setMaterial(material);
                                //		}
                                //	}
                                // }

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

        void MeshRenderer::visibilityChanged()
        {
            if( auto actor = getActor() )
            {
                auto visisble = actor->isVisible();
                m_graphicshNode->setVisible( visisble );
            }
        }

        void MeshRenderer::updateTransform()
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

                    if( m_graphicshNode )
                    {
                        m_graphicshNode->setPosition( actorPosition );
                        m_graphicshNode->setOrientation( actorOrientation );
                        m_graphicshNode->setScale( actorScale );
                    }
                }
            }
        }

        SmartPtr<render::IGraphicsMesh> MeshRenderer::getMeshObject() const
        {
            return m_meshObject;
        }

        void MeshRenderer::setMeshObject( SmartPtr<render::IGraphicsMesh> meshObject )
        {
            m_meshObject = meshObject;
        }

        SmartPtr<render::ISceneNode> MeshRenderer::getMeshNode() const
        {
            return m_graphicshNode;
        }

        void MeshRenderer::setMeshNode( SmartPtr<render::ISceneNode> meshNode )
        {
            m_graphicshNode = meshNode;
        }
    }  // namespace scene
}  // end namespace fb
