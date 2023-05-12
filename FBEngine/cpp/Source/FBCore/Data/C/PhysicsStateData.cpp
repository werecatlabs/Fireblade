#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/PhysicsStateData.h"
#include "FBCore/Base/StringUtil.h"

namespace fb
{
    PhysicsStateData MakeSaracenModelData()
    {
        PhysicsStateData d;
        d.position = MakeSaracenVec();
        d.orientation = MakeSaracenVec( 0.0f, 0.0f, 0.0f, 1.0f );
        d.scale = MakeSaracenVec();

        d.linearVelocity = MakeSaracenVec();
        d.angularVelocity = MakeSaracenVec();

        d.isVisible = true;
        d.modelType = 0;
        d.modelState = 0;
        d.modelSubState = 0;
        d.isReady = 0;

        return d;
    }

    PhysicsStateData::PhysicsStateData()
    {
        position = MakeSaracenVec();
        orientation = MakeSaracenVec( 0.0f, 0.0f, 0.0f, 1.0f );
        scale = MakeSaracenVec();

        linearVelocity = MakeSaracenVec();
        angularVelocity = MakeSaracenVec();

        isVisible = true;
        modelType = 0;
        modelState = 0;
        modelSubState = 0;
        isReady = 0;
    }

    PhysicsStateData::PhysicsStateData( const PhysicsStateData &other )
    {
        *this = other;
    }

    PhysicsStateData &PhysicsStateData::operator=( const PhysicsStateData &other )
    {
        position = other.position;
        orientation = other.orientation;
        scale = other.scale;
        linearVelocity = other.linearVelocity;
        angularVelocity = other.angularVelocity;
        isVisible = other.isVisible;
        modelType = other.modelType;
        modelState = other.modelState;
        modelSubState = other.modelSubState;
        isReady = other.isReady;
        return *this;
    }
}  // namespace fb
