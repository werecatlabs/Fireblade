#ifndef FBPhysxBody3_h__
#define FBPhysxBody3_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBPhysx/FBPhysxSharedObject.h>
#include <FBCore/Interface/Physics/IPhysicsBody3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/State/States/PhysicsBodyState.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace physics
    {
        template <class T>
        class PhysxBody3 : public PhysxSharedObject<T>
        {
        public:
            PhysxBody3()
            {
            }

            ~PhysxBody3() override
            {
            }

            SmartPtr<IPhysicsScene3> getScene() const override;

            void setScene( SmartPtr<IPhysicsScene3> scene ) override;

            void setTransform( const Transform3<real_Num> &transform ) override
            {
            }

            Transform3<real_Num> getTransform() const override
            {
                return Transform3<real_Num>();
            }

            void setActorFlag( PxActorFlag::Enum flag, bool value ) override
            {
            }

            PxActorFlag::Enum getActorFlags() const override
            {
                return (PxActorFlag::Enum)0;
            }

            real_Num getMass() const override
            {
                return real_Num( 0.0 );
            }

            void setMass( real_Num mass ) override
            {
            }

            void setCollisionType( u32 type ) override
            {
            }

            u32 getCollisionType() const override
            {
                return 0;
            }

            void setCollisionMask( u32 mask ) override
            {
            }

            u32 getCollisionMask() const override
            {
                return 0;
            }

            void setEnabled( bool enabled ) override
            {
            }

            bool isEnabled() const override
            {
                return false;
            }

            void *getUserDataById( u32 id ) const override
            {
                return nullptr;
            }

            void setUserDataById( u32 id, void *userData ) override
            {
            }

            bool getKinematicMode() const override
            {
                return false;
            }

            void setKinematicMode( bool kinematicMode ) override
            {
            }

            SmartPtr<IPhysicsBody3> clone() override
            {
                return nullptr;
            }

            SmartPtr<Properties> getProperties() const override;

            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_TEMPLATE_DECL( PhysxBody3, T );

        protected:
            AtomicSmartPtr<IPhysicsScene3> m_scene;

            SmartPtr<PhysicsBodyState> m_state;

            mutable RecursiveMutex m_mutex;
        };

        template <class T>
        void PhysxBody3<T>::setScene( SmartPtr<IPhysicsScene3> scene )
        {
            m_scene = scene;
        }

        template <class T>
        SmartPtr<IPhysicsScene3> PhysxBody3<T>::getScene() const
        {
            return m_scene;
        }

        template <class T>
        SmartPtr<Properties> PhysxBody3<T>::getProperties() const
        {
            return nullptr;
        }

        template <class T>
        void PhysxBody3<T>::setProperties( SmartPtr<Properties> properties )
        {
        }

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, PhysxBody3, T, CSharedObject<T> );
    }  // namespace physics
}  // namespace fb

#endif  // FBPhysxBody3_h__
