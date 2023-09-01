/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
#ifndef _SAMPLE_BASE_INPUT_EVENT_IDS_H
#define _SAMPLE_BASE_INPUT_EVENT_IDS_H

#include <SampleFrameworkInputEventIds.h>

// InputEvents used by SampleBase
enum SampleBaseInputEventIds
{
	SAMPLE_BASE_FIRST = NUM_SAMPLE_FRAMEWORK_INPUT_EVENT_IDS,

	PICKUP ,

	SPAWN_DEBUG_OBJECT ,

	PAUSE_SAMPLE ,
	STEP_ONE_FRAME ,
	TOGGLE_VISUALIZATION ,
	DECREASE_DEBUG_RENDER_SCALE ,
	INCREASE_DEBUG_RENDER_SCALE ,
	HIDE_GRAPHICS ,
	WIREFRAME ,	
	TOGGLE_PVD_CONNECTION ,
	SHOW_HELP ,
	SHOW_DESCRIPTION ,
	SHOW_EXTENDED_HELP ,
	VARIABLE_TIMESTEP,
	DELETE_PICKED,

	QUIT,
	MENU_VISUALIZATIONS,
	MENU_SAMPLES,

	MENU_ESCAPE,
	MENU_UP,
	MENU_DOWN,
	MENU_LEFT,
	MENU_RIGHT,
	MENU_SELECT,

	MENU_QUICK_UP,
	MENU_QUICK_DOWN,
	MENU_QUICK_LEFT,
	MENU_QUICK_RIGHT,

	TOGGLE_CPU_GPU,

	MOUSE_LOOK_BUTTON,

	CONSOLE_OPEN,
	CONSOLE_ESCAPE,
	CONSOLE_BACKSPACE,
	CONSOLE_ENTER,
	CONSOLE_SCROLL_UP,
	CONSOLE_SCROLL_DOWN,
	CONSOLE_LIST_COMMAND_UP,
	CONSOLE_LIST_COMMAND_DOWN,

	NEXT_PAGE,
	PREVIOUS_PAGE,

	RUN_NEXT_SAMPLE,
	RUN_PREVIOUS_SAMPLE,

	PROFILE_ONLY_PVD,

	NUM_SAMPLE_BASE_INPUT_EVENT_IDS, 
};

#endif
