#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Path.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Mesh, IComponent );

        u32 Mesh::m_idExt = 0;

        Mesh::Mesh()
        {
        }

        Mesh::~Mesh()
        {
            unload( nullptr );
        }

        void Mesh::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loading );
            Component::load( data );
            setLoadingState( LoadingState::Loaded );
        }

        void Mesh::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    m_mesh = nullptr;
                    Component::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        String Mesh::getMeshPath() const
        {
            return m_meshPath;
        }

        void Mesh::setMeshPath( const String &meshPath )
        {
            m_meshPath = meshPath;
        }

        SmartPtr<IMesh> Mesh::getMesh() const
        {
            return m_mesh;
        }

        void Mesh::setMesh( SmartPtr<IMesh> mesh )
        {
            m_mesh = mesh;

            static const auto ext = String( ".fbmeshbin" );
            auto meshPath = m_mesh->getName() + ext;
            setMeshPath( meshPath );
        }

        SmartPtr<IMeshResource> Mesh::getMeshResource() const
        {
            return m_meshResource;
        }

        void Mesh::setMeshResource( SmartPtr<IMeshResource> meshResource )
        {
            m_meshResource = meshResource;
        }

        SmartPtr<Properties> Mesh::getProperties() const
        {
            auto properties = Component::getProperties();

            static const auto meshPathStr = String( "Mesh Path" );
            static const auto meshStr = String( "Mesh Resource" );

            auto meshPath = getMeshPath();
            properties->setProperty( meshPathStr, meshPath );

            auto meshResource = getMeshResource();
            properties->setProperty( meshStr, meshResource );

            auto &meshPathProperty = properties->getPropertyObject( meshPathStr );
            meshPathProperty.setType( "file" );

            return properties;
        }

        void Mesh::setProperties( SmartPtr<Properties> properties )
        {
            Component::setProperties( properties );

            auto applicationManager = core::IApplicationManager::instance();
            auto resourceDatabase = applicationManager->getResourceDatabase();

            static const auto meshPathStr = String( "Mesh Path" );
            static const auto meshStr = String( "Mesh Resource" );

            auto meshPath = getMeshPath();
            properties->getPropertyValue( meshPathStr, meshPath );

            auto meshResource = getMeshResource();
            properties->getPropertyValue( meshStr, meshResource );

            if( meshResource != getMeshResource() )
            {
                setMeshResource( meshResource );
            }

            if( meshPath != getMeshPath() )
            {
                meshPath = StringUtil::cleanupPath( meshPath );
                setMeshPath( meshPath );

                if( !StringUtil::isNullOrEmpty( meshPath ) )
                {
                    auto meshResourceResult =
                        resourceDatabase->createOrRetrieveByType<IMeshResource>( meshPath );
                    setMeshResource( meshResourceResult.first );
                }
            }
        }

        IFSM::ReturnType Mesh::handleComponentEvent( u32 state, IFSM::Event eventType )
        {
            Component::handleComponentEvent( state, eventType );

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
    }  // namespace scene
}  // end namespace fb
