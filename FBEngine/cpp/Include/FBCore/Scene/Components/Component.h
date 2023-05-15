#ifndef __BaseComponent_h__
#define __BaseComponent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/FSM/FSMListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Base/PropertiesUtil.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Resource/CResource.h>

namespace fb
{
    namespace scene
    {
        /** Base class for a component object. */
        class Component : public CResource<IComponent>
        {
        public:
            /** Listener class for finite state machine. */
            class BaseComponentFSMListener : public FSMListener
            {
            public:
                /** Default constructor. */
                BaseComponentFSMListener();

                /** Virtual destructor. */
                ~BaseComponentFSMListener() override;

                /** Handle finite state machine event. */
                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                /** Get the owner of the listener. */
                Component *getOwner() const;

                /** Set the owner of the listener. */
                void setOwner( Component *owner );

            protected:
                Component *m_owner = nullptr;
            };

            /** Default constructor. */
            Component();

            /** Copy constructor. */
            Component( const Component &other ) = delete;

            /** Virtual destructor. */
            ~Component() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::preUpdateDirty */
            void preUpdateDirty();

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc IComponent::postUpdateDirty */
            void postUpdateDirty();

            /** @copydoc IComponent::setDirty */
            void setDirty( Thread::Task taskId, Thread::UpdateState updateType, bool dirty );

            /** @copydoc IComponent::isDirty */
            bool isDirty( Thread::Task taskId, Thread::UpdateState updateType ) const;

            /** @copydoc IComponent::makeDirty */
            void makeDirty() override;

            /** @copydoc IComponent::makeDirty */
            void makeDirty( Thread::Task task ) override;

            /** @copydoc IComponent::getActor */
            SmartPtr<IActor> getActor() const override;

            /** @copydoc IComponent::setActor */
            void setActor( SmartPtr<IActor> actor ) override;

            /** @copydoc IComponent::getStateObject */
            SmartPtr<IStateContext> getStateObject() const override;

            /** @copydoc IComponent::setStateObject */
            void setStateObject( SmartPtr<IStateContext> stateObject ) override;

            /** @copydoc IComponent::actorReset */
            void actorReset() override;

            /** @copydoc IComponent::actorUnload */
            void actorUnload() override;

            /** @copydoc IComponent::reset */
            void reset() override;

            /** @copydoc IComponent::awake */
            void awake() override;

            /** @copydoc IComponent::destroy */
            void destroy() override;

            /** @copydoc IComponent::start */
            void play();

            /** @copydoc IComponent::edit */
            void edit();

            /** @copydoc IComponent::levelWasLoaded */
            void levelWasLoaded( s32 level ) override;

            /** @copydoc IComponent::parentChanged */
            void parentChanged( SmartPtr<IActor> newParent, SmartPtr<IActor> oldParent ) override;

            /** @copydoc IComponent::hierarchyChanged */
            void hierarchyChanged() override;

            /** @copydoc IComponent::childAdded */
            void childAdded( SmartPtr<IActor> child ) override;

            /** @copydoc IComponent::childRemoved */
            void childRemoved( SmartPtr<IActor> child ) override;

            /** @copydoc IComponent::childAddedInHierarchy */
            void childAddedInHierarchy( SmartPtr<IActor> child ) override;

            /** @copydoc IComponent::childRemovedInHierarchy */
            void childRemovedInHierarchy( SmartPtr<IActor> child ) override;

            /** @copydoc IComponent::visibilityChanged */
            virtual void visibilityChanged();

            /** @copydoc IComponent::enable */
            void enable();

            /** @copydoc IComponent::disable */
            void disable();

            /** @copydoc IComponent::triggerEnter */
            void triggerEnter( SmartPtr<IComponent> collision );

            /** @copydoc IComponent::triggerLeave */
            void triggerLeave( SmartPtr<IComponent> collision );

            void componentLoaded( SmartPtr<IComponent> component );

            /** Gets the component id. */
            hash32 getComponentId() const;

            /** Sets the component id. */
            void _setComponentId( hash32 val );

            /** Gets the component type as String. */
            virtual const String &getComponentType() const;

            /** Gets the component type as an integer. */
            virtual hash32 getComponentTypeId() const;

            /** Sets the flags of this component. */
            void setComponentFlag( u32 flag, bool value );

            /** Gets the flags of this component. */
            bool getComponentFlag( u32 flag );

            /** @copydoc IComponent::setEnabled */
            void setEnabled( bool enabled ) override;

            /** @copydoc IComponent::isEnabled */
            bool isEnabled() const override;

            /** @copydoc IComponent::setupUpdateRegistration */
            void setupUpdateRegistration() override;

            /** @copydoc IComponent::isDirty */
            bool isDirty() const;

            /** @copydoc IComponent::setDirty */
            virtual void setDirty( bool dirty );

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            /** @copydoc IComponent::updateVisibility */
            void updateVisibility() override;

            /** @copydoc IComponent::compareTag */
            bool compareTag( const String &tag ) const;

            /** @copydoc IComponent::handleActorReparent */
            void handleActorReparent();

            /**
             * Sets the state of the component.
             * @param state The state to set.
             */
            void setState( State state ) override;

            /**
             * Gets the state of the component.
             * @return The state of the component.
             */
            State getState() const override;

            /**
             * Gets the entity ID associated with the component.
             * @return The entity ID.
             */
            u32 getEntity() const override;

