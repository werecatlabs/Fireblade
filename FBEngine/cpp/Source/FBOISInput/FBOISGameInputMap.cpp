#include <FBOISInput/FBOISGameInputMap.h>
#include <FBOISInput/FBOISKeyConverter.h>
#include <FBOISInput/FBGameInputState.h>
#include <FBOISInput/FBJoystickState.h>
#include <FBOISInput/FBKeyboardState.h>
#include <FBOISInput/FBMouseState.h>
#include <FBOISInput/FBInputEvent.h>
#include <FBOISInput/FBInputActionData.h>
#include <FBCore/FBCore.h>

namespace fb
{
    OISGameInputMap::OISGameInputMap()
    {
    }

    OISGameInputMap::~OISGameInputMap()
    {
    }

    void OISGameInputMap::setKeyboardAction( u32 id, const String &key0, const String &key1 )
    {
        if( auto converter = OISKeyConverter::getSingletonPtr() )
        {
            auto keyCode0 = converter->getCodeFromString( key0 );
            auto keyCode1 = converter->getCodeFromString( key1 );

            auto action = fb::make_ptr<InputActionData>( keyCode0, keyCode1, id );
            setKeyboardAction( id, action );
        }
    }

    void OISGameInputMap::setJoystickAction( u32 id, u32 button0, u32 button1 )
    {
        setJoystickAction( id,
                           SmartPtr<IInputActionData>( new InputActionData( button0, button1, id ) ) );
    }

    void OISGameInputMap::setKeyboardAction( u32 id, const SmartPtr<IInputActionData> &actionData )
    {
        m_keyboardMap[id] = actionData;
    }

    void OISGameInputMap::setJoystickAction( u32 id, const SmartPtr<IInputActionData> &actionData )
    {
        m_joystickMap[id] = actionData;
    }

    u32 OISGameInputMap::getActionFromKey( u32 key ) const
    {
        const ActionKeyMap &keyboardMap = getKeyboardMap();
        auto keyActionIt = keyboardMap.begin();
        for( ; keyActionIt != keyboardMap.end(); ++keyActionIt )
        {
            const SmartPtr<IInputActionData> &actionData = keyActionIt->second;
            if( actionData->getPrimaryAction() == key || actionData->getSecondaryAction() == key )
            {
                return actionData->getActionId();
            }
        }

        return IGameInput::UNASSIGNED;
    }

    void OISGameInputMap::getKeyboardAction( u32 id, String &key0, String &key1 )
    {
    }

    void OISGameInputMap::getJoystickAction( u32 id, u32 &button0, u32 &button1 )
    {
    }

    u32 OISGameInputMap::getActionFromButton( u32 button ) const
    {
        const ActionKeyMap &joystickMap = getJoystickMap();
        auto joyIt = joystickMap.begin();
        for( ; joyIt != joystickMap.end(); ++joyIt )
        {
            const SmartPtr<IInputActionData> &actionData = joyIt->second;
            if( actionData->getPrimaryAction() == button || actionData->getSecondaryAction() == button )
            {
                return actionData->getActionId();
            }
        }

        return IGameInput::UNASSIGNED;
    }

    const ActionKeyMap &OISGameInputMap::getKeyboardMap() const
    {
        return m_keyboardMap;
    }

    const ActionKeyMap &OISGameInputMap::getJoystickMap() const
    {
        return m_joystickMap;
    }

    bool OISGameInputMap::getInputActionData( u32 button, SmartPtr<IInputActionData> &data )
    {
        const ActionKeyMap &joystickMap = getJoystickMap();
        auto joyIt = joystickMap.begin();
        for( ; joyIt != joystickMap.end(); ++joyIt )
        {
            const SmartPtr<IInputActionData> &actionData = joyIt->second;

            bool buttonState0 = false;
            bool buttonState1 = false;

            if( actionData->getPrimaryAction() == button || actionData->getSecondaryAction() == button )
            {
                data = actionData;
                return true;
            }
        }

        return false;
    }
}  // end namespace fb
