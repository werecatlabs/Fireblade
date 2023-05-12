#ifndef ApplicationState_h__
#define ApplicationState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Data/C/ObjectTransformData.h>
#include <FBCore/Data/C/CameraData.h>
#include <FBCore/Data/C/PhysicsStateData.h>
#include <FBCore/Data/C/ComponentData.h>
#include <FBCore/Data/C/StatsData.h>
#include <FBCore/Data/C/StateData.h>
#include <FBCore/Data/C/PlayerInputStateData.h>
#include <FBCore/Data/C/SwitchIndicatorsData.h>
#include <FBCore/State/States/VehicleStateData.h>

namespace fb
{
    class ApplicationState : public BaseState
    {
    public:
        ApplicationState();
        ApplicationState( const ApplicationState &other );

        ApplicationState &operator=( const ApplicationState &other );

        StateData toState() const;

        FB_CLASS_REGISTER_DECL;

        CameraData cameraData;
        VehicleStateData modelData;
        StatsData stats;
        PlayerInputStateData playerInputState;
        SwitchIndicatorsData switchIndicatorsData;

        int appState = 0;
        int isModelLoading = 0;
        int isSceneLoading = 0;
        int isMouseOver = 0;

        float reserved0 = 0.0f;
        float reserved1 = 0.0f;
        float reserved2 = 0.0f;
    };
}  // namespace fb

#endif  // ApplicationState_h__
