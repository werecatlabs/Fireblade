#ifndef Collision_h__
#define Collision_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    namespace scene
    {

        /** Base class for a collision component. */
        class Collision : public Component
        {
        public:
            /** Default constructor. */
            Collision() = default;

            /** Default destructor. */
            ~Collision() override = default;

            /** @copydoc IComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** Get the extents of the collision. */
            Vector3<real_Num> getExtents() const;

            /** Set the extents of the collision. */
            virtual void setExtents( const Vector3<real_Num> &extents );

            /** Get the position of the collision. */
            Vector3<real_Num> getPosition() const;

            /** Set the position of the collision. */
            void setPosition( const Vector3<real_Num> &position );

            /** Get the radius of the collision. */
            f32 getRadius() const;

            /** Set the radius of the collision. */
            void setRadius( f32 radius );

            /** Gets the rigid body. */
            SmartPtr<Rigidbody> getRigidBody() const;

            /** Sets the rigid body. */
            void setRigidBody( SmartPtr<Rigidbody> rigidBody );

            /** Get the bounding box of the collision. */
            AABB3F getBoundingBox() const;

            /** Get physics material. */
            SmartPtr<physics::IPhysicsMaterial3> getMaterial() const;

            /** Set physics material. */
            void setMaterial( SmartPtr<physics::IPhysicsMaterial3> material );

            /** Get physics shape. */
            SmartPtr<physics::IPhysicsShape3> getShape() const;

            /** Set physics shape. */
            void setShape( SmartPtr<physics::IPhysicsShape3> shape );

            /** Get trigger flag. */
            bool isTrigger() const;

            /** Set trigger flag. */
            void setTrigger( bool trigger );

            /** @copydoc Component::isValid */
            bool isValid() const;

            /** @copydoc Component::updateTransform */
            void updateTransform() override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Creates the physics shape. */
            virtual void createPhysicsShape();

            /** Updates the rigid body. */
            virtual void updateRigidBody();

            // The extents of the collision.
            Vector3<real_Num> m_extents = Vector3<real_Num>::unit();

            // The position of the collision.
            Vector3<real_Num> m_position = Vector3<real_Num>::zero();

            // The radius of the collision.
            f32 m_radius = 1.0f;

            // The rigid body.
            SmartPtr<Rigidbody> m_rigidBody;

            // The physics material.
            SmartPtr<physics::IPhysicsMaterial3> m_material;

            // The physics shape.
            SmartPtr<physics::IPhysicsShape3> m_shape;

            // The trigger flag.
            bool m_isTrigger = false;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // Collision_h__
