#ifndef __FB_Component_h__
#define __FB_Component_h__

#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Core/FSMListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Memory/Data.h>
#include <FBCore/Resource/Resource.h>

namespace fb
{
    namespace scene
    {
        /** Implementation of the IComponent component interface.
         *  Base class for a component object.
         */
        class Component : public Resource<IComponent>
        {
        public:
            /** Listener class for finite state machine. */
            class ComponentFSMListener : public FSMListener
            {
            public:
                /** Default constructor. */
                ComponentFSMListener();

                /** Virtual destructor. */
                ~ComponentFSMListener() override;

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

            /** Sets the flags of this component. */
            void setComponentFlag( u32 flag, bool value );

            /** Gets the flags of this component. */
            bool getComponentFlag( u32 flag );

            /** @copydoc IComponent::setEnabled */
            void setEnabled( bool enabled ) override;

            /** @copydoc IComponent::isEnabled */
            bool isEnabled() const override;

            /** @copydoc IComponent::isDirty */
            bool isDirty() const;

            /** @copydoc IComponent::setDirty */
            virtual void setDirty( bool dirty );

            /** @copydoc IComponent::toData */
            SmartPtr<ISharedObject> toData() const override;

            /** @copydoc IComponent::fromData */
            void fromData( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform( const Transform3<real_Num> &transform ) override;

            /** @copydoc IComponent::updateVisibility */
            void updateVisibility() override;

            /**
             * @brief Updates the materials of the UI component.
             */
            virtual void updateMaterials(){}

            /** @copydoc IComponent::setState */
            void setState( State state ) override;

            /** @copydoc IComponent::getState */
            State getState() const override;

            /** @copydoc IComponent::getEvents */
            Array<SmartPtr<IComponentEvent>> getEvents() const override;

            /** @copydoc IComponent::setEvents */
            void setEvents( Array<SmartPtr<IComponentEvent>> events ) override;

            /** @copydoc IComponent::addEvent */
            void addEvent( SmartPtr<IComponentEvent> event ) override;

            /** @copydoc IComponent::removeEvent */
            void removeEvent( SmartPtr<IComponentEvent> event ) override;

            /** @copydoc IComponent::addSubComponent */
            void addSubComponent( SmartPtr<ISubComponent> child ) override;

            /** @copydoc IComponent::removeSubComponent */
            void removeSubComponent( SmartPtr<ISubComponent> child ) override;

            /** @copydoc IComponent::removeSubComponentByIndex */
            void removeSubComponentByIndex( u32 index ) override;

            /** @copydoc IComponent::getNumSubComponents */
            u32 getNumSubComponents() const override;

            /** @copydoc IComponent::getSubComponents */
            Array<SmartPtr<ISubComponent>> getSubComponents() const override;

            /** @copydoc IComponent::setSubComponents */
            void setSubComponents( Array<SmartPtr<ISubComponent>> children ) override;

            /** @copydoc IComponent::getSubComponentsPtr */
            SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> getSubComponentsPtr() const override;

            /** @copydoc IComponent::setSubComponentsPtr */
            void setSubComponentsPtr(
                SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children ) override;

            /** @copydoc IComponent::getDirector */
            SmartPtr<IDirector> getDirector() const override;

            /** @copydoc IComponent::setDirector */
            void setDirector( SmartPtr<IDirector> director ) override;

            /** @copydoc IComponent::getComponentState */
            SmartPtr<IState> &getComponentState() override;

            /** @copydoc IComponent::getComponentState */
            const SmartPtr<IState> &getComponentState() const override;

            /** @copydoc IComponent::setComponentState */
            void setComponentState( SmartPtr<IState> state ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /**
             * BaseComponentStateListener is a class that serves as a base implementation for an IStateListener.
             * It provides functionality for handling state change messages and queries.
             */
            class ComponentStateListener : public IStateListener
            {
            public:
                /**
                 * Default constructor for BaseComponentStateListener.
                 */
                ComponentStateListener();

                /**
                 * Virtual destructor for BaseComponentStateListener.
                 */
                ~ComponentStateListener() override;

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

            SmartPtr<IState> m_componentState;

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
             * Whether the component is enabled or not.
             */
            atomic_bool m_enabled = true;

            /**
             * Whether the component is dirty or not.
             */
            atomic_bool m_isDirty = false;

            /**
             * The ID extension of the component.
             */
            static u32 m_idExt;
        };

    }  // namespace scene
}  // namespace fb

#endif  // BaseComponent_h__
