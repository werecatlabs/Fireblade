#pragma once 
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Sound/IAudioProcessData.h>



namespace fb
{

	class CAudioProcessData : public IAudioProcessData
	{
	public:
		CAudioProcessData();
		~CAudioProcessData();

	};

}