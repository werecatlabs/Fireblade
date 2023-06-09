#include "OISConfig.h"
#ifdef OIS_WIN32_WIIMOTE_SUPPORT
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
#include "OISWiiMoteForceFeedback.h"
#include "OISEffect.h"

using namespace OIS;

//--------//
WiiMoteForceFeedback::WiiMoteForceFeedback(cWiiMote& wiiMote) :
 mWiiMote(wiiMote), mHandle(-1)
{
	//One and only supported effect
	_addEffectTypes(OIS::Effect::ConstantForce, OIS::Effect::Constant);
}

//--------//
WiiMoteForceFeedback::~WiiMoteForceFeedback()
{
	mWiiMote.SetVibration(false);
}

//--------//
void WiiMoteForceFeedback::upload(const Effect* effect)
{
	if(effect)
	{
		//Multiple effects are useless, just return
		if(mHandle != -1 || effect->_handle != -1) return;

		//Ok, so we are uploading a fresh effect
		effect->_handle = mHandle = 1;

		mWiiMote.SetVibration(true);
	}
}

//--------//
void WiiMoteForceFeedback::modify(const Effect* effect)
{
	//Nothing to modify
}

//--------//
void WiiMoteForceFeedback::remove(const Effect* effect)
{
	//We have no effects uploaded, so just return
	if(mHandle == -1 || effect == 0) return;

	if(mHandle == effect->_handle)
	{
		mWiiMote.SetVibration(false);
		mHandle = effect->_handle = -1;
	}
}
#endif
