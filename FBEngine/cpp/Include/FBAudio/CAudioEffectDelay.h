#pragma once 
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Sound/IAudioEffectDelay.h>



namespace fb
{

	class CAudioEffectDelay : public IAudioEffectDelay
	{
	public:
		CAudioEffectDelay();
		~CAudioEffectDelay();
		
		void process() override;

		f32* getInput() override;
		void setInput(f32* input) override;

		f32* getOutput() override;
		void setOutput(f32* output) override;

		u32 getNumSamples() const override;
		void setNumSamples(u32 numSamples) override;

		bool getBypass() const override;
		void setBypass(bool bypass) override;

		u32 getNumChannels() const override;
		void setNumChannels(u32 numChannels) override;

		u32 getSampleRate() const override;
		void setSampleRate(u32 sampleRate) override;

	private:
		f32* m_input = nullptr;
		f32* m_output = nullptr;
		u32 m_numSamples = 0;
		u32 m_numChannels = 0;
		f32 m_delay = 0.0f;
		f32** m_buffer = nullptr;
		s32 m_bufferPosition = 0;
		u32 m_sampleRate = 0;
		bool m_bBypass = false;
	};

}