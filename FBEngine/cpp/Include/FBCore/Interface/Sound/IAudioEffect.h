#pragma once

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class IAudioEffect : public ISharedObject
    {
    public:
        ~IAudioEffect() override = default;

        virtual void process() = 0;

        virtual f32 *getInput() = 0;
        virtual void setInput( f32 *input ) = 0;

        virtual f32 *getOutput() = 0;
        virtual void setOutput( f32 *output ) = 0;

        virtual u32 getNumSamples() const = 0;
        virtual void setNumSamples( u32 numSamples ) = 0;

        virtual bool getBypass() const = 0;
        virtual void setBypass( bool bypass ) = 0;

        virtual u32 getSampleRate() const = 0;
        virtual void setSampleRate( u32 sampleRate ) = 0;
    };
}  // end namespace fb
