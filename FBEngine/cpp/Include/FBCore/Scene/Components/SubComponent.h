#ifndef __BaseSubComponent_h__
#define __BaseSubComponent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/FSM/FSMListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Resource/CResource.h>

namespace fb
{
    namespace scene
    {
        /** Base class for a component object. */
        class SubComponent : public CResource<ISubComponent>
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
}  // end namespace fb

#endif  // BaseComponent_h__
