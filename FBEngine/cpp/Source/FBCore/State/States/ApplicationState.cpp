#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/ApplicationState.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ApplicationState, BaseState );

    ApplicationState::ApplicationState()
    {
        constexpr int size = sizeof( ApplicationState );
    }

    ApplicationState::ApplicationState( const ApplicationState &other )
    {
        *this = other;
    }

    ApplicationState &ApplicationState::operator=( const ApplicationState &other )
    {
        cameraData = other.cameraData;
        modelData = other.modelData;
        stats = other.stats;
        playerInputState = other.playerInputState;
        switchIndicatorsData = other.switchIndicatorsData;

        appState = other.appState;
        isModelLoading = other.isModelLoading;
        isSceneLoading = other.isSceneLoading;
        isMouseOver = other.isMouseOver;

        reserved0 = other.reserved0;
        reserved1 = other.reserved1;
        reserved2 = other.reserved2;

        return *this;
    }

    StateData ApplicationState::toState() const
    {
        const ApplicationState &other = *this;
        StateData state;

        state.cameraData = other.cameraData;
        //state.modelData = other.modelData.;
        state.stats = other.stats;
        state.playerInputState = other.playerInputState;
        state.switchIndicatorsData = other.switchIndicatorsData;
        state.appState = other.appState;
        state.isModelLoading = other.isModelLoading;
        state.isSceneLoading = other.isSceneLoading;
        state.isMouseOver = other.isMouseOver;
        state.reserved0 = other.reserved0;
        state.reserved1 = other.reserved1;
        state.reserved2 = other.reserved2;

        return state;
    }
}  // namespace fb
