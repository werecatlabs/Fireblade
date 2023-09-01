/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

#ifndef LINUX_SAMPLE_USER_INPUT_IDS_H
#define LINUX_SAMPLE_USER_INPUT_IDS_H

namespace SampleFramework
{
	enum LinuxSampleUserInputIds
	{
		LINUXKEY_UNKNOWN = 0,

		// the following keys are keyboard layout dependent
		// its not a complete list
		LINUXKEY_A,
		LINUXKEY_B,
		LINUXKEY_C,
		LINUXKEY_D,
		LINUXKEY_E,
		LINUXKEY_F,
		LINUXKEY_G,
		LINUXKEY_H,
		LINUXKEY_I,
		LINUXKEY_J,
		LINUXKEY_K,
		LINUXKEY_L,
		LINUXKEY_M,
		LINUXKEY_N,
		LINUXKEY_O,
		LINUXKEY_P,
		LINUXKEY_Q,
		LINUXKEY_R,
		LINUXKEY_S,
		LINUXKEY_T,
		LINUXKEY_U,
		LINUXKEY_V,
		LINUXKEY_W,
		LINUXKEY_X,
		LINUXKEY_Y,
		LINUXKEY_Z,

		LINUXKEY_0,
		LINUXKEY_1,
		LINUXKEY_2,
		LINUXKEY_3,
		LINUXKEY_4,
		LINUXKEY_5,
		LINUXKEY_6,
		LINUXKEY_7,
		LINUXKEY_8,
		LINUXKEY_9,
		
		LINUXKEY_NUMPAD0,
		LINUXKEY_NUMPAD1,
		LINUXKEY_NUMPAD2,
		LINUXKEY_NUMPAD3,
		LINUXKEY_NUMPAD4,
		LINUXKEY_NUMPAD5,
		LINUXKEY_NUMPAD6,
		LINUXKEY_NUMPAD7,
		LINUXKEY_NUMPAD8,
		LINUXKEY_NUMPAD9,
		
		LINUXKEY_SHIFT,
		LINUXKEY_CONTROL,
		LINUXKEY_SPACE,
		LINUXKEY_RETURN,
		LINUXKEY_ESCAPE,
		LINUXKEY_COMMA,
		LINUXKEY_DIVIDE,
		LINUXKEY_SUBTRACT,
		LINUXKEY_ADD,
		
		LINUXKEY_F1,
		LINUXKEY_F2,
		LINUXKEY_F3,
		LINUXKEY_F4,
		LINUXKEY_F5,
		LINUXKEY_F6,
		LINUXKEY_F7,
		LINUXKEY_F8,
		LINUXKEY_F9,
		LINUXKEY_F10,
		LINUXKEY_F11,
		LINUXKEY_F12,
		
		LINUXKEY_TAB,
		LINUXKEY_BACKSPACE,
		LINUXKEY_PRIOR,
		LINUXKEY_NEXT,
		LINUXKEY_UP,
		LINUXKEY_DOWN,
		LINUXKEY_LEFT,
		LINUXKEY_RIGHT,
		
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_CENTER,

		MOUSE_MOVE,

		SCAN_CODE_UP ,
		SCAN_CODE_DOWN ,
		SCAN_CODE_LEFT,
		SCAN_CODE_RIGHT,
		SCAN_CODE_FORWARD,
		SCAN_CODE_BACKWARD,
		SCAN_CODE_L,
		SCAN_CODE_9,
		SCAN_CODE_0,

		NUM_KEY_CODES,
	};
	
}

#endif
