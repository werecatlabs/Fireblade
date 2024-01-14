#ifndef Constraint_h__
#define Constraint_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Atomics/AtomicFloat.h>
#include <FBCore/Interface/Physics/FBPhysicsTypes.h>
#include <FBCore/System/Job.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace scene
    {

        /** A component to constrain two rigid bodies. */
        class Constraint : public Component
        {
        public:
            /** The constraint type. */
            enum class Type
            {
                D6,
                Fixed,

                Count
            };

            /** Constructor. */
            Constraint() = default;

            /** Destructor. */
            ~Constraint() override;

            /** @copydoc IComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** Gets the first body.
            @return The first body.
            */
            SmartPtr<Rigidbody> getBodyA() const;

            /** Sets the first body.
            @param body The first body.
            */
            void setBodyA( SmartPtr<Rigidbody> body );

            /** Gets the second body.
            @return The second body.
            */
            SmartPtr<Rigidbody> getBodyB() const;

            /** Sets the second body.
            @param body The second body.
            */
            void setBodyB( SmartPtr<Rigidbody> body );

            /** Gets the type of the constraint.
            @return The type of the constraint.
            */
            Type getType() const;

            /** Sets the type of the constraint.
            @param type The type of the constraint.
            */
            void setType( Type type );

            /** Gets the constraint. */
            SmartPtr<physics::IPhysicsConstraint3> getConstraint() const;

            /** Sets the constraint. */
            void setConstraint( SmartPtr<physics::IPhysicsConstraint3> constraint );

            void updateConstraint();

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Sets up the constraint. */
            void setupConstraint();

            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            // The constraint.
            SmartPtr<physics::IPhysicsConstraint3> m_constraint;

            // The first body.
            SmartPtr<Rigidbody> m_body0;

            // The second body.
            SmartPtr<Rigidbody> m_body1;

            // The type of the constraint.
            Type m_type = Type::D6;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // Constraint_h__
