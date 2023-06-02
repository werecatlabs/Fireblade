#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionTerrain.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/ITerrainShape.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, CollisionTerrain, Collision );

        CollisionTerrain::CollisionTerrain()
        {
        }

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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                auto shape = physicsManager->createCollisionShape<physics::ITerrainShape>(nullptr);
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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                Collision::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        IFSM::ReturnType CollisionTerrain::handleComponentEvent( u32 state, IFSM::Event eventType )
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

    }  // namespace scene
}  // end namespace fb
