/*
The zlib/libpng License

Copyright (c) 2018 Arthur Brainville
Copyright (c) 2015 Andrew Fenn
Copyright (c) 2005-2010 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no
event will the authors be held liable for any damages arising from the use of this
software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to the
following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that
        you wrote the original software. If you use this software in a product,
        an acknowledgment in the product documentation would be appreciated
        but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.   
*/
#include "win32/Win32Mouse.h"
#include "win32/Win32InputManager.h"
#include "OISException.h"
#include "OISEvents.h"

using namespace OIS;

//--------//
Win32Mouse::Win32Mouse(InputManager* creator, IDirectInput8* pDI, bool buffered, DWORD coopSettings) :
 Mouse(creator->inputSystemName(), buffered, 0, creator)
{
	mMouse		 = nullptr;
	mDirectInput = pDI;
	coopSetting	 = coopSettings;
	mHwnd		 = nullptr;

	static_cast<Win32InputManager*>(mCreator)->_setMouseUsed(true);
}

//--------//
void Win32Mouse::_initialize()
{
	DIPROPDWORD dipdw;

	//Clear old state
	mState.clear();

	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= MOUSE_DX_BUFFERSIZE;

	if(FAILED(mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, NULL)))
		OIS_EXCEPT(E_General, "Win32Mouse::Win32Mouse >> Failed to create device");

	if(FAILED(mMouse->SetDataFormat(&c_dfDIMouse2)))
		OIS_EXCEPT(E_General, "Win32Mouse::Win32Mouse >> Failed to set format");

	mHwnd = static_cast<Win32InputManager*>(mCreator)->getWindowHandle();

	if(FAILED(mMouse->SetCooperativeLevel(mHwnd, coopSetting)))
		OIS_EXCEPT(E_General, "Win32Mouse::Win32Mouse >> Failed to set coop level");

	if(FAILED(mMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		OIS_EXCEPT(E_General, "Win32Mouse::Win32Mouse >> Failed to set property");

	const HRESULT hr = mMouse->Acquire();
	if(FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
		OIS_EXCEPT(E_General, "Win32Mouse::Win32Mouse >> Failed to aquire mouse!");

	mMouse->SetDataFormat(&c_dfDIMouse2);
}

//--------//
Win32Mouse::~Win32Mouse()
{
	if(mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = nullptr;
	}

	static_cast<Win32InputManager*>(mCreator)->_setMouseUsed(false);
}

//--------//
void Win32Mouse::capture()
{
	//Clear old relative values
	mState.X.rel = mState.Y.rel = mState.Z.rel = 0;

	DWORD entries = MOUSE_DX_BUFFERSIZE;
	DIMOUSESTATE2 mouseState;

	HRESULT hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	if(hr != DI_OK)
	{
		hr = mMouse->Acquire();
		while(hr == DIERR_INPUTLOST)
			hr = mMouse->Acquire();

		hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);

		//Perhaps the user just tabbed away, and coop settings
		//are nonexclusive..so just ignore
		if(FAILED(hr))
			return;
	}

	mState.X.rel = mouseState.lX;
	mState.Y.rel = mouseState.lY;
	mState.Z.rel = mouseState.lZ;
	for(unsigned int i = 0; i < 8; i++)
		if(!_doMouseClick(i, mouseState.rgbButtons[i])) return;

	bool axesMoved = mouseState.lX != 0 || mouseState.lY != 0 || mouseState.lZ != 0;

	if(axesMoved)
	{
		if(coopSetting & DISCL_NONEXCLUSIVE)
		{
			//DirectInput provides us with meaningless values, so correct that
			POINT point;
			GetCursorPos(&point);
			ScreenToClient(mHwnd, &point);
			mState.X.abs = point.x;
			mState.Y.abs = point.y;
		}
		else
		{
			mState.X.abs += mState.X.rel;
			mState.Y.abs += mState.Y.rel;
		}
		mState.Z.abs += mState.Z.rel;

		//Clip values to window
		if(mState.X.abs < 0)
			mState.X.abs = 0;
		else if(mState.X.abs > mState.width)
			mState.X.abs = mState.width;
		if(mState.Y.abs < 0)
			mState.Y.abs = 0;
		else if(mState.Y.abs > mState.height)
			mState.Y.abs = mState.height;

		//Do the move
		if(mListener && mBuffered)
			mListener->mouseMoved(MouseEvent(this, mState));
	}
}

//--------//
bool Win32Mouse::_doMouseClick(int mouseButton, unsigned char di)
{
	if(di & 0x80 && !mState.buttonDown(MouseButtonID(mouseButton)))
	{
		mState.buttons |= 1 << mouseButton; //turn the bit flag on
		if(mListener && mBuffered)
			return mListener->mousePressed(MouseEvent(this, mState), MouseButtonID(mouseButton));
	}
	else if(!(di & 0x80) && mState.buttonDown(MouseButtonID(mouseButton)))
	{
		mState.buttons &= ~(1 << mouseButton); //turn the bit flag off
		if(mListener && mBuffered)
			return mListener->mouseReleased(MouseEvent(this, mState), MouseButtonID(mouseButton));
	}

	return true;
}

//--------//
void Win32Mouse::setBuffered(bool buffered)
{
	mBuffered = buffered;
}
