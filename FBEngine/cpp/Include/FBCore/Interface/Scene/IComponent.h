#ifndef __FB_ICOMPONENT_H__
#define __FB_ICOMPONENT_H__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Thread/ThreadTypes.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/Scene/IDirector.h>
#include <FBCore/Interface/Scene/ISubComponent.h>

namespace fb
{
    namespace scene
    {

        /**
         * Interface for a component object.
         * This class extends the IResource interface and provides functionality to manage the state and properties of a game object's components.
         * @author Zane Desir
         * @version 1.0
         */
        class IComponent : public IResource
        {
        public:
            /**
             * The state of a component.
             */
            enum class State
            {
                None,
                Create,
                Destroyed,
                Edit,
                Play,
                Reset,

                Count
            };

            static const hash_type actorFlagsChanged;
            static const hash_type actorReset;
            static const hash_type actorUnload;
            static const hash_type sceneWasLoaded;
            static const hash_type parentChanged;
            static const hash_type hierarchyChanged;
            static const hash_type childAdded;
            static const hash_type childRemoved;
            static const hash_type childAddedInHierarchy;
            static const hash_type childRemovedInHierarchy;

            static const hash_type visibilityChanged;
            static const hash_type enabledChanged;
            static const hash_type staticChanged;

            static const hash_type triggerCollisionEnter;
            static const hash_type triggerCollisionLeave;
            static const hash_type componentLoaded;

            /** Virtual destructor. */
            ~IComponent() override = default;

            /**
             * Updates the component when it's dirty.
             * @param flags An integer bitmask representing the dirty flags.
             * @param oldFlags An integer bitmask representing the old dirty flags.
             */
            virtual void updateFlags( u32 flags, u32 oldFlags ) = 0;

            /**
             * Marks the component as dirty on all tasks.
             */
            virtual void makeDirty() = 0;

            /**
             * Marks the component as dirty on a specific task.
             * @param task The task that should update the dirty state.
             */
            virtual void makeDirty( Thread::Task task ) = 0;

            /**
             * Gets the actor that this component is attached to.
             * @return The actor this component is attached to, or null if it is not attached to any actor.
             */
            virtual SmartPtr<IActor> &getActor() = 0;

            /**
             * Gets the actor that this component is attached to.
             * @return The actor this component is attached to, or null if it is not attached to any actor.
             */
            virtual const SmartPtr<IActor> &getActor() const = 0;

            /**
             * Sets the actor that this component is attached to.
             * @param actor The actor this component should be attached to, or null if it should not be attached to any actor.
             */
            virtual void setActor( SmartPtr<IActor> actor ) = 0;

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

            /**
             * Updates the component using the actor transform.
             */
            virtual void updateTransform() = 0;

            /**
             * Updates the component using the transform passed.
             */
            virtual void updateTransform( const Transform3<real_Num> &transform ) = 0;

            /**
             * Updates the component visibility.
             */
            virtual void updateVisibility() = 0;

            /**
             * Updates the component materials.
             */
            virtual void updateMaterials() = 0;

            /**
             * Updates the component dependencies.
             */
            virtual void updateDependentComponents() = 0;

            /**
             * Sets the component state.
             * @param state The new state for the component.
             */
            virtual void setState( State state ) = 0;

            /**
             * Sets whether the component is enabled.
             * @param enabled True if the component is enabled.
             */
            virtual void setEnabled( bool enabled ) = 0;

            /** Gets if the component is enabled.
            @return true if the component is enabled, false otherwise.
            */
            virtual bool isEnabled() const = 0;

            /** Gets the component state.
            @return The component state.
            */
            virtual State getState() const = 0;

            /** Gets the component events.
            @return The component events.
            */
            virtual Array<SmartPtr<IComponentEvent>> getEvents() const = 0;

            /** Sets the component events.
            @param events The component events.
            */
            virtual void setEvents( Array<SmartPtr<IComponentEvent>> events ) = 0;

            /** Adds a component event.
            @param event The component event to add.
            */
            virtual void addEvent( SmartPtr<IComponentEvent> event ) = 0;

