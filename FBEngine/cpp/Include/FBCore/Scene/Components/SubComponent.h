#ifndef __SubComponent_h__
#define __SubComponent_h__

#include <FBCore/Interface/Scene/ISubComponent.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{
    namespace scene
    {
        /** Base class for a component object. */
        class SubComponent : public Resource<ISubComponent>
        {
        public:
            /** Default constructor. */
            SubComponent();

            /** Virtual destructor. */
            ~SubComponent() override;

            /**
             * Get the parent component of this component.
             *
             * @return A shared pointer to the parent component.
             */
            SmartPtr<IComponent> getParent() const override;

            /**
             * Set the parent component of this component.
             *
             * @param parent A shared pointer to the parent component.
             */
            void setParent( SmartPtr<IComponent> parent ) override;

            SmartPtr<ISharedObject> toData() const override;

            void fromData(SmartPtr<ISharedObject> data) override;

            /**
             * Gets the data associated with the component as a properties object.
             * @return The data associated with the component as a properties object.
             */
            SmartPtr<Properties> getProperties() const override;

            /**
             * Sets the data associated with the component as a properties object.
             * @param properties The properties object to associate with this component.
             */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<IComponent> m_parent;

            /**
             * The ID extension of the component.
             */
            static u32 m_idExt;
        };

    }  // namespace scene
}  // namespace fb

#endif  // SubComponent_h__
