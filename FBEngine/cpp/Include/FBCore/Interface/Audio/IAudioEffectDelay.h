#pragma once

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Audio/IAudioEffect.h>

namespace fb
{
    class IAudioEffectDelay : public IAudioEffect
    {
    public:
        ~IAudioEffectDelay() override = default;

        virtual u32 getNumChannels() const = 0;
        virtual void setNumChannels( u32 numChannels ) = 0;
    };
}  // end namespace fb
