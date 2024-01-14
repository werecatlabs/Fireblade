#ifndef UIComponent_h__
#define UIComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace scene
    {
        /**
         * @class UIComponent
         * @brief UIComponent is a user interface component that inherits from BaseComponent.
         *
         * This class provides functionality for UI elements and manages their properties,
         * state, visibility, input handling, and more.
         */
        class UIComponent : public Component
        {
        public:
            /**
             * @brief Default constructor.
             */
            UIComponent();

            /**
             * @brief Destructor.
             */
            ~UIComponent() override;

            /**
             * @brief Loads the UIComponent data.
             * @copydoc Component::load
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Unloads the UIComponent data.
             * @copydoc Component::unload
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Gets the UI element listener.
             * @return A smart pointer to the UI element listener.
             */
            SmartPtr<IEventListener> getElementListener() const;

            /**
             * @brief Sets the UI element listener.
             * @param elementListener A smart pointer to the new UI element listener.
             */
            void setElementListener( SmartPtr<IEventListener> elementListener );

            /**
             * @brief Gets the UI element.
             * @return A smart pointer to the UI element.
             */
            SmartPtr<ui::IUIElement> getElement() const;

            /**
             * @brief Sets the UI element.
             * @param element A smart pointer to the new UI element.
             */
            void setElement( SmartPtr<ui::IUIElement> element );

            /**
             * @brief Gets the canvas.
             * @return A smart pointer to the UIComponent canvas.
             */
            SmartPtr<UIComponent> getCanvas() const;

            /**
             * @brief Sets the canvas.
             * @param canvas A smart pointer to the new UIComponent canvas.
             */
            void setCanvas( SmartPtr<UIComponent> canvas );

            /**
             * @brief Updates the dimensions of the UI component.
             */
            virtual void updateDimensions();

            /**
             * @brief Updates the materials of the UI component.
             */
            void updateMaterials() override;

            /**
             * @brief Gets the child objects of the UI component.
             * @copydoc Component::getChildObjects
             */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /**
             * @brief Gets the properties of the UI component.
             * @copydoc Component::getProperties
             */
            SmartPtr<Properties> getProperties() const override;

            /**
             * @brief Sets the properties of the UI component.
             * @copydoc Component::setProperties
             */
            void setProperties( SmartPtr<Properties> properties ) override;

            /**
             * @brief Updates the dirty state of the UI component.
             * @copydoc Component::updateDirty
             */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /**
             * @brief Updates the transform of the UI component.
             * @copydoc Component::updateTransform
             */
            void updateTransform() override;

            /**
             * @brief Gets if to cascade the input.
             */
            bool getCascadeInput() const;

            /**
             * @brief Sets if to cascade the input.
             */
            void setCascadeInput( bool cascadeInput );

            /**
             * @brief Updates the z order.
             */
            void updateOrder();

            u32 getZOrder() const;

            void setZOrder( u32 zOrder );

            Array<SmartPtr<IActor>> getActorListeners() const;

            Array<SmartPtr<IComponent>> getComponentListeners() const;

            void addActorListener( SmartPtr<IActor> actor );

            void removeActorListener( SmartPtr<IActor> actor );

            void addListener( SmartPtr<IComponent> component );

            void removeListener( SmartPtr<IComponent> component );

            FB_CLASS_REGISTER_DECL;

        protected:
            /**
             * @class UIElementListener
             * @brief A class that inherits from ui::CUIElementListener and listens for events on a UIComponent.
             *
             * UIElementListener is designed to handle events and manage the relationship between the listener
             * and its owner (a UIComponent). The class provides methods to set and get the owner, as well as
             * handling events.
             */
            class UIElementListener : public IEventListener
            {
            public:
                /**
                 * @brief Default constructor.
                 */
                UIElementListener() = default;

                /**
                 * @brief Default destructor.
                 */
                ~UIElementListener() override = default;

                /**
                 * @brief Handles the specified event type and updates the UIComponent.
                 *
                 * @param eventType The type of the event.
                 * @param eventValue The value associated with the event.
                 * @param arguments An array of parameters related to the event.
                 * @param sender A smart pointer to the object that sent the event.
                 * @param event A smart pointer to the event object.
                 * @return A Parameter object as a result of handling the event.
                 */
                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                /**
                 * @brief Retrieves the owner of this UIElementListener.
                 *
                 * @return A pointer to the UIComponent that owns this UIElementListener.
                 */
                SmartPtr<UIComponent> getOwner() const;

                /**
                 * @brief Sets the owner of this UIElementListener.
                 *
                 * @param owner A pointer to the UIComponent that will own this UIElementListener.
                 */
                void setOwner( SmartPtr<UIComponent> owner );

            protected:
                /**
                 * @brief A pointer to the UIComponent that owns this UIElementListener.
                 */
                SmartPtr<UIComponent> m_owner;
            };

            /**
             * @brief Handles the specified event type and updates the UIComponent.
             *
             * @param eventType The type of the event.
             * @param eventValue The value associated with the event.
             * @param arguments An array of parameters related to the event.
             * @param sender A smart pointer to the object that sent the event.
             * @param event A smart pointer to the event object.
             * @return A Parameter object as a result of handling the event.
             */
            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            /**
             * @brief Handles component events within the context of the finite state machine (FSM).
             *
             * @param state The current state of the FSM.
             * @param eventType The type of event to handle.
             * @return The return type of the FSM after handling the event.
             */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /**
             * @brief Sets up the canvas for the UIComponent.
             *
             * This method is responsible for initializing the canvas for the UIComponent.
             * It should be implemented by derived classes if additional setup is needed.
             */
            virtual void setupCanvas();

            /**
             * @brief Creates the user interface for the UIComponent.
             *
             * This method is responsible for creating and initializing UI elements.
             * It should be implemented by derived classes to define the UI structure.
             */
            virtual void createUI();

            /**
             * @brief Updates the visibility of the UIComponent based on its current state.
             *
             * This method is called to refresh the visibility of the UIComponent when its state changes.
             * The default implementation can be overridden by derived classes if custom behavior is needed.
             */
            void updateVisibility() override;

            /**
             * @brief Updates the state of UI elements within the UIComponent.
             *
             * This method is called to refresh the state of UI elements based on the current state
             * of the UIComponent. It should be implemented by derived classes if additional state
             * management is needed.
             */
            virtual void updateElementState();

            Array<SmartPtr<IActor>> m_actorListeners;
            Array<SmartPtr<IComponent>> m_componentListeners;

            /**
             * @brief A smart pointer to the UIElementListener object responsible for handling UI events.
             */
            SmartPtr<IEventListener> m_elementListener;

            /**
             * @brief A smart pointer to the IUIElement object representing a UI element in the UIComponent.
             */
            SmartPtr<ui::IUIElement> m_element;

            /**
             * @brief A smart pointer to the UIComponent object representing the canvas on which the UIComponent is drawn.
             */
            SmartPtr<UIComponent> m_canvas;

            /**
             * @brief The ui component's z order.
             */
            u32 m_zOrder = 0;

            /**
             * @brief A boolean flag indicating whether the input events should be cascaded down to child UIComponents.
             *
             * If true, input events will be passed down to child components. If false, input events will be consumed
             * by the current UIComponent and not propagated to children.
             */
            bool m_cascadeInput = true;
        };
    }  // namespace scene
}  // namespace fb

#endif  // UIComponent_h__
