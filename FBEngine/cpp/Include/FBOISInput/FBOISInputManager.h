#ifndef __FB_OISInputManager_H__
#define __FB_OISInputManager_H__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Graphics/IWindowListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/FixedArray.h>
#include <FBCore/Base/Map.h>
#include <FBCore/Base/HashMap.h>
#include <FBOISInput/Extern/OIS/OISKeyboard.h>
#include <FBOISInput/Extern/OIS/OISJoyStick.h>
#include <FBOISInput/Extern/OIS/OISMouse.h>

namespace fb
{
    class OISInputManager : public CSharedObject<IInputDeviceManager>
    {
    public:
        class KeyListener : public OIS::KeyListener
        {
        public:
            KeyListener() = default;
            ~KeyListener() override = default;

            bool keyPressed( const OIS::KeyEvent &arg ) override;
            bool keyReleased( const OIS::KeyEvent &arg ) override;

            OISInputManager *getOwner() const;

            void setOwner( OISInputManager *owner );

        protected:
            OISInputManager *m_owner = nullptr;
        };

        class MouseListener : public OIS::MouseListener
        {
        public:
            MouseListener() = default;
            ~MouseListener() override = default;

            bool mouseMoved( const OIS::MouseEvent &arg ) override;

            bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) override;
            bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) override;

            OISInputManager *getOwner() const;

            void setOwner( OISInputManager *owner );

