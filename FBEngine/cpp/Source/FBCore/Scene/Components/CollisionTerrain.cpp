#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionTerrain.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/ITerrainShape.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, CollisionTerrain, Collision );

    CollisionTerrain::CollisionTerrain() = default;

    CollisionTerrain::~CollisionTerrain()
    {
        unload( nullptr );
    }

    void CollisionTerrain::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Collision::load( data );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            auto shape = physicsManager->addCollisionShape<physics::ITerrainShape>( nullptr );
            FB_ASSERT( shape );

            setShape( shape );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CollisionTerrain::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            Collision::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CollisionTerrain::handleComponentEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        switch( eventType )
        {
        case IFSM::Event::Change:
        {
        }
        break;
        case IFSM::Event::Enter:
        {
        }
        break;
        default:
        {
        }
        break;
        };

        return IFSM::ReturnType::Ok;
    }

}  // namespace fb::scene
