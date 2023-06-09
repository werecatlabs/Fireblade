#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Renderer.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Scene/Components/Material.h>
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
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Core/BitUtil.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Renderer, IComponent );

        u32 Renderer::m_idExt = 0;

        Renderer::Renderer()
        {
        }

        Renderer::~Renderer()
        {
            unload( nullptr );
        }

        void Renderer::awake()
        {
            // if (!m_meshObject)
            //{
            //	updateMesh();
            //	updateMaterials();
            //	updateTransform();
            // }
        }

        void Renderer::play()
        {
            auto graphicsObject = getGraphicsObject();
            if( !graphicsObject )
            {
                updateMesh();
                updateMaterials();
                updateTransform();
            }
        }

        void Renderer::edit()
        {
            auto graphicsObject = getGraphicsObject();
            if( !graphicsObject )
            {
                updateMesh();
                updateMaterials();
                updateTransform();
            }
        }

        void Renderer::updateDirty( u32 flags, u32 oldFlags )
        {
            auto actor = getActor();
            if( actor )
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
                        auto graphicsObject = getGraphicsObject();
                        if( !graphicsObject )
                        {
                            updateMesh();
                            updateMaterials();
                            updateTransform();
                        }

                        auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagInScene );
                        if( visible )
                        {
                            auto graphicsNode = getGraphicsNode();
                            if( graphicsNode )
                            {
                                auto parent = graphicsNode->getParent();
                                if( !parent )
                                {
                                    rootNode->addChild( graphicsNode );
                                }

                                graphicsNode->setVisible( true );
                            }
                        }
                        else
                        {
                            auto graphicsNode = getGraphicsNode();
                            if( graphicsNode )
                            {
                                auto parent = graphicsNode->getParent();
                                if( parent )
                                {
                                    parent->removeChild( graphicsNode );
                                }

                                graphicsNode->setVisible( false );
                            }
                        }
                    }
                    else if( BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled ) !=
                             BitUtil::getFlagValue( oldFlags, IActor::ActorFlagEnabled ) )
                    {
                        auto visible = BitUtil::getFlagValue( flags, IActor::ActorFlagEnabled );
                        if( visible )
                        {
                            auto graphicsNode = getGraphicsNode();
                            if( graphicsNode )
                            {
                                auto parent = graphicsNode->getParent();
                                if( !parent )
                                {
                                    rootNode->addChild( graphicsNode );
                                }

                                graphicsNode->setVisible( true );
                            }
                        }
                        else
                        {
                            auto graphicsNode = getGraphicsNode();
                            if( graphicsNode )
                            {
                                auto parent = graphicsNode->getParent();
                                if( parent )
                                {
                                    parent->removeChild( graphicsNode );
                                }

                                graphicsNode->setVisible( false );
                            }
                        }
                    }
                }
            }
        }

        void Renderer::preUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateTask = applicationManager->getStateTask();
            auto applicationState = applicationManager->getApplicationTask();

            auto task = Thread::getCurrentTask();
            if( task == applicationState )
            {
                // if (!m_meshObject)
                //{
                //	updateMesh();
                //	updateMaterials();
                // }
            }
        }

        void Renderer::update()
        {
        }

        void Renderer::postUpdate()
        {
        }

        void Renderer::load( SmartPtr<ISharedObject> data )
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

        void Renderer::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    if( graphicsSystem )
                    {
                        auto smgr = graphicsSystem->getGraphicsScene();
                        FB_ASSERT( smgr );

                        auto graphicsNode = getGraphicsNode();
                        if( graphicsNode )
                        {
                            auto graphicsObject = getGraphicsObject();
                            if( graphicsObject )
                            {
                                graphicsNode->detachObject( graphicsObject );
                                smgr->removeGraphicsObject( graphicsObject );
                                setGraphicsObject( nullptr );
                            }

                            graphicsNode->detachAllObjects();
                            smgr->removeSceneNode( graphicsNode );
                            setGraphicsNode( nullptr );
                        }
                        else
                        {
                            setGraphicsObject( nullptr );
                            setGraphicsNode( nullptr );
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

        Array<SmartPtr<ISharedObject>> Renderer::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( 4 );

            auto graphicsObject = getGraphicsObject();
            auto graphicsNode = getGraphicsNode();

            objects.push_back( graphicsObject );
            objects.push_back( graphicsNode );
            objects.push_back( m_sharedMaterial );
            return objects;
        }

        SmartPtr<Properties> Renderer::getProperties() const
        {
            if( auto properties = Component::getProperties() )
            {
                properties->setProperty( "castShadows", static_cast<s32>( m_castShadows ) );

                return properties;
            }

            return nullptr;
        }

        void Renderer::setProperties( SmartPtr<Properties> properties )
        {
            s32 castShadows = 0;
            properties->getPropertyValue( "castShadows", castShadows );
            m_castShadows = static_cast<CastShadows>( castShadows );
        }

        SmartPtr<render::IMaterial> Renderer::getSharedMaterial() const
        {
            return m_sharedMaterial;
        }

        void Renderer::setSharedMaterial( SmartPtr<render::IMaterial> sharedMaterial )
        {
            m_sharedMaterial = sharedMaterial;
        }

        String Renderer::getMaterialName() const
        {
            return m_materialName;
        }

        void Renderer::setMaterialName( const String &materialName )
        {
            m_materialName = materialName;
        }

        IFSM::ReturnType Renderer::handleComponentEvent( u32 state, IFSM::Event eventType )
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
                {
                    if( !getGraphicsObject() )
                    {
                        updateMesh();
                        updateMaterials();
                        updateTransform();
                    }
                }
                break;
                case State::Play:
                {
                    if( !getGraphicsObject() )
                    {
                        updateMesh();
                        updateMaterials();
                        updateTransform();
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

        void Renderer::updateMesh()
        {
        }

        void Renderer::updateMaterials()
        {
        }

        void Renderer::visibilityChanged()
        {
            auto actor = getActor();
            if( actor )
            {
                auto visisble = actor->isVisible();

                if( auto node = getGraphicsNode() )
                {
                    node->setVisible( visisble );
                }
            }
        }

        void Renderer::updateTransform()
        {
            auto actor = getActor();
            if( actor )
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

                    if( auto node = getGraphicsNode() )
                    {
                        node->setPosition( actorPosition );
                        node->setOrientation( actorOrientation );
                        node->setScale( actorScale );
                    }
                }
            }
        }

        SmartPtr<render::IGraphicsObject> Renderer::getGraphicsObject() const
        {
            return m_graphicsObject;
        }

        void Renderer::setGraphicsObject( SmartPtr<render::IGraphicsObject> graphicsObject )
        {
            m_graphicsObject = graphicsObject;
        }

        SmartPtr<render::ISceneNode> Renderer::getGraphicsNode() const
        {
            return m_graphicshNode;
        }

        void Renderer::setGraphicsNode( SmartPtr<render::ISceneNode> graphicshNode )
        {
            m_graphicshNode = graphicshNode;
        }
    }  // namespace scene
}  // end namespace fb
