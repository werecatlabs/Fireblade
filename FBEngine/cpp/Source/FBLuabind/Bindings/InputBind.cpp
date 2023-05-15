#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/InputBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "FBLuabind/FBLuabindTypes.h"

namespace fb
{

    //IGameInput *_addGameInput( IInputManager *mgr, lua_Integer id )
    //{
    //    //GameInputPtr gameInput = mgr->addGameInput( id );
    //    //return gameInput.get();
    //}

    void _setKeyboardAction( IGameInput *gameInput, hash32 id, const char *key0, const char *key1 )
    {
        gameInput->getGameInputMap()->setKeyboardAction( id, key0, key1 );
    }

    bool _isPressedDown( const IKeyboardState *keyboardState )
    {
        return keyboardState->isPressedDown();
    }

    lua_Integer _getJoystickEventType( const IJoystickState *joystickState )
    {
        return joystickState->getEventType();
    }

    lua_Integer _getGameEventType( const IGameInputState *gameInputEvent )
    {
        return gameInputEvent->getEventType();
    }

    lua_Integer _getAction( const IGameInputState *gameInputEvent )
    {
        return gameInputEvent->getAction();
    }

    //GameInputMapPtr _getGameInputMap( IGameInput *input )
    //{
    //    return input->getGameInputMap();
    //}

    void _setKeyboardActionMap( IGameInputMap *map, lua_Integer id, const String &key0,
                                const String &key1 )
    {
        map->setKeyboardAction( (u32)id, key0, key1 );
    }

    void _getKeyboardAction( IGameInputMap *map, lua_Integer id, String &key0, String &key1 )
    {
        map->getKeyboardAction( (u32)id, key0, key1 );
    }

    void _setJoystickAction( IGameInputMap *map, lua_Integer id, lua_Integer button )
    {
        map->setJoystickAction( *reinterpret_cast<hash32 *>( &id ),
                                *reinterpret_cast<hash32 *>( &button ), IGameInput::UNASSIGNED );
    }

    void _setJoystickAction2( IGameInputMap *map, lua_Integer id, lua_Integer button0,
                              lua_Integer button1 )
    {
        map->setJoystickAction( *reinterpret_cast<hash32 *>( &id ),
                                *reinterpret_cast<hash32 *>( &button0 ),
                                *reinterpret_cast<hash32 *>( &button1 ) );
    }

    void _getJoystickAction( IGameInputMap *map, lua_Integer id, lua_Integer &button0,
                             lua_Integer &button1 )
    {
    }

    hash32 _getActionFromButton( IGameInputMap *map, lua_Integer button )
    {
        return 0;
    }

    hash32 _getActionFromKey( IGameInputMap *map, lua_Integer key )
    {
        return 0;
    }

    //MouseStatePtr _getMouseState( IInputEvent *event )
    //{
    //    return event->getMouseState();
    //}

    //KeyboardStatePtr _getKeyboardState( IInputEvent *event )
    //{
    //    return event->getKeyboardState();
    //}

    //JoystickStatePtr _getJoystickState( IInputEvent *event )
    //{
    //    return event->getJoystickState();
    //}

    //GameInputStatePtr _getGameInputState( IInputEvent *event )
    //{
    //    return event->getGameInputState();
    //}

    void bindInput( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IInputDeviceManager, ISharedObject, SmartPtr<IInputDeviceManager>>(
                        "IInputDeviceManager" )
                        .def( "addGameInput", &IInputDeviceManager::addGameInput )
                        .def( "findGameInput", &IInputDeviceManager::findGameInput )];

        //module( L )[class_<IGameInputMap, IScriptObject, GameInputMapPtr>( "IGameInputMap" )
        //                .def( "setKeyboardAction", _setKeyboardActionMap )
        //                .def( "getKeyboardAction", _getKeyboardAction )

        //                .def( "setJoystickAction", _setJoystickAction )
        //                .def( "getJoystickAction", _getJoystickAction )

        //                .def( "getActionFromButton", _getActionFromButton )
        //                .def( "getActionFromKey", _getActionFromKey )];

        //module( L )[class_<IGameInput, IScriptObject>( "IGameInput" )
        //                .def( "setId", &IGameInput::setId )
        //                .def( "getId", &IGameInput::getId )
        //                .def( "setKeyboardAction", _setKeyboardAction )

