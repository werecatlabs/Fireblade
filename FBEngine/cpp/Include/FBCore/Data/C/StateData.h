#ifndef StateData_h__
#define StateData_h__

#include <FBCore/FBCorePrerequisites.h>

#include "FBCore/Math/Vector3.h"
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Data/C/ObjectTransformData.h"
#include "FBCore/Data/C/CameraData.h"
#include "FBCore/Data/C/PhysicsStateData.h"
#include <FBCore/Data/C/ComponentData.h>
#include "FBCore/Data/C/StatsData.h"
#include "FBCore/Data/C/PlayerInputStateData.h"
#include "FBCore/Data/C/SwitchIndicatorsData.h"

namespace fb
{
    class StateData
    {
    public:
        StateData();
        StateData( const StateData &other );

        StateData &operator=( const StateData &other );

        CameraData cameraData;
        PhysicsStateData modelData;
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
}  // end namespace fb

#endif  // StateData_h__
