#ifndef FBPhysxRigidBody3_h__
#define FBPhysxRigidBody3_h__

#include <FBPhysx/FBPhysxBody3.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/State/Messages/StateMessageObject.h>
#include <FBCore/Core/LogManager.h>
#include <PxRigidBody.h>

namespace fb
{
    namespace physics
    {

        template <class T>
        class PhysxRigidBody3 : public PhysxBody3<T>
        {
        public:
            PhysxRigidBody3()
            {
            }

            ~PhysxRigidBody3() override
            {
                unload( nullptr );
            }

            void unload( SmartPtr<ISharedObject> data )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                if( auto stateObject = PhysxRigidBody3<T>::getStateObject() )
                {
                    if( auto stateListener = PhysxRigidBody3<T>::getStateListener() )
                    {
                        stateObject->removeStateListener( stateListener );
                    }

                    stateManager->removeStateObject( stateObject );

                    stateObject->unload( nullptr );
                    PhysxRigidBody3<T>::setStateObject( nullptr );
                }

                if( auto stateListener = PhysxRigidBody3<T>::getStateListener() )
                {
                    stateListener->unload( nullptr );
                    PhysxRigidBody3<T>::setStateListener( nullptr );
                }
            }

            void setRigidBodyFlag( RigidBodyFlag::Enum flag, bool value ) override
            {
            }

            RigidBodyFlag::Enum getRigidBodyFlags() const override
            {
                return (RigidBodyFlag::Enum)0;
            }

            void addShape( SmartPtr<IPhysicsShape3> shape ) override
            {
                try
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto physicsManager = applicationManager->getPhysicsManager();
                    FB_ASSERT( physicsManager );

                    ISharedObject::ScopedLock lock( physicsManager );

                    auto factoryManager = applicationManager->getFactoryManager();
                    FB_ASSERT( factoryManager );

                    auto task = Thread::getCurrentTask();
                    auto physicsTask = physicsManager->getPhysicsTask();

                    const auto &loadingState = PhysxBody3<T>::getLoadingState();

                    //if( loadingState == LoadingState::Loaded && task == physicsTask )
                    {
                        if( !PhysxRigidBody3<T>::isLoaded() )
                        {
                            PhysxRigidBody3<T>::load( nullptr );
                        }

                        if( shape )
                        {
                            auto pThis = PhysxRigidBody3<T>::template getSharedFromThis<T>();
                            shape->setActor( pThis );

                            if( !shape->isLoaded() )
                            {
                                shape->load( nullptr );
                            }

                            physx::PxShape *pShape = nullptr;
                            shape->_getObject( (void **)&pShape );

                            //FB_ASSERT( pShape );

                            if( pShape )
                            {
                                if( auto pxActor = PhysxRigidBody3<T>::getActor() )
                                {
                                    pxActor->attachShape( *pShape );
                                }
                            }

                            m_shapes.push_back( shape );
                        }
                    }
                    //else
                    //{
                    //    auto message = factoryManager->make_ptr<StateMessageObject>();
                    //    message->setType( IPhysicsBody3::STATE_MESSAGE_ATTACH_SHAPE );
                    //    message->setObject( shape );

                    //    if( auto stateObject = getStateObject() )
                    //    {
                    //        stateObject->addMessage( physicsTask, message );
                    //    }
                    //}
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            void removeShape( SmartPtr<IPhysicsShape3> shape, bool wakeOnLostTouch = true ) override
            {
                try
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto physicsManager = applicationManager->getPhysicsManager();
                    FB_ASSERT( physicsManager );

                    auto factoryManager = applicationManager->getFactoryManager();
                    FB_ASSERT( factoryManager );

                    auto task = Thread::getCurrentTask();
                    auto physicsTask = physicsManager->getPhysicsTask();

                    const auto &loadingState = PhysxBody3<T>::getLoadingState();

                    if( loadingState == LoadingState::Loaded && task == physicsTask )
                    {
                        if( shape )
                        {
                            physx::PxShape *pShape = nullptr;
                            shape->_getObject( (void **)&pShape );

                            if( pShape )
                            {
                                if( auto pxActor = PhysxRigidBody3<T>::getActor() )
                                {
                                    pxActor->detachShape( *pShape );
                                }
                            }

                            shape->unload( nullptr );
                            shape->setActor( nullptr );

                            auto it = std::find( m_shapes.begin(), m_shapes.end(), shape );
                            if( it != m_shapes.end() )
                            {
                                m_shapes.erase( it );
                            }
                        }
                    }
                    else
                    {
                        auto message = factoryManager->make_ptr<StateMessageObject>();
                        message->setType( IPhysicsBody3::STATE_MESSAGE_DETACH_SHAPE );
                        message->setObject( shape );

                        if( auto stateObject = PhysxRigidBody3<T>::getStateObject() )
                        {
                            stateObject->addMessage( physicsTask, message );
                        }
                    }
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }

            Array<SmartPtr<IPhysicsShape3>> getShapes() const override
            {
                return Array<SmartPtr<IPhysicsShape3>>();
            }

            u32 getNumShapes() const override
            {
                return 0;
            }

            void setLinearVelocity( const Vector3<real_Num> &linVel, bool autowake = true ) override
            {
            }

            Vector3<real_Num> getLinearVelocity() const override
            {
                return Vector3<real_Num>::zero();
            }

            void setAngularVelocity( const Vector3<real_Num> &angVel, bool autowake = true ) override
            {
            }

            Vector3<real_Num> getAngularVelocity() const override
            {
                return Vector3<real_Num>::zero();
            }

            void addForce( const Vector3<real_Num> &force ) override
            {
            }

            void clearForce( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override
            {
            }

            void addTorque( const Vector3<real_Num> &torque ) override
            {
            }

            void clearTorque( PhysicsTypes::ForceMode mode = PhysicsTypes::ForceMode::Force ) override
            {
            }

            AABB3<real_Num> getLocalAABB() const override
            {
                return AABB3<real_Num>();
            }

            AABB3<real_Num> getWorldAABB() const override
            {
                return AABB3<real_Num>();
            }

            void setCMassLocalPose( const Transform3<real_Num> &pose ) override
            {
            }

            Transform3<real_Num> getCMassLocalPose() const override
            {
                return Transform3<real_Num>();
            }

            void setMassSpaceInertiaTensor( const Vector3<real_Num> &m ) override
            {
            }

            Vector3<real_Num> getMassSpaceInertiaTensor() const override
            {
                return Vector3<real_Num>::zero();
            }

            Vector3<real_Num> getMassSpaceInvInertiaTensor() const override
            {
                return Vector3<real_Num>::zero();
            }

            void _getObject( void** object ) const
            {
                auto p = PhysxRigidBody3<T>::getActor();
                *object = p;
            }

            physx::PxRigidActor *getActor() const;

            void setActor( physx::PxRigidActor *actor );

            FB_CLASS_REGISTER_TEMPLATE_DECL( PhysxRigidBody3, T );

        protected:
            AtomicRawPtr<physx::PxRigidActor> m_actor;

            Array<SmartPtr<IPhysicsShape3>> m_shapes;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, PhysxRigidBody3, T, T );

        template <class T>
        physx::PxRigidActor *PhysxRigidBody3<T>::getActor() const
        {
            auto p = m_actor.load();
            return p;
        }

        template <class T>
        void PhysxRigidBody3<T>::setActor( physx::PxRigidActor *actor )
        {
            m_actor = actor;
        }

    }  // namespace physics
}  // namespace fb

#endif  // FBPhysxRigidBody3_h__