        //                .def( "getPlayerIndex", &IGameInput::getPlayerIndex )
        //                .def( "setPlayerIndex", &IGameInput::setPlayerIndex )

        //                .def( "getJoystickId", &IGameInput::getJoystickId )
        //                .def( "setJoystickId", &IGameInput::setJoystickId )
        //                .def( "getGameInputMap", _getGameInputMap )];

        //module( L )[class_<IMouseState>( "IMouseState" )
        //                .def( "getRelativePosition", &IMouseState::getRelativePosition )
        //                .def( "setRelativePosition", &IMouseState::setRelativePosition )

        //                .def( "getAbsolutePosition", &IMouseState::getAbsolutePosition )
        //                .def( "setAbsolutePosition", &IMouseState::setAbsolutePosition )

        //                .def( "getWheelPosition", &IMouseState::getWheelPosition )
        //                .def( "setWheelPosition", &IMouseState::setWheelPosition )

        //                .def( "isButtonPressed", &IMouseState::isButtonPressed )
        //                .def( "setButtonPressed", &IMouseState::setButtonPressed )

        //                .def( "getEventType", &IMouseState::getEventType )
        //                .def( "setEventType", &IMouseState::setEventType )];

        //module( L )[class_<IKeyboardState>( "IKeyboardState" ).def( "isPressedDown", _isPressedDown )];

        //module( L )[class_<IJoystickState>( "IJoystickState" )
        //                .def( "getEventType", _getJoystickEventType )

        //                .def( "getJoystick", &IJoystickState::getJoystick )
        //                .def( "setJoystick", &IJoystickState::setJoystick )

        //                .def( "getPOV", &IJoystickState::getPOV )
        //                .def( "setPOV", &IJoystickState::setPOV )];

        //module( L )[class_<IGameInputState>( "IGameInputState" )
        //                .def( "getEventType", _getGameEventType )
        //                .def( "getAction", _getAction )];

        //module( L )[class_<IInputEvent, IScriptObject>( "IInputEvent" )
        //                .def( "getMouseState", _getMouseState )
        //                .def( "getKeyboardState", _getKeyboardState )
        //                .def( "getJoystickState", _getJoystickState )
        //                .def( "getGameInputState", _getGameInputState )

        //                .def( "getGameInputId", &IInputEvent::getGameInputId )
        //                .def( "setGameInputId", &IInputEvent::setGameInputId )
        //                .def( "getEventType", &IInputEvent::getEventType )
        //                .def( "setEventType", &IInputEvent::setEventType )];

        //LUA_CONST_START( IInputEvent )
        //LUA_CONST( IInputEvent, MOUSE_INPUT_EVENT );
        //LUA_CONST( IInputEvent, KEY_INPUT_EVENT );
        //LUA_CONST( IInputEvent, JOYSTICK_INPUT_EVENT );
        //LUA_CONST( IInputEvent, USER_INPUT_EVENT );
        //LUA_CONST_END;

        //LUA_CONST_START( IJoystickState )
        //LUA_CONST( IJoystickState, EVT_TYPE_POV_MOVED );
        //LUA_CONST( IJoystickState, EVT_TYPE_VECTOR3_MOVED );
        //LUA_CONST( IJoystickState, EVT_TYPE_BUTTON_PRESSED );
        //LUA_CONST( IJoystickState, EVT_TYPE_BUTTON_RELEASED );
        //LUA_CONST( IJoystickState, EVT_TYPE_SLIDER_MOVED );
        //LUA_CONST( IJoystickState, EVT_TYPE_AXIS_MOVED );

        //LUA_CONST( IJoystickState, POV_Centered );
        //LUA_CONST( IJoystickState, POV_North );
        //LUA_CONST( IJoystickState, POV_South );
        //LUA_CONST( IJoystickState, POV_East );
        //LUA_CONST( IJoystickState, POV_West );
        //LUA_CONST( IJoystickState, POV_NorthEast );
        //LUA_CONST( IJoystickState, POV_SouthEast );
        //LUA_CONST( IJoystickState, POV_NorthWest );
        //LUA_CONST( IJoystickState, POV_SouthWest );
        //LUA_CONST_END;
    }

}  // end namespace fb
