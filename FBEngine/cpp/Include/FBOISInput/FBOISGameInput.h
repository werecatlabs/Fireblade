#ifndef FBOISGameInput_h__
#define FBOISGameInput_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IGameInput.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class OISGameInput : public IGameInput
    {
    public:
        OISGameInput( OISInputManager *inputManager );
        ~OISGameInput() override;

        void update() override;

        hash32 getId() const override;
        void setId( hash32 id ) override;

        SmartPtr<IGameInputMap> getGameInputMap() const override;

        u32 getPlayerIndex() const override;
        void setPlayerIndex( u32 val ) override;

        u32 getJoystickId() const override;
        void setJoystickId( u32 joystickId ) override;

        void *getUserData() const override;
        void setUserData( void *val ) override;

        bool isAssigned() const override;

        void setKeyboardAction( u32 id, const String &key0, const String &key1 );
        void setJoystickAction( u32 id, u32 button0, u32 button1 );

        OIS::JoyStick *getJoyStick() const;
        void setJoyStick( OIS::JoyStick *val );

        void setPOVDirection( u32 pov );
        u32 getPOVDirection() const;

        void setPollHardware( bool poll );
        bool getPollHardware() const;

        void setHandleEvents( bool handleEvents );
        bool getHandleEvents() const;

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<IGameInputMap> m_inputMap;
        RawPtr<OISInputManager> m_inputManager;
        RawPtr<OIS::JoyStick> m_joyStick;
        void *m_userData = nullptr;

        hash32 m_id;
        u32 m_pov;
        u32 m_playerIndex;
        u32 m_joystickId;
        bool m_poll;
        bool m_handleEvents;
    };

    using OISGameInputPtr = SmartPtr<OISGameInput>;
}  // end namespace fb

#endif  // FBOISGameInput_h__
