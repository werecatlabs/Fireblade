#ifndef InputState_h__
#define InputState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Data/C/InputStateData.h>

namespace fb
{

    class PlayerInputState : public BaseState
    {
    public:
        static const u32 NUM_AXES;

        PlayerInputState( RawPtr<InputState> parent );

        f32 getFunctions( s32 idx );

        void setFunctions( s32 idx, s32 val );

        f32 getAxes( s32 idx );

        void setAxes( s32 idx, s32 val );

        f32 getRawAxes( s32 idx );

        void setRawAxes( s32 idx, s32 val );

        f32 getPrevRawAxes( s32 idx );

        void setPrevRawAxes( s32 idx, s32 val );

        s32 getMap( s32 idx );

        void setMap( s32 idx, s32 val );

        s32 getChannel( s32 idx );

        void setChannel( s32 idx, s32 val );

        s32 getValues( s32 idx );

        void setValues( s32 idx, s32 val );

        s32 getButtons( s32 idx );

        void setButtons( s32 idx, s32 val );

        s32 getReverse( s32 idx );

        void setReverse( s32 idx, s32 val );

        s32 getAxisCount();

        void setAxisCount( s32 val );

        s32 getButtonValues() const;

        void setButtonValues( s32 val );

        s32 getPrevButtonValues() const;

        void setPrevButtonValues( s32 val );

        s32 getNumButtons() const;

        void setNumButtons( s32 val );

        RawPtr<InputState> m_parent;
        RawPtr<InputStateData> m_state;
    };

    class InputState : public CSharedObject<ISharedObject>
    {
    public:
        InputState();

        const SmartPtr<PlayerInputState> &getPlayerInputState() const;
        SmartPtr<PlayerInputState> &getPlayerInputState();
        void setPlayerInputState( SmartPtr<PlayerInputState> val );

    public:
        InputStateData m_state;
        FB_SPIN_MUTEX_MUTABLE( Mutex );

    private:
        SmartPtr<PlayerInputState> m_playerInputState;
    };
}  // namespace fb

#endif  // InputState_h__
