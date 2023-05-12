#include <FBCore/FBCorePCH.h>
#include <FBCore/State/States/InputState.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{
    const u32 PlayerInputState::NUM_AXES = 128;

    PlayerInputState::PlayerInputState( RawPtr<InputState> parent ) : m_parent( parent )
    {
        m_state = &m_parent->m_state;
    }

    f32 PlayerInputState::getFunctions( s32 idx )
    {
        return m_state->playerInputState.functions[idx];
    }

    void PlayerInputState::setFunctions( s32 idx, s32 val )
    {
        m_state->playerInputState.functions[idx] = (f32)val;
    }

    f32 PlayerInputState::getAxes( s32 idx )
    {
        return m_state->playerInputState.axes[idx];
    }

    void PlayerInputState::setAxes( s32 idx, s32 val )
    {
        m_state->playerInputState.axes[idx] = (f32)val;
    }

    f32 PlayerInputState::getRawAxes( s32 idx )
    {
        return m_state->playerInputState.rawAxes[idx];
    }

    void PlayerInputState::setRawAxes( s32 idx, s32 val )
    {
        m_state->playerInputState.rawAxes[idx] = (f32)val;
    }

    f32 PlayerInputState::getPrevRawAxes( s32 idx )
    {
        return m_state->playerInputState.prevRawAxes[idx];
    }

    void PlayerInputState::setPrevRawAxes( s32 idx, s32 val )
    {
        m_state->playerInputState.prevRawAxes[idx] = (f32)val;
    }

    s32 PlayerInputState::getMap( s32 idx )
    {
        return m_state->playerInputState.channel[idx];
    }

    void PlayerInputState::setMap( s32 idx, s32 val )
    {
        m_state->playerInputState.channel[idx] = val;
    }

    s32 PlayerInputState::getChannel( s32 idx )
    {
        return m_state->playerInputState.channel[idx];
    }

    void PlayerInputState::setChannel( s32 idx, s32 val )
    {
        m_state->playerInputState.channel[idx] = val;
    }

    s32 PlayerInputState::getValues( s32 idx )
    {
        return m_state->playerInputState.values[idx];
    }

    void PlayerInputState::setValues( s32 idx, s32 val )
    {
        m_state->playerInputState.values[idx] = val;
    }

    s32 PlayerInputState::getButtons( s32 idx )
    {
        return m_state->playerInputState.buttons[idx];
    }

    void PlayerInputState::setButtons( s32 idx, s32 val )
    {
        m_state->playerInputState.buttons[idx] = val;
    }

    s32 PlayerInputState::getReverse( s32 idx )
    {
        return m_state->playerInputState.reverse[idx];
    }

    void PlayerInputState::setReverse( s32 idx, s32 val )
    {
        m_state->playerInputState.reverse[idx] = val;
    }

    s32 PlayerInputState::getAxisCount()
    {
        return m_state->playerInputState.axisCount;
    }

    void PlayerInputState::setAxisCount( s32 val )
    {
        m_state->playerInputState.axisCount = val;
    }

    s32 PlayerInputState::getButtonValues() const
    {
        return m_state->playerInputState.buttonValues;
    }

    void PlayerInputState::setButtonValues( s32 val )
    {
        m_state->playerInputState.buttonValues = val;
    }

    s32 PlayerInputState::getPrevButtonValues() const
    {
        return m_state->playerInputState.prevButtonValues;
    }

    void PlayerInputState::setPrevButtonValues( s32 val )
    {
        m_state->playerInputState.prevButtonValues = val;
    }

    s32 PlayerInputState::getNumButtons() const
    {
        return m_state->playerInputState.numButtons;
    }

    void PlayerInputState::setNumButtons( s32 val )
    {
        m_state->playerInputState.numButtons = val;
    }

    InputState::InputState()
    {
        //setPlayerInputState(new PlayerInputState(this));
    }

    SmartPtr<PlayerInputState> &InputState::getPlayerInputState()
    {
        return m_playerInputState;
    }

    const SmartPtr<PlayerInputState> &InputState::getPlayerInputState() const
    {
        return m_playerInputState;
    }

    void InputState::setPlayerInputState( SmartPtr<PlayerInputState> val )
    {
        m_playerInputState = val;
    }
}  // namespace fb
