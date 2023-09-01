/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

#ifndef WINDOWS_SAMPLE_USER_INPUT_IDS_H
#define WINDOWS_SAMPLE_USER_INPUT_IDS_H


namespace SampleFramework
{
	enum WindowsSampleUserInputIds
	{
		WKEY_UNKNOWN = 0,

		WKEY_DEFINITION_START,

		WKEY_A,
		WKEY_B,
		WKEY_C,
		WKEY_D,
		WKEY_E,
		WKEY_F,
		WKEY_G,
		WKEY_H,
		WKEY_I,
		WKEY_J,
		WKEY_K,
		WKEY_L,
		WKEY_M,
		WKEY_N,
		WKEY_O,
		WKEY_P,
		WKEY_Q,
		WKEY_R,
		WKEY_S,
		WKEY_T,
		WKEY_U,
		WKEY_V,
		WKEY_W,
		WKEY_X,
		WKEY_Y,
		WKEY_Z,

		WKEY_0,
		WKEY_1,
		WKEY_2,
		WKEY_3,
		WKEY_4,
		WKEY_5,
		WKEY_6, 
		WKEY_7,
		WKEY_8,
		WKEY_9,

		WKEY_SPACE,
		WKEY_RETURN,
		WKEY_SHIFT,
		WKEY_CONTROL,
		WKEY_ESCAPE,
		WKEY_COMMA, 
		WKEY_NUMPAD0,
		WKEY_NUMPAD1,
		WKEY_NUMPAD2,
		WKEY_NUMPAD3,
		WKEY_NUMPAD4,
		WKEY_NUMPAD5,
		WKEY_NUMPAD6,
		WKEY_NUMPAD7,
		WKEY_NUMPAD8,
		WKEY_NUMPAD9,
		WKEY_MULTIPLY,
		WKEY_ADD,
		WKEY_SEPARATOR,
		WKEY_SUBTRACT,
		WKEY_DECIMAL,
		WKEY_DIVIDE,

		WKEY_F1,
		WKEY_F2,
		WKEY_F3,
		WKEY_F4,
		WKEY_F5,
		WKEY_F6,
		WKEY_F7,
		WKEY_F8,
		WKEY_F9,
		WKEY_F10,
		WKEY_F11,
		WKEY_F12,

		WKEY_TAB,
		WKEY_BACKSPACE,
		WKEY_PRIOR,
		WKEY_NEXT,
		WKEY_UP,
		WKEY_DOWN,
		WKEY_LEFT,
		WKEY_RIGHT,

		SCAN_CODE_UP ,
		SCAN_CODE_DOWN ,
		SCAN_CODE_LEFT,
		SCAN_CODE_RIGHT,
		SCAN_CODE_FORWARD,
		SCAN_CODE_BACKWARD,
		SCAN_CODE_LEFT_SHIFT,
		SCAN_CODE_SPACE,
		SCAN_CODE_L,
		SCAN_CODE_9,
		SCAN_CODE_0,

		WKEY_DEFINITION_END,

		MOUSE_DEFINITION_START,

		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_CENTER,

		MOUSE_MOVE,

		MOUSE_DEFINITION_END,

		GAMEPAD_DEFINITION_START,

		GAMEPAD_DIGI_UP,
		GAMEPAD_DIGI_DOWN,
		GAMEPAD_DIGI_LEFT,
		GAMEPAD_DIGI_RIGHT,
		GAMEPAD_START,
		GAMEPAD_SELECT,
		GAMEPAD_LEFT_STICK,
		GAMEPAD_RIGHT_STICK,
		GAMEPAD_NORTH,
		GAMEPAD_SOUTH,
		GAMEPAD_WEST,
		GAMEPAD_EAST,
		GAMEPAD_LEFT_SHOULDER_TOP,
		GAMEPAD_RIGHT_SHOULDER_TOP,
		GAMEPAD_LEFT_SHOULDER_BOT,
		GAMEPAD_RIGHT_SHOULDER_BOT,

		GAMEPAD_RIGHT_STICK_X, 
		GAMEPAD_RIGHT_STICK_Y,
		GAMEPAD_LEFT_STICK_X , 
		GAMEPAD_LEFT_STICK_Y ,

		GAMEPAD_DEFINITION_END,

		NUM_KEY_CODES,
	};
	
}

#endif
