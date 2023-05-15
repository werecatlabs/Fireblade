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

            Vector3<real_Num> getExtents() const;
            virtual void setExtents( const Vector3<real_Num> &extents );

            Vector3<real_Num> getPosition() const;
            void setPosition( const Vector3<real_Num> &position );

            f32 getRadius() const;
            void setRadius( f32 radius );

            SmartPtr<Rigidbody> getRigidBody() const;
            void setRigidBody( SmartPtr<Rigidbody> rigidBody );

            AABB3F getBoundingBox() const;

            SmartPtr<physics::IPhysicsMaterial3> getMaterial() const;
            void setMaterial( SmartPtr<physics::IPhysicsMaterial3> material );

            SmartPtr<physics::IPhysicsShape3> getShape() const;
            void setShape( SmartPtr<physics::IPhysicsShape3> shape );

            bool isTrigger() const;
            void setTrigger( bool trigger );

            bool isValid() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            ///
            Vector3<real_Num> m_extents = Vector3<real_Num>::unit();

            ///
            Vector3<real_Num> m_position = Vector3<real_Num>::zero();

            ///
            f32 m_radius = 1.0f;

            ///
            SmartPtr<Rigidbody> m_rigidBody;

            ///
            SmartPtr<physics::IPhysicsMaterial3> m_material;

            SmartPtr<physics::IPhysicsShape3> m_shape;

            bool m_isTrigger = false;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // Collision_h__
