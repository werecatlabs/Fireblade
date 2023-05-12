#pragma once

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>

namespace fb
{
    class IAudioProcessData : public ISharedObject
    {
    public:
        ~IAudioProcessData() override = default;

        virtual s32 getProcessMode() const = 0;
        virtual void setProcessMode( s32 processMode ) = 0;

        virtual s32 getSymbolicSampleSize() const = 0;
        virtual void setSymbolicSampleSize( s32 symbolicSampleSize ) = 0;

        virtual s32 getNumSamples() const = 0;
        virtual void setNumSamples( s32 numSamples ) = 0;

        virtual s32 getNumInputs() const = 0;
        virtual void setNumInputs( s32 numInputs ) = 0;

        virtual s32 getNumOutputs() const = 0;
        virtual void setNumOutputs( s32 numOutputs ) = 0;

        virtual Array<SmartPtr<IAudioBusBuffers>> getInputs() const = 0;
        virtual Array<SmartPtr<IAudioBusBuffers>> getOutputs() const = 0;
    };
}  // end namespace fb