            /** Removes a component event.
            @param event The component event to remove.
            */
            virtual void removeEvent( SmartPtr<IComponentEvent> event ) = 0;

            /**
             * Add a child component to this component.
             *
             * @param child A shared pointer to the child component.
             */
            virtual void addSubComponent( SmartPtr<ISubComponent> child ) = 0;

            /**
             * Remove a child component from this component.
             *
             * @param child A shared pointer to the child component.
             */
            virtual void removeSubComponent( SmartPtr<ISubComponent> child ) = 0;

            /**
             * Remove a child component from this component.
             *
             * @param index The index to the sub component.
             */
            virtual void removeSubComponentByIndex( u32 index ) = 0;

            /**
             * Gets the number of sub components.
             */
            virtual u32 getNumSubComponents() const = 0;

            /** Gets a sub component by index. */
            virtual SmartPtr<ISubComponent> getSubComponentByIndex( u32 index ) const = 0;

            /**
             * Get an array of the child components of this component.
             *
             * @return An array of shared pointers to the child components.
             */
            virtual Array<SmartPtr<ISubComponent>> getSubComponents() const = 0;

            /**
             * Set the child components of this component.
             *
             * @param children An array of shared pointers to the child components.
             */
            virtual void setSubComponents( Array<SmartPtr<ISubComponent>> children ) = 0;

            /**
             * Get a pointer to the concurrent array of child components of this component.
             *
             * @return A shared pointer to the concurrent array of child components.
             */
            virtual SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> getSubComponentsPtr() const = 0;

            /**
             * Set the pointer to the concurrent array of child components of this component.
             *
             * @param children A shared pointer to the concurrent array of child components.
             */
            virtual void setSubComponentsPtr(
                SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children ) = 0;

            virtual SmartPtr<IState> &getComponentState() = 0;

            virtual const SmartPtr<IState> &getComponentState() const = 0;

            virtual void setComponentState( SmartPtr<IState> state ) = 0;

            /** Compares the tag with the actor's tag. */
            virtual bool compareTag( const String &tag ) const = 0;

            virtual Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                           const Array<Parameter> &arguments,
                                           SmartPtr<ISharedObject> sender,
                                           SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) = 0;

            template <class T>
            SmartPtr<T> getComponentStateByType() const;

            /**
             * Adds a sub component component.
             *
             * @tparam T The type of component to add.
             * @return A SmartPtr to the new component.
             */
            template <class T>
            SmartPtr<T> addSubComponentByType();

            /**
             * Removes a sub component component.
             *
             * @tparam T The type of component to remove.
             */
            template <class T>
            void removeSubComponentByType();

            /**
             * Returns an array of all child components that are derived from the specified type.
             * @tparam T The component type to search for.
             * @return An array of SmartPtrs to child components of type T.
             */
            template <class T>
            Array<SmartPtr<T>> getSubComponentsByType() const;

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        SmartPtr<T> IComponent::addSubComponentByType()
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto component = factoryManager->make_ptr<T>();
            addSubComponent( component );
            component->load( nullptr );
            return component;
        }

        template <class T>
        void IComponent::removeSubComponentByType()
        {
            auto subComponents = getSubComponentsByType<T>();
            if( !subComponents.empty() )
            {
                auto subComponent = subComponents.front();
                removeSubComponent( subComponent );
            }
        }

        template <class T>
        Array<SmartPtr<T>> IComponent::getSubComponentsByType() const
        {
            // Get all child components of this component.
            auto children = getSubComponents();

            // Create an array for storing child components of the specified type.
            Array<SmartPtr<T>> childrenByType;
            childrenByType.reserve( children.size() );

            // Loop over all child components.
            for( auto &child : children )
            {
                // Check if the child component is derived from the specified type.
                if( child->isDerived<T>() )
                {
                    // If the child is of the specified type, add it to the array of components of that type.
                    childrenByType.push_back( child );
                }
            }

            // Return the array of child components that are of the specified type.
            return childrenByType;
        }

        template <class T>
        SmartPtr<T> IComponent::getComponentStateByType() const
        {
            auto &state = getComponentState();
            return (T *)state.get();
        }

    }  // namespace scene
}  // end namespace fb

#endif