            /**
             * Sets the entity ID associated with the component.
             * @param entity The entity ID to set.
             */
            void setEntity( u32 entity ) override;

            /**
             * Gets a pointer to the data for the component.
             * @tparam T The type of data to get a pointer for.
             * @return A pointer to the data of the specified type, or null if not found.
             */
            template <class T>
            T *getDataPtrByType() const;

            /**
             * Gets a pointer to the data for the component.
             * @return A pointer to the data.
             */
            void *getDataPtr() const;

            /**
             * Sets the data pointer for the component.
             * @param data The data pointer to set.
             */
            void setDataPtr( void *data );

            /**
             * Gets the list of events associated with the component.
             * @return The list of events.
             */
            Array<SmartPtr<IComponentEvent>> getEvents() const override;

            /**
             * Sets the list of events associated with the component.
             * @param events The list of events to set.
             */
            void setEvents( Array<SmartPtr<IComponentEvent>> events ) override;

            /**
             * Adds an event to the list of events associated with the component.
             * @param event The event to add.
             */
            void addEvent( SmartPtr<IComponentEvent> event ) override;

            /**
             * Removes an event from the list of events associated with the component.
             * @param event The event to remove.
             */
            void removeEvent( SmartPtr<IComponentEvent> event ) override;

            /** @copydoc BaseComponent::addChild */
            void addSubComponent( SmartPtr<ISubComponent> child ) override;

            /** @copydoc BaseComponent::removeChild */
            void removeSubComponent( SmartPtr<ISubComponent> child ) override;

            void removeSubComponentByIndex( u32 index ) override;

            u32 getNumSubComponents() const override;

            /** @copydoc BaseComponent::getChildren */
            Array<SmartPtr<ISubComponent>> getSubComponents() const override;

            /** @copydoc BaseComponent::setChildren */
            void setSubComponents( Array<SmartPtr<ISubComponent>> children ) override;

            /** @copydoc BaseComponent::getChildrenPtr */
            SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> getSubComponentsPtr() const override;

            /** @copydoc BaseComponent::setChildrenPtr */
            void setSubComponentsPtr(
                SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children ) override;

            SmartPtr<IDirector> getDirector() const override;

            void setDirector( SmartPtr<IDirector> director ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /**
             * BaseComponentStateListener is a class that serves as a base implementation for an IStateListener.
             * It provides functionality for handling state change messages and queries.
             */
            class BaseComponentStateListener : public CSharedObject<IStateListener>
            {
            public:
                /**
                 * Default constructor for BaseComponentStateListener.
                 */
                BaseComponentStateListener();

                /**
                 * Virtual destructor for BaseComponentStateListener.
                 */
                ~BaseComponentStateListener() override;

                /**
                 * Implementation of the IStateListener::handleStateChanged method that handles a state change message.
                 * @param message The state change message to handle.
                 */
                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                /**
                 * Implementation of the IStateListener::handleStateChanged method that handles a state change.
                 * @param state The state to handle.
                 */
                void handleStateChanged( SmartPtr<IState> &state ) override;

                /**
                 * Implementation of the IStateListener::handleQuery method that handles a state query.
                 * @param query The state query to handle.
                 */
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                /**
                 * Gets the component associated with this state listener.
                 * @return The component associated with this state listener.
                 */
                Component *getComponent() const;

                /**
                 * Sets the component associated with this state listener.
                 * @param component The component to associate with this state listener.
                 */
                void setComponent( Component *component );

            protected:
                Component *m_component = nullptr;
            };

            /**
             * Handle an event in the component's finite state machine.
             *
             * @param state The current state of the state machine.
             * @param eventType The type of the event to handle.
             *
             * @return The next state to transition to.
             */
            virtual IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType );

            /**
             * The children of the component.
             */
            SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> m_children;

            /**
             * The events of the component.
             */
            Array<SmartPtr<IComponentEvent>> m_events;

            /**
             * The finite state machine of the component.
             */
            SmartPtr<IFSM> m_componentFSM;

            /**
             * The listener for the finite state machine of the component.
             */
            SmartPtr<IFSMListener> m_componentFsmListener;

            /**
             * The director object of the component.
             */
            SmartPtr<IDirector> m_director;

            /**
             * The state object of the component.
             */
            SmartPtr<IStateContext> m_stateObject;

            /**
             * The listener for the state of the component.
             */
            SmartPtr<IStateListener> m_stateListener;

            /**
             * The parent of the component.
             */
            SmartPtr<IComponent> m_parent;

            /**
             * The actor to which the component is attached.
             */
            WeakPtr<IActor> m_actor;

            /**
             * The owner of the component, used as data.
             */
            WeakPtr<ISharedObject> m_owner;

            /**
             * The flags of the component.
             */
            atomic_u32 m_componentFlags = 0;

            /**
             * The entity ID of the component.
             */
            u32 m_entity = 0;

            /**
             * Whether the component is enabled or not.
             */
            atomic_bool m_enabled = true;

            /**
             * Whether the component is dirty or not.
             */
            atomic_bool m_isDirty = false;

            /**
             * The data pointer of the component.
             */
            void *m_dataPtr = nullptr;

            /**
             * The ID extension of the component.
             */
            static u32 m_idExt;
        };

        template <class T>
        T *Component::getDataPtrByType() const
        {
            return static_cast<T *>( m_dataPtr );
        }

    }  // namespace scene
}  // namespace fb

#endif  // BaseComponent_h__
