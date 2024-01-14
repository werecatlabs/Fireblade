#pragma once
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Sound/IAudioEffectVolume.h>


namespace fb
{
	class CAudioEffectVolume : public IAudioEffectVolume
	{
	public:
		CAudioEffectVolume() = default;
		~CAudioEffectVolume() = default;

		void process() override;

		f32* getInput() override;
		void setInput(f32* input) override;

		f32* getOutput() override;
		void setOutput(f32* output) override;

		u32 getNumSamples() const override;
		void setNumSamples(u32 numSamples) override;

		bool getBypass() const override;
		void setBypass(bool bypass) override;

		f32 getVolume() const override;
		void setVolume(f32 volume) override;

		u32 getSampleRate() const override;
		void setSampleRate(u32 sampleRate) override;

	protected:
		f32* m_input = nullptr;
		f32* m_output = nullptr;
		u32 m_numSamples = 0;
		u32 m_sampleRate = 0;
		f32 m_volume = 0.0f;
		bool m_bypass = false;
	};
}
