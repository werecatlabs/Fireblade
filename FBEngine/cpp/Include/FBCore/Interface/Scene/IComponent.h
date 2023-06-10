#ifndef __FB_ICOMPONENT_H__
#define __FB_ICOMPONENT_H__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Thread/ThreadTypes.h>
#include <FBCore/Interface/IApplicationManager.h>
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

            /** Virtual destructor. */
            ~IComponent() override = default;

            /**
             * Updates the component when it's dirty.
             * @param flags An integer bitmask representing the dirty flags.
             * @param oldFlags An integer bitmask representing the old dirty flags.
             */
            virtual void updateDirty( u32 flags, u32 oldFlags ) = 0;

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
            virtual SmartPtr<IActor> getActor() const = 0;

            /**
             * Sets the actor that this component is attached to.
             * @param actor The actor this component should be attached to, or null if it should not be attached to any actor.
             */
            virtual void setActor( SmartPtr<IActor> actor ) = 0;

            /**
             * Gets the component's state object.
             * @return The state object associated with this component, or null if no state object has been set.
             */
            SmartPtr<IStateContext> getStateObject() const override = 0;

            /**
             * Sets the component's state object.
             * @param stateObject The state object to associate with this component.
             */
            void setStateObject( SmartPtr<IStateContext> stateObject ) override = 0;

            /**
             * Sets up the component registration.
             */
            virtual void setupUpdateRegistration() = 0;

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
             * Updates the component visibility.
             */
            virtual void updateVisibility() = 0;

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

            /** Called when the actor is reset. */
            virtual void actorReset() = 0;

            /** Called when the actor is unloaded. */
            virtual void actorUnload() = 0;

            /** Called when the component is reset.  */
            virtual void reset() = 0;

            /** Called when the component is created. */
            virtual void awake() = 0;

            /** Called when the component is destroyed. */
            virtual void destroy() = 0;

            /** Called when entering play mode. */
            virtual void play() = 0;

            /** Called when entering edit mode. */
            virtual void edit() = 0;

            /** Called when the level was loaded.
            @param level An id of the level loaded.
            */
            virtual void levelWasLoaded( s32 level ) = 0;

            /** Called when the actor parent has changed.
            @param newParent The actor this component's owner has been newly parented to.
            @param oldParent The actor this component's owner was parented to.
            */
            virtual void parentChanged( SmartPtr<IActor> newParent, SmartPtr<IActor> oldParent ) = 0;

            /** Triggered when the hierarchy changes. */
            virtual void hierarchyChanged() = 0;

            /** Triggered when a child is added.
            @param child The child that was added.
            */
            virtual void childAdded( SmartPtr<IActor> child ) = 0;

            /** Triggered when a child is removed.
            @param child The child that was removed.
            */
            virtual void childRemoved( SmartPtr<IActor> child ) = 0;

            /** Triggered when a child is added.
            @param child The child that was added.
            */
            virtual void childAddedInHierarchy( SmartPtr<IActor> child ) = 0;

            /** Triggered when a child is removed.
            @param child The child that was removed.
            */
            virtual void childRemovedInHierarchy( SmartPtr<IActor> child ) = 0;

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
             * @param child A shared pointer to the child component.
             */
            virtual void removeSubComponentByIndex( u32 index ) = 0;

            /**
             * Gets the number of sub components.
             */
            virtual u32 getNumSubComponents() const = 0;

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

            virtual SmartPtr<IDirector> getDirector() const = 0;

            virtual void setDirector( SmartPtr<IDirector> director ) = 0;

            virtual SmartPtr<IState> &getComponentState() = 0;

            virtual const SmartPtr<IState> &getComponentState() const = 0;

            virtual void setComponentState( SmartPtr<IState> state ) = 0;

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

            /**
            @brief Creates a component of the given type using a director and properties.
            @param director The director that should create the component.
            @returns The newly created component.
            */
            template <class T>
            static void create( SmartPtr<IDirector> director );



            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        SmartPtr<T> IComponent::addSubComponentByType()
        {
            auto applicationManager = core::IApplicationManager::instance();
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
        void IComponent::create( SmartPtr<IDirector> director )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto component = factoryManager->make_shared<T>();
            FB_ASSERT( component );

            auto properties = director->getProperties();
            component->setProperties( properties );
            return component;
        }

        template <class T>
        SmartPtr<T> IComponent::getComponentStateByType() const
        {
            auto& state = getComponentState();
            return (T *)state.get();
        }

    }  // namespace scene
}  // end namespace fb

#endif
