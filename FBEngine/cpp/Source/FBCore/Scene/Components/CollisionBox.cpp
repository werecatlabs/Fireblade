#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionBox.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsMaterial3.h>
#include <FBCore/Interface/Physics/IBoxShape3.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Math/MathUtil.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, CollisionBox, Collision );

    CollisionBox::CollisionBox() = default;

    CollisionBox::~CollisionBox()
    {
        unload( nullptr );
    }

    void CollisionBox::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            Collision::load( data );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factory = applicationManager->getFactoryManager();
            FB_ASSERT( factory );

            if( auto physicsManager = applicationManager->getPhysicsManager() )
            {
                auto shape = physicsManager->addCollisionShape<physics::IBoxShape3>( nullptr );
                FB_ASSERT( shape );

                setShape( shape );

                auto extents = getExtents();
                FB_ASSERT( MathUtil<real_Num>::isFinite( extents ) );

                shape->setExtents( extents );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CollisionBox::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                if( physicsManager )
                {
                    auto shape = getShape();
                    physicsManager->removeCollisionShape( shape );
                    setShape( nullptr );
                    shape = nullptr;
                }

                Collision::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CollisionBox::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            unload( nullptr );
            load( nullptr );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CollisionBox::getProperties() const -> SmartPtr<Properties>
    {
        return Collision::getProperties();
    }

    void CollisionBox::setProperties( SmartPtr<Properties> properties )
    {
        Collision::setProperties( properties );
    }

    void CollisionBox::setExtents( const Vector3<real_Num> &extents )
    {
        Collision::setExtents( extents );
        updateTransform();
    }

    auto CollisionBox::isValid() const -> bool
    {
        if( auto actor = getActor() )
        {
            switch( actor->getState() )
            {
            case IActor::State::Edit:
            case IActor::State::Play:
            {
                if( auto valid = Collision::isValid() )
                {
                    if( auto shape = getShape() )
                    {
                        if( auto boxShape = fb::dynamic_pointer_cast<physics::IBoxShape3>( shape ) )
                        {
                            valid = true;
                        }
                    }

                    return valid;
                }

                return false;
            }
            break;
            default:
            {
                return true;
            }
            };
        }

        return false;
    }

    void CollisionBox::updateTransform()
    {
        Collision::updateTransform();

        auto extents = getExtents();

        if( auto shape = getShape() )
        {
            if( auto boxShape = fb::static_pointer_cast<physics::IBoxShape3>( shape ) )
            {
                boxShape->setExtents( extents );
            }
        }
    }

}  // namespace fb::scene
