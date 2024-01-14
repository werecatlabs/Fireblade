#ifndef FBCPhysicsBody3_h__
#define FBCPhysicsBody3_h__

#include <FBCore/Interface/Physics/IPhysicsBody3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/State/States/PhysicsBodyState.h>
#include <FBCore/Physics/Physics3SharedObject.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace physics
    {

        template <class T>
        class PhysicsBody3 : public Physics3SharedObject<T>
        {
        public:
            PhysicsBody3();
            ~PhysicsBody3() override;

            SmartPtr<IPhysicsScene3> getScene() const override;

            void setScene( SmartPtr<IPhysicsScene3> scene ) override;

            void setTransform( const Transform3<real_Num> &transform ) override;

            Transform3<real_Num> getTransform() const override;

            void setActorFlag( ActorFlag::Enum flag, bool value ) override;

            ActorFlag::Enum getActorFlags() const override;

            real_Num getMass() const override;

            void setMass( real_Num mass ) override;

            void setCollisionType( u32 type ) override;

            u32 getCollisionType() const override;

            void setCollisionMask( u32 mask ) override;

            u32 getCollisionMask() const override;

            void setEnabled( bool enabled ) override;

            bool isEnabled() const override;

            void *getUserDataById( u32 id ) const override;

            void setUserDataById( u32 id, void *userData ) override;

            bool getKinematicMode() const override;

            void setKinematicMode( bool kinematicMode ) override;

            SmartPtr<IPhysicsBody3> clone() override;

            void wakeUp()
            {
            }

            SmartPtr<Properties> getProperties() const override;

            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_TEMPLATE_DECL( PhysicsBody3, T );

        protected:
            SmartPtr<PhysicsBodyState> m_state;
        };

        template <class T>
        PhysicsBody3<T>::PhysicsBody3()
        {
        }

        template <class T>
        PhysicsBody3<T>::~PhysicsBody3()
        {
        }

        template <class T>
        SmartPtr<IPhysicsScene3> PhysicsBody3<T>::getScene() const
        {
            return nullptr;
        }

        template <class T>
        void PhysicsBody3<T>::setScene( SmartPtr<IPhysicsScene3> scene )
        {
        }

        template <class T>
        void PhysicsBody3<T>::setTransform( const Transform3<real_Num> &transform )
        {
        }

        template <class T>
        Transform3<real_Num> PhysicsBody3<T>::getTransform() const
        {
            return Transform3<real_Num>();
        }

        template <class T>
        void PhysicsBody3<T>::setActorFlag( ActorFlag::Enum flag, bool value )
        {
        }

        template <class T>
        ActorFlag::Enum PhysicsBody3<T>::getActorFlags() const
        {
            return static_cast<ActorFlag::Enum>( 0 );
        }

        template <class T>
        real_Num PhysicsBody3<T>::getMass() const
        {
            return static_cast<real_Num>( 0.0 );
        }

        template <class T>
        void PhysicsBody3<T>::setMass( real_Num mass )
        {
        }

        template <class T>
        void PhysicsBody3<T>::setCollisionType( u32 type )
        {
        }

        template <class T>
        u32 PhysicsBody3<T>::getCollisionType() const
        {
            return 0;
        }

        template <class T>
        void PhysicsBody3<T>::setCollisionMask( u32 mask )
        {
        }

        template <class T>
        u32 PhysicsBody3<T>::getCollisionMask() const
        {
            return 0;
        }

        template <class T>
        void PhysicsBody3<T>::setEnabled( bool enabled )
        {
        }

        template <class T>
        bool PhysicsBody3<T>::isEnabled() const
        {
            return false;
        }

        template <class T>
        void *PhysicsBody3<T>::getUserDataById( u32 id ) const
        {
            return nullptr;
        }

        template <class T>
        void PhysicsBody3<T>::setUserDataById( u32 id, void *userData )
        {
        }

        template <class T>
        bool PhysicsBody3<T>::getKinematicMode() const
        {
            return false;
        }

        template <class T>
        void PhysicsBody3<T>::setKinematicMode( bool kinematicMode )
        {
        }

        template <class T>
        SmartPtr<IPhysicsBody3> PhysicsBody3<T>::clone()
        {
            return nullptr;
        }

        template <class T>
        SmartPtr<Properties> PhysicsBody3<T>::getProperties() const
        {
            return nullptr;
        }

        template <class T>
        void PhysicsBody3<T>::setProperties( SmartPtr<Properties> properties )
        {
        }

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, PhysicsBody3, T, T );
    }  // namespace physics
}  // namespace fb

#endif  // FBCPhysicsBody3_h__
