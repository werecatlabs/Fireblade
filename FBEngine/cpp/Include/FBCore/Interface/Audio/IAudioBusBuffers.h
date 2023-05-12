#pragma once

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class IAudioBusBuffers : public ISharedObject
    {
    public:
        ~IAudioBusBuffers() override = default;

        virtual s32 getNumChannels() const = 0;
        virtual void setNumChannels( s32 numChannels ) = 0;

        virtual u64 getSilenceFlags() const = 0;
        virtual void setSilenceFlags( u64 flags ) = 0;

        virtual f32 **getChannelBuffers32() const = 0;
        virtual void setChannelBuffers32( f32 **value ) = 0;

        virtual f64 **getChannelBuffers64() const = 0;
        virtual void setChannelBuffers64( f64 **value ) = 0;
    };
}  // end namespace fb
