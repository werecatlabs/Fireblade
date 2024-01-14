#ifndef ISubComponent_h__
#define ISubComponent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace scene
    {

        /**
         * @file ISubComponent.h
         * @brief Interface for a sub component.
         *
         * This class defines an interface for a sub component of a larger component in an ECS system.
         * Sub components can be thought of as optional pieces of functionality that can be attached
         * to a component, and may be implemented as separate classes derived from this interface.
         * Sub components should provide their own implementation for the methods defined here.
         *
         * @author Zane Desir
         * @version 1.0
         */
        class ISubComponent : public IResource
        {
        public:
            /** Virtual destructor. */
            ~ISubComponent() override = default;

            /**
             * Get the parent component of this component.
             *
             * @return A shared pointer to the parent component.
             */
            virtual SmartPtr<IComponent> getParentComponent() const = 0;

            /**
             * Set the parent component of this component.
             *
             * @param parentComponent A shared pointer to the parent component.
             */
            virtual void setParentComponent( SmartPtr<IComponent> parentComponent ) = 0;

            virtual SmartPtr<ISubComponent> getParent() const = 0;

            virtual void setParent( SmartPtr<ISubComponent> parent ) = 0;

            virtual void addChildByType( hash_type componentType ) = 0;
            virtual void addChild( SmartPtr<ISubComponent> child ) = 0;
            virtual void removeChild( SmartPtr<ISubComponent> child ) = 0;
            virtual Array<SmartPtr<ISubComponent>> getChildren() const = 0;

            /**
             * Gets the data associated with the component as a properties object.
             * @return The data associated with the component as a properties object.
             */
            SmartPtr<Properties> getProperties() const override = 0;

            /**
             * Sets the data associated with the component as a properties object.
             * @param properties The properties object to associate with this component.
             */
            void setProperties( SmartPtr<Properties> properties ) override = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif  // ISubComponent_h__
