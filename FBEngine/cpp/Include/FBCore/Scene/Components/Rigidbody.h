#ifndef __Rigidbody_h__
#define __Rigidbody_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    namespace scene
    {
        /**
         * @class Rigidbody
         * @brief Represents a rigid body component in the physics simulation, handling physical interactions.
         * @extends BaseComponent
         */
        class Rigidbody : public Component
        {
        public:
            /** @brief Constructor */
            Rigidbody();

            /** @brief Destructor */
            ~Rigidbody() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @brief Invoked when a component is loaded */
            void componentLoaded( SmartPtr<IComponent> component );

            /** @brief Invoked when the component is awake */
            void awake() override;

            /** @brief Initiates the playback of the Rigidbody simulation */
            void play();

            /** @brief Updates all Rigidbody components */
            static void updateComponents();

            /** @brief Invoked before the update loop */
            void preUpdate() override;

            /** @brief Invoked during the update loop */
            void update() override;

            /** @brief Invoked after the update loop */
            void postUpdate() override;

            /** @brief Updates the kinematic state of the Rigidbody */
            void updateKinematicState( bool val );

            /** @brief Returns the RigidDynamic3 object */
            SmartPtr<physics::IRigidDynamic3> getRigidDynamic() const;

            /** @brief Sets the RigidDynamic3 object */
            void setRigidDynamic( SmartPtr<physics::IRigidDynamic3> val );

            /** @brief Returns the RigidStatic3 object */
            SmartPtr<physics::IRigidStatic3> getRigidStatic() const;

            /** @brief Sets the RigidStatic3 object */
            void setRigidStatic( SmartPtr<physics::IRigidStatic3> val );

            /** @brief Returns true if the Rigidbody is static, false otherwise */
            bool isStatic() const;

            /** @brief Sets the static property of the Rigidbody */
            void setStatic( bool val );

            /** @brief Returns true if the Rigidbody is kinematic, false otherwise */
            bool isKinematic() const;

            /** @brief Sets the kinematic property of the Rigidbody */
            void setKinematic( bool val );

            /** @brief Returns the group mask of the Rigidbody */
            u32 getGroupMask() const;

            /** @brief Sets the group mask of the Rigidbody */
            void setGroupMask( u32 val );

            /** @brief Returns the collision mask of the Rigidbody */
            u32 getCollisionMask() const;

            /** @brief Sets the collision mask of the Rigidbody */
            void setCollisionMask( u32 val );

            /** @brief Returns the local bounds of the Rigidbody */
            AABB3<real_Num> getLocalBounds() const;

            /** @brief Sets the local bounds of the Rigidbody */
            void setLocalBounds( const AABB3<real_Num> &val );

            /** @brief Returns the IPhysicsScene3 object */
            SmartPtr<physics::IPhysicsScene3> getScene() const;

            /** @brief Sets the IPhysicsScene3 object */
            void setScene( SmartPtr<physics::IPhysicsScene3> scene );

            /** @brief Returns the IPhysicsMaterial3 object */
            SmartPtr<physics::IPhysicsMaterial3> getMaterial() const;

            /** @brief Sets the IPhysicsMaterial3 object */
            void setMaterial( SmartPtr<physics::IPhysicsMaterial3> material );

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @brief Updates the transform of the Rigidbody in the physics simulation */
            void updateTransform() override;

            /** @brief Returns the linear velocity of the Rigidbody */
            Vector3<real_Num> getLinearVelocity() const;

            /** @brief Sets the linear velocity of the Rigidbody */
            void setLinearVelocity( const Vector3<real_Num> &linearVelocity );

            /** @brief Returns the angular velocity of the Rigidbody */
            Vector3<real_Num> getAngularVelocity() const;

            /** @brief Sets the angular velocity of the Rigidbody */
            void setAngularVelocity( const Vector3<real_Num> &angularVelocity );

            /** @brief Returns the local angular velocity of the Rigidbody */
            Vector3<real_Num> getLocalAngularVelocity() const;

            /** @brief Returns the local linear velocity of the Rigidbody */
            Vector3<real_Num> getLocalLinearVelocity() const;

            /** @brief Adds a force to the Rigidbody */
            void addForce( const Vector3<real_Num> &force );

            /** @brief Adds a torque to the Rigidbody */
            void addTorque( const Vector3<real_Num> &torque );

            /** @brief Sets the mass of the Rigidbody */
            void setMass( real_Num mass );

            /** @brief Returns the mass of the Rigidbody */
            real_Num getMass() const;

            /** @brief Sets the moment of inertia (MOI) of the Rigidbody */
            void setMOI( const Vector3<real_Num> &moi );

            /** @brief Returns the moment of inertia (MOI) of the Rigidbody */
            Vector3<real_Num> getMOI() const;

            /** @brief Returns the velocity at a specific point on the Rigidbody */
            Vector3<real_Num> getPointVelocity( const Vector3<real_Num> &point );

            /** @brief Returns the local velocity at a specific point on the Rigidbody */
            Vector3<real_Num> getLocalPointVelocity( const Vector3<real_Num> &point );

            /** @brief Returns the mass space inertia tensor of the Rigidbody */
            Vector3<real_Num> getMassSpaceInertiaTensor() const;

            /** @brief Sets the mass space inertia tensor of the Rigidbody */
            void setMassSpaceInertiaTensor( const Vector3<real_Num> &massSpaceInertiaTensor );

            FB_CLASS_REGISTER_DECL;

        protected:
            struct rigidbody_data
            {
                bool dirty = false;
                Rigidbody *owner = nullptr;
            };

            void createRigidbodyObject();
            void destroyRigidbodyObject();

            void attachShape();

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @brief Handles events related to the component's state machine */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /** @brief Pointer to the RigidDynamic3 object of the Rigidbody */
            SmartPtr<physics::IRigidDynamic3> m_rigidDynamic;

            /** @brief Pointer to the RigidStatic3 object of the Rigidbody */
            SmartPtr<physics::IRigidStatic3> m_rigidStatic;

            /** @brief Pointer to the cloned physics body */
            SmartPtr<physics::IPhysicsBody3> m_clonedActor;

            /** @brief Pointer to the IPhysicsScene3 object related to the Rigidbody */
            SmartPtr<physics::IPhysicsScene3> m_scene;

            /** @brief Pointer to the IPhysicsMaterial3 object related to the Rigidbody */
            SmartPtr<physics::IPhysicsMaterial3> m_material;

            /** @brief The mass space inertia tensor of the Rigidbody */
            Vector3<real_Num> m_massSpaceInertiaTensor = Vector3<real_Num>( 1, 1, 1 );

            /** @brief The mass of the Rigidbody */
            real_Num m_mass = static_cast<real_Num>( 1000.0 );

            /** @brief The group mask of the Rigidbody */
            atomic_u32 m_groupMask = 0;

            /** @brief The collision mask of the Rigidbody */
            atomic_u32 m_collisionMask = 0;

            /** @brief Indicates whether the Rigidbody is static or not */
            atomic_bool m_isStatic = false;

            /** @brief Indicates whether the Rigidbody is kinematic or not */
            atomic_bool m_isKinematic = false;

            /** @brief Indicates whether the Rigidbody's transform is being updated */
            atomic_bool m_isUpdatingTransform = false;

            /** @brief The bounds of the Rigidbody */
            AABB3<real_Num> m_bounds;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // Rigidbody_h__
