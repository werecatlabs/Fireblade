#pragma once 
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Sound/IAudioBusBuffers.h>



namespace fb
{

	class CAudioBusBuffers : public IAudioBusBuffers
	{
	public:
		CAudioBusBuffers();
		~CAudioBusBuffers();

	};

}