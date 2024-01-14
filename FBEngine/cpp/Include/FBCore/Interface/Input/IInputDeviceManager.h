#ifndef IInputManager_h__
#define IInputManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /**
     * @class IInputDeviceManager
     * @brief Interface for managing input devices and events
     */
    class IInputDeviceManager : public ISharedObject
    {
    public:
        /**
         * @brief Destructor
         */
        ~IInputDeviceManager() override = default;

        /**
         * @brief Triggers an input event immediately
         * @param inputEvent The input event to trigger
         */
        virtual void triggerEvent( SmartPtr<IInputEvent> inputEvent ) = 0;

        /**
         * @brief Queues an input event for processing
         * @param event The input event to queue
         */
        virtual void queueEvent( SmartPtr<IInputEvent> event ) = 0;

        /**
         * @brief Posts an input event to the event queue
         * @param event The input event to post
         * @return True if the event was posted successfully, false otherwise
         */
        virtual bool postEvent( SmartPtr<IInputEvent> event ) = 0;

        virtual SmartPtr<IInputEvent> createInputEvent() = 0;

        virtual SmartPtr<IMouseState> createMouseState() = 0;
        virtual SmartPtr<IKeyboardState> createKeyboardState() = 0;

        virtual SmartPtr<IInputEvent> getCurrentInputEvent() const = 0;
        virtual SmartPtr<IMouseState> getCurrentMouseState() const = 0;
        virtual SmartPtr<IKeyboardState> getCurrentKeyboardState() const = 0;

        /**
         * @brief Adds a game input device to the manager
         * @param id The ID of the game input device
         * @return A smart pointer to the newly added game input device
         */
        virtual SmartPtr<IGameInput> addGameInput( hash_type id ) = 0;

        /**
         * @brief Finds a game input device in the manager by ID
         * @param id The ID of the game input device to find
         * @return A smart pointer to the found game input device, or nullptr if not found
         */
        virtual SmartPtr<IGameInput> findGameInput( hash_type id ) const = 0;

        /**
         * @brief Gets an array of all game input devices managed by this manager
         * @return An array of smart pointers to the game input devices
         */
        virtual Array<SmartPtr<IGameInput>> getGameInputs() const = 0;

        /**
         * @brief Gets the visibility of the system cursor
         * @return True if the cursor is visible, false otherwise
         */
        virtual bool isCursorVisible() const = 0;

        /**
         * @brief Sets the visibility of the system cursor
         * @param visible True to make the cursor visible, false to hide it
         */
        virtual void setCursorVisible( bool visible ) = 0;

        /**
         * @brief Adds an event listener to the manager
         * @param listener A smart pointer to the event listener to add
         */
        virtual void addListener( SmartPtr<IEventListener> listener ) = 0;

        /**
         * @brief Removes an event listener from the manager
         * @param listener A smart pointer to the event listener to remove
         */
        virtual void removeListener( SmartPtr<IEventListener> listener ) = 0;

        /**
         * @brief Removes all event listeners from the manager
         */
        virtual void removeListeners() = 0;

        /**
         * @brief Gets the current mouse scroll value
         * @return A vector representing the current mouse scroll value
         */
        virtual Vector3<real_Num> getMouseScroll() const = 0;

        /**
         * @brief Gets whether the Shift key is currently pressed
         * @return True if the Shift key is pressed, false otherwise
         */
        virtual bool isShiftPressed() const = 0;

        /**
         * @brief Sets whether the Shift key is currently pressed
         * @param shiftPressed True to set the Shift key as pressed, false to release it
         */
        virtual void setShiftPressed( bool shiftPressed ) = 0;

        virtual bool isMouseButtonDown( u32 button ) const = 0;

        /**
         * @brief Gets the time of the last click event.
         * @return The time of the last click event.
         */
        virtual f64 getLastClickTime() const = 0;

        /**
         * @brief Sets the time of the last click event.
         * @param lastClickTime The time of the last click event.
         */
        virtual void setLastClickTime( f64 lastClickTime ) = 0;

        /**
         * @brief Gets the time interval in which two clicks are considered a double-click.
         * @return The time interval in which two clicks are considered a double-click.
         */
        virtual f64 getDoubleClickInterval() const = 0;

        /**
         * @brief Sets the time interval in which two clicks are considered a double-click.
         * @param doubleClickInterval The time interval in which two clicks are considered a double-click.
         */
        virtual void setDoubleClickInterval( f64 doubleClickInterval ) = 0;

        /**
         * @brief Gets the time of the last input event.
         * @return The time of the last input event.
         */
        virtual f64 getLastInputTime() const = 0;

        /**
         * @brief Sets the time of the last input event.
         * @param lastInputTime The time of the last input event.
         */
        virtual void setLastInputTime( f64 lastInputTime ) = 0;

        /**
         * @brief Gets the input time
         * @return The input time
         */
        virtual f64 getInputTime() const = 0;

        /**
         * @brief Sets the input time
         * @param inputTime The new input time
         */
        virtual void setInputTime( f64 inputTime ) = 0;

        /**
         * @brief Adds the given input time to the current input time
         * @param inputTime The input time to add
         */
        virtual void addInputTime( f64 inputTime ) = 0;

        /**
         * @brief Checks if the given key is currently pressed
         * @param keyCode The key code to check
         * @return true if the key is pressed, false otherwise
         */
        virtual bool isKeyPressed( KeyCodes keyCode ) const = 0;

        /**
         * @brief Gets whether the mouse should be created
         * @return true if the mouse should be created, false otherwise
         */
        virtual bool getCreateMouse() const = 0;

        /**
         * @brief Sets whether the mouse should be created
         * @param createMouse true if the mouse should be created, false otherwise
         */
        virtual void setCreateMouse( bool createMouse ) = 0;

        /**
         * @brief Gets whether the keyboard should be created
         * @return true if the keyboard should be created, false otherwise
         */
        virtual bool getCreateKeyboard() const = 0;

        /**
         * @brief Sets whether the keyboard should be created
         * @param createKeyboard true if the keyboard should be created, false otherwise
         */
        virtual void setCreateKeyboard( bool createKeyboard ) = 0;

        /**
         * @brief Gets whether the joysticks should be created
         * @return true if the joysticks should be created, false otherwise
         */
        virtual bool getCreateJoysticks() const = 0;

        /**
         * @brief Sets whether the joysticks should be created
         * @param createJoysticks true if the joysticks should be created, false otherwise
         */
        virtual void setCreateJoysticks( bool createJoysticks ) = 0;

        /**
         * @brief Gets the window associated with the input device manager
         * @return The window associated with the input device manager
         */
        virtual SmartPtr<render::IWindow> getWindow() const = 0;

        /**
         * @brief Sets the window associated with the input device manager
         * @param window The window to associate with the input device manager
         */
        virtual void setWindow( SmartPtr<render::IWindow> window ) = 0;

        /**
         * @brief Retrieves the underlying object pointer
         * @param ppObject A pointer to a void pointer that will receive the underlying object pointer
         */
        virtual void _getObject( void **ppObject ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IInputManager_h__
