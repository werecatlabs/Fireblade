#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionMesh.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IMeshShape.h>
#include <FBCore/Interface/Physics/IRigidbody3.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>
#include <FBCore/Interface/Resource/IMeshManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, CollisionMesh, Collision );

        CollisionMesh::CollisionMesh()
        {
        }

        CollisionMesh::~CollisionMesh()
        {
            unload( nullptr );
        }

        void CollisionMesh::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                Collision::load( data );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CollisionMesh::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                m_meshResource = nullptr;
                Collision::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<Properties> CollisionMesh::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();

            static const auto meshPathStr = String( "Mesh Path" );

            auto meshPath = getMeshPath();
            properties->setProperty( meshPathStr, meshPath );

            properties->setProperty( "isConvex", m_isConvex );
            properties->setProperty( "isTrigger", m_isTrigger );

            auto &meshPathProperty = properties->getPropertyObject( meshPathStr );
            meshPathProperty.setType( "file" );

            return properties;
        }

        void CollisionMesh::setProperties( SmartPtr<Properties> properties )
        {
            static const auto meshPathStr = String( "Mesh Path" );

            auto meshPath = getMeshPath();
            properties->getPropertyValue( meshPathStr, meshPath );

            properties->getPropertyValue( "isConvex", m_isConvex );
            properties->getPropertyValue( "isTrigger", m_isTrigger );

            if( meshPath != getMeshPath() )
            {
                meshPath = StringUtil::cleanupPath( meshPath );
                auto ext = Path::getFileExtension( meshPath );

                if( ext == ".mesh" )
                {
                    meshPath = StringUtil::replaceAll( meshPath, ".mesh", ".fbmeshbin" );
                    meshPath = StringUtil::cleanupPath( meshPath );
                    setMeshPath( meshPath );
                }
                else
                {
                    meshPath = StringUtil::cleanupPath( meshPath );
                    setMeshPath( meshPath );
                }
            }
        }

        String CollisionMesh::getMeshPath() const
        {
            return m_meshPath;
        }

        void CollisionMesh::setMeshPath( const String &meshPath )
        {
            auto meshFilePath = StringUtil::cleanupPath( meshPath );

            if( m_meshPath != meshFilePath )
            {
                m_meshPath = meshFilePath;
                setupMesh();
            }
        }

        SmartPtr<IMeshResource> CollisionMesh::getMeshResource() const
        {
            return m_meshResource;
        }

        void CollisionMesh::setupMesh()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto meshPath = getMeshPath();
            if( !StringUtil::isNullOrEmpty( meshPath ) )
            {
                if( auto meshManager = applicationManager->getMeshManager() )
                {
                    auto resource = meshManager->loadFromFile( meshPath );
                    if( resource )
                    {
                        FB_ASSERT( fb::dynamic_pointer_cast<IMeshResource>( resource ) );

                        auto meshResource = fb::static_pointer_cast<IMeshResource>( resource );
                        FB_ASSERT( meshResource );

                        setMeshResource( meshResource );

                        if( auto shape = getShape() )
                        {
                            FB_ASSERT( fb::dynamic_pointer_cast<physics::IMeshShape>( shape ) );

                            auto meshShape = fb::static_pointer_cast<physics::IMeshShape>( shape );
                            if( meshShape )
                            {
                                meshShape->setMeshResource( meshResource );
                            }
                        }
                    }
                }
            }
        }

        IFSM::ReturnType CollisionMesh::handleComponentEvent( u32 state, IFSM::Event eventType )
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
                    try
                    {
                        auto applicationManager = core::IApplicationManager::instance();
                        FB_ASSERT( applicationManager );

                        auto factory = applicationManager->getFactoryManager();
                        FB_ASSERT( factory );

                        auto physicsManager = applicationManager->getPhysicsManager();
                        if( physicsManager )
                        {
                            if( auto actor = getActor() )
                            {
                                if( auto meshComponent = actor->getComponent<Mesh>() )
                                {
                                    auto meshPath = meshComponent->getMeshPath();
                                    setMeshPath( meshPath );
                                }
                            }

                            setupMesh();

                            auto mesh = getMeshResource();
                            if( !mesh->isLoaded() )
                            {
                                mesh->load( nullptr );
                            }

                            auto shape =
                                physicsManager->addCollisionShape<physics::IMeshShape>( mesh );
                            FB_ASSERT( shape );

                            setShape( shape );
                        }
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }
                break;
                default:
                {
                }
                break;
                }
            }
            break;
            case IFSM::Event::Leave:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Edit:
                case State::Play:
                {
                }
                break;
                default:
                {
                }
                break;
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

        void CollisionMesh::setMeshResource( SmartPtr<IMeshResource> meshResource )
        {
            m_meshResource = meshResource;
        }

        bool CollisionMesh::isValid() const
        {
            return Collision::isValid();
        }
    }  // namespace scene
}  // end namespace fb
