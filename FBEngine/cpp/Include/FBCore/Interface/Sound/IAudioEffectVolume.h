#pragma once

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Sound/IAudioEffect.h>

namespace fb
{
    class IAudioEffectVolume : public IAudioEffect
    {
    public:
        ~IAudioEffectVolume() override = default;

        virtual f32 getVolume() const = 0;
        virtual void setVolume( f32 volume ) = 0;
    };
}  // end namespace fb
