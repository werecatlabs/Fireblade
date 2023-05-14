#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionPlane.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, CollisionPlane, Collision );

        CollisionPlane::CollisionPlane()
        {
        }

        CollisionPlane::~CollisionPlane()
        {
            unload( nullptr );
        }

        void CollisionPlane::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                Collision::load( data );

                auto applicationManager = core::IApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();
                m_material = physicsManager->createMaterial();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CollisionPlane::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    Collision::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // namespace scene
}  // end namespace fb