        protected:
            OISInputManager *m_owner = nullptr;
        };

        class JoyStickListener : public OIS::JoyStickListener
        {
        public:
            JoyStickListener() = default;
            ~JoyStickListener() override = default;

            bool povMoved( const OIS::JoyStickEvent &arg, int index ) override;

            bool vector3Moved( const OIS::JoyStickEvent &arg, int index ) override;

            bool buttonPressed( const OIS::JoyStickEvent &arg, int button ) override;

            bool sliderMoved( const OIS::JoyStickEvent &arg, int index ) override;

            bool axisMoved( const OIS::JoyStickEvent &arg, int axis ) override;

            bool buttonReleased( const OIS::JoyStickEvent &arg, int button ) override;

            OISInputManager *getOwner() const;

            void setOwner( OISInputManager *owner );

        protected:
            OISInputManager *m_owner = nullptr;
        };

        OISInputManager();
        // Constructor takes a RenderWindow because it uses that to determine input context
        OISInputManager( SmartPtr<render::IWindow> win, bool bufferedKeys = true,
                         bool bufferedMouse = true, bool bufferedJoy = true );
        ~OISInputManager() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void update() override;

        bool postEvent( SmartPtr<IInputEvent> event ) override;

        SmartPtr<IGameInput> addGameInput( hash32 id ) override;
        SmartPtr<IGameInput> findGameInput( hash32 id ) const override;
        Array<SmartPtr<IGameInput>> getGameInputs() const override;

        bool isCursorVisible() const override;
        void setCursorVisible( bool visible ) override;

        OIS::InputManager *getInputManager() const;
        void setInputManager( OIS::InputManager *inputManager );

        OIS::Mouse *getMouse() const;
        void setMouse( OIS::Mouse *val );

        OIS::Keyboard *getKeyboard() const;
        void setKeyboard( OIS::Keyboard *val );

        Array<RawPtr<OIS::JoyStick>> getJoySticks() const;
        void setJoySticks( Array<RawPtr<OIS::JoyStick>> val );

        Array<RawPtr<OIS::ForceFeedback>> getForceFeedbackDevices() const;
        void setForceFeedbackDevices( Array<RawPtr<OIS::ForceFeedback>> val );

        void setUserData( void *ptr1, void *ptr2 );
        void *getUserData( void *ptr ) const;

        void triggerEvent( SmartPtr<IInputEvent> inputEvent ) override;

        void queueEvent( SmartPtr<IInputEvent> event ) override;

        void addListener( SmartPtr<IEventListener> listener ) override;

        void removeListener( SmartPtr<IEventListener> listener ) override;

        void removeListeners() override;

        Vector3<real_Num> getMouseScroll() const;

        bool isShiftPressed() const override;

        void setShiftPressed( bool shiftPressed ) override;

        bool isLeftPressed() const override;

        void setLeftPressed( bool leftPressed ) override;

        bool isRightPressed() const override;

        void setRightPressed( bool rightPressed ) override;

        bool isMiddlePressed() const override;

        void setMiddlePressed( bool middlePressed ) override;

        f64 getLastClickTime() const override;

        void setLastClickTime( f64 lastClickTime ) override;

        f64 getDoubleClickInterval() const override;

        void setDoubleClickInterval( f64 doubleClickInterval ) override;

        f64 getLastInputTime() const override;

        void setLastInputTime( f64 lastInputTime ) override;

        f64 getInputTime() const override;

        void setInputTime( f64 inputTime ) override;

        void addInputTime( f64 inputTime ) override;

        bool isKeyPressed( KeyCodes keyCode ) const override;

        bool getCreateMouse() const override;
        void setCreateMouse( bool createMouse ) override;

        bool getCreateKeyboard() const override;
        void setCreateKeyboard( bool createKeyboard ) override;

        bool getCreateJoysticks() const override;
        void setCreateJoysticks( bool createJoysticks ) override;

        SmartPtr<render::IWindow> getWindow() const override;
        void setWindow( SmartPtr<render::IWindow> window ) override;

        void _getObject(void** ppObject);

        FB_CLASS_REGISTER_DECL;

    protected:
        class WindowListener : public CSharedObject<render::IWindowListener>
        {
        public:
            WindowListener() = default;
            ~WindowListener() override = default;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            void handleEvent( SmartPtr<render::IWindowEvent> event ) override;

            void setOwner( OISInputManager *owner );
            OISInputManager *getOwner() const;

        protected:
            OISInputManager *m_owner = nullptr;
        };

        // mouse events
        bool mouseMoved( const OIS::MouseEvent &arg );
        bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

        // keyboard events
        bool keyPressed( const OIS::KeyEvent &arg );
        bool keyReleased( const OIS::KeyEvent &arg );

        // joystick events
        bool povMoved( const OIS::JoyStickEvent &arg, int index );
        bool vector3Moved( const OIS::JoyStickEvent &arg, int index );
        bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
        bool sliderMoved( const OIS::JoyStickEvent &, int index );
        bool axisMoved( const OIS::JoyStickEvent &arg, int axis );
        bool buttonReleased( const OIS::JoyStickEvent &arg, int button );

        void setJoystickIdx( const String &gameInputDeviceName, s32 joystickIdx );
        s32 getJoystickIdx( const String &gameInputDeviceName ) const;

        Vector2F m_mousePosition = Vector2F::zero();
        Vector3<real_Num> m_mouseScroll = Vector3<real_Num>::zero();

        SharedPtr<KeyListener> m_keyListener;
        SharedPtr<MouseListener> m_mouseListener;
        SharedPtr<JoyStickListener> m_joyStickListener;

        ///
        RawPtr<OISKeyConverter> m_keyConverter;

        ///
        RawPtr<OIS::InputManager> m_inputManager;

        ///
        RawPtr<OIS::Mouse> m_mouse;

        ///
        RawPtr<OIS::Keyboard> m_keyboard;

        ///
        Array<RawPtr<OIS::JoyStick>> m_joySticks;

        ///
        Array<RawPtr<OIS::ForceFeedback>> m_forceFeedbackDevices;

        Array<SmartPtr<IEventListener>> m_listeners;

        using GameDeviceJoystickMap = std::map<String, s32>;
        GameDeviceJoystickMap m_gameDeviceJoystickMap;

        SmartPtr<render::IWindow> m_window;
        SmartPtr<render::IWindowListener> m_windowListener;

        u32 m_numSticks = 0;

        bool m_debugEnabled = false;

        using GameInputs = HashMap<hash32, SmartPtr<IGameInput>>;
        GameInputs m_gameInputs;

        using UserData = HashMap<u32, void *>;
        UserData m_userData;

        bool m_createMouse = true;
        bool m_createKeyboard = true;
        bool m_createJoysticks = true;

        SmartPtr<PlatformInputManager> m_platformInputManager;

        mutable RecursiveMutex m_mutex;
    };
}  // end namespace fb

#endif
