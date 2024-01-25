#ifndef __FB_Component_h__
#define __FB_Component_h__

#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Resource/Resource.h>
#include <FBCore/Core/FSMListener.h>
#include <FBCore/Atomics/Atomics.h>

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
            static const String enabledStr;
            static const String dirtyStr;

            /** Listener class for finite state machine. */
            class ComponentFSMListener : public FSMListener
            {
            public:
                /** Default constructor. */
                ComponentFSMListener();

                /** Virtual destructor. */
                ~ComponentFSMListener() override;

                void unload( SmartPtr<ISharedObject> data ) override;

                /** Handle finite state machine event. */
                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                /** Get the owner of the listener. */
                SmartPtr<Component> getOwner() const;

                /** Set the owner of the listener. */
                void setOwner( SmartPtr<Component> owner );

            protected:
                SmartPtr<Component> m_owner;
            };

            /** Default constructor. */
            Component();

            /** Copy constructor. */
            Component( const Component &other ) = delete;

            /** Virtual destructor. */
            ~Component() override;

            /** @copydoc IComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::updateFlags */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** @copydoc IComponent::getActor */
            SmartPtr<IActor> &getActor() override;

            /** @copydoc IComponent::getActor */
            const SmartPtr<IActor> &getActor() const override;

            /** @copydoc IComponent::setActor */
            void setActor( SmartPtr<IActor> actor ) override;

            /** Gets the flags of this component. */
            u32 getComponentFlags() const override;

            /** Sets the flags of this component. */
            void setComponentFlags( u32 flags ) override;

            /** Sets the flags of this component. */
            void setComponentFlag( u32 flag, bool value ) override;

            /** Gets the flags of this component. */
            bool getComponentFlag( u32 flag ) const override;

            /** @copydoc IComponent::setEnabled */
            void setEnabled( bool enabled ) override;

            /** @copydoc IComponent::isEnabled */
            bool isEnabled() const override;

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
            void updateMaterials() override;

            /** @copydoc IComponent::updateDependentComponents */
            void updateDependentComponents() override;

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

            /** @copydoc IComponent::getSubComponentByIndex */
            SmartPtr<ISubComponent> getSubComponentByIndex( u32 index ) const override;

            /** @copydoc IComponent::getSubComponents */
            Array<SmartPtr<ISubComponent>> getSubComponents() const override;

            /** @copydoc IComponent::setSubComponents */
            void setSubComponents( Array<SmartPtr<ISubComponent>> children ) override;

            /** @copydoc IComponent::getSubComponentsPtr */
            SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> getSubComponentsPtr() const override;

            /** @copydoc IComponent::setSubComponentsPtr */
            void setSubComponentsPtr(
                SharedPtr<ConcurrentArray<SmartPtr<ISubComponent>>> children ) override;

            /** @copydoc IComponent::getComponentState */
            SmartPtr<IState> &getComponentState() override;

            /** @copydoc IComponent::getComponentState */
            const SmartPtr<IState> &getComponentState() const override;

            /** @copydoc IComponent::setComponentState */
            void setComponentState( SmartPtr<IState> state ) override;

            /** Compares the tag with the actor's tag. */
            bool compareTag( const String &tag ) const override;

            /** @copydoc IComponent::handleEvent */
            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

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
             * The finite state machine of the component.
             */
            SmartPtr<IFSM> m_componentFSM;

            /**
             * The listener for the finite state machine of the component.
             */
            SmartPtr<IFSMListener> m_componentFsmListener;

            /**
             * The parent of the component.
             */
            SmartPtr<IComponent> m_parent;

            /**
             * The actor to which the component is attached.
             */
            SmartPtr<IActor> m_actor;

            /**
             * The flags of the component.
             */
            atomic_u32 m_componentFlags = 0;

            /**
             * The events of the component.
             */
            Array<SmartPtr<IComponentEvent>> m_events;

            /**
             * The ID extension of the component.
             */
            static u32 m_idExt;
        };
    } // namespace scene
}     // namespace fb

#endif  // BaseComponent_h__
