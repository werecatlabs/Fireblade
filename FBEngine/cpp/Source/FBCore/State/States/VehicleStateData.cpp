#include <FBCore/FBCorePCH.h>
#include "FBCore/State/States/VehicleStateData.h"
#include "FBCore/Core/StringUtil.h"

namespace fb
{
    VehicleStateData::VehicleStateData( const VehicleStateData &other )
    {
        *this = other;
    }

    VehicleStateData &VehicleStateData::operator=( const VehicleStateData &other )
    {
        position = other.position;
        orientation = other.orientation;
        scale = other.scale;
        linearVelocity = other.linearVelocity;
        angularVelocity = other.angularVelocity;

        forcePosition = other.forcePosition;
        force = other.force;
        torque = other.torque;

        derivedLinearVelocity = other.derivedLinearVelocity;
        derivedAngularVelocity = other.derivedAngularVelocity;
        isVisible = other.isVisible;
        modelType = other.modelType;
        modelState = other.modelState;
        modelSubState = other.modelSubState;
        isReady = other.isReady;
        return *this;
    }

    /*
    SaracenModelData ModelStateData::toSaracenModelData() const
    {
        const ModelStateData &other = *this;
        SaracenModelData data;

        data.position = SaracenVec( other.position.X(), other.position.Y(), other.position.Z(), 0.0f );
        data.orientation = SaracenVec( other.orientation.X(), other.orientation.Y(),
                                       other.orientation.Z(), other.orientation.W() );
        data.scale = SaracenVec( other.scale.X(), other.scale.Y(), other.scale.Z(), 0.0f );
        data.linearVelocity = SaracenVec( other.linearVelocity.X(), other.linearVelocity.Y(),
                                          other.linearVelocity.Z(), 0.0f );
        data.angularVelocity = SaracenVec( other.angularVelocity.X(), other.angularVelocity.Y(),
                                           other.angularVelocity.Z(), 0.0f );

        data.force = SaracenVec( other.forcePosition.X(), other.forcePosition.Y(),
                                 other.forcePosition.Z(), 0.0f );
        data.force = SaracenVec( other.force.X(), other.force.Y(), other.force.Z(), 0.0f );
        data.torque = SaracenVec( other.torque.X(), other.torque.Y(), other.torque.Z(), 0.0f );

        data.isVisible = other.isVisible;
        data.modelType = other.modelType;
        data.modelState = other.modelState;
        data.modelSubState = other.modelSubState;
        data.isReady = other.isReady;

        return data;
    }
    */
}  // namespace fb
