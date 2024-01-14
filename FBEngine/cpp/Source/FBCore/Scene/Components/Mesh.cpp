#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Mesh.h>
#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/Interface/Mesh/IMeshResource.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Path.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, Mesh, IComponent );

    const String Mesh::meshPathStr = String( "Mesh Path" );
    const String Mesh::meshStr = String( "Mesh Resource" );

    u32 Mesh::m_idExt = 0;

    Mesh::Mesh() = default;

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

                Component::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Mesh::getMeshPath() const -> String
    {
        return m_meshPath;
    }

    void Mesh::setMeshPath( const String &meshPath )
    {
        m_meshPath = meshPath;
    }

    auto Mesh::getMeshResource() const -> SmartPtr<IMeshResource>
    {
        return m_meshResource;
    }

    void Mesh::setMeshResource( SmartPtr<IMeshResource> meshResource )
    {
        m_meshResource = meshResource;
    }

    auto Mesh::getProperties() const -> SmartPtr<Properties>
    {
        auto properties = Component::getProperties();

        auto meshPath = getMeshPath();
        properties->setProperty( meshPathStr, meshPath );

        auto meshResource = getMeshResource();
        properties->setPropertyAsType( meshStr, meshResource );

        auto &meshPathProperty = properties->getPropertyObject( meshPathStr );
        meshPathProperty.setType( "file" );

        return properties;
    }

    void Mesh::setProperties( SmartPtr<Properties> properties )
    {
        Component::setProperties( properties );

        auto applicationManager = core::ApplicationManager::instance();
        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto meshPath = getMeshPath();
        properties->getPropertyValue( meshPathStr, meshPath );

        auto meshResource = getMeshResource();
        properties->getPropertyAsType( meshStr, meshResource );

        //if( meshResource != getMeshResource() )
        //{
        //    setMeshResource( meshResource );

        //    auto meshPath = meshResource->getFilePath();
        //    setMeshPath( meshPath );
        //}
        //else
        
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

    auto Mesh::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
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
}  // namespace fb::scene
