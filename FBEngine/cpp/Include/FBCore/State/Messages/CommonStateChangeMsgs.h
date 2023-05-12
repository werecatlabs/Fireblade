#ifndef _CommonStateChangeMsgs_H_
#define _CommonStateChangeMsgs_H_

#include "FBCore/State/Messages/StateMessageStandard.h"
#include "FBCore/Math/Vector2.h"
#include <FBCore/Math/Vector3.h>
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Base/StringTypes.h"

namespace fb
{
    extern const u32 CSCT_UDPATE;
    extern const u32 CSCT_TRANSFORMATION;
    extern const u32 CSCT_POSITION;
    extern const u32 CSCT_ROTATION;
    extern const u32 CSCT_ORIENTATION;
    extern const u32 CSCT_SCALE;
    extern const u32 CSCT_VELOCITY;
    extern const u32 CSCT_SETMATERIAL;
    extern const u32 CSCT_SETVISIBLE;
    extern const u32 CSCT_SHUTDOWN;
    extern const u32 CSCT_SETSTATE;
    extern const u32 CSCT_SETFLAG;
    extern const u32 CSCT_MATERIALNAME;

    // 2d messages
    extern const u32 CSCT_POSITION_2;
    extern const u32 CSCT_VELOCITY_2;
    extern const u32 CSCT_ROTATION_2;
    extern const u32 CSCT_SCALE_2;

    //---------------------------------------------
    class Position2Msg : public StateMessageStandard
    {
    public:
        Position2Msg()
        {
            setType( CSCT_POSITION_2 );
        }

        Position2Msg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_POSITION_2 );
        }

        Position2Msg( u32 subjectId, const Vector2F &position ) :
            StateMessageStandard( subjectId ),
            Position( position )
        {
            setType( CSCT_POSITION_2 );
        }

        Vector2F Position;
    };

    using Position2MsgPtr = SmartPtr<Position2Msg>;

    //---------------------------------------------
    class Velocity2Msg : public StateMessageStandard
    {
    public:
        Velocity2Msg()
        {
            setType( CSCT_VELOCITY_2 );
        }

        Velocity2Msg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_VELOCITY_2 );
        }

        Velocity2Msg( u32 subjectId, const Vector2F &position ) :
            StateMessageStandard( subjectId ),
            Velocity( position )
        {
            setType( CSCT_VELOCITY_2 );
        }

        Vector2F Velocity;
    };

    using Velocity2MsgPtr = SmartPtr<Velocity2Msg>;

    //---------------------------------------------
    class TransformChangedMsg : public StateMessageStandard
    {
    public:
        TransformChangedMsg()
        {
            setType( CSCT_TRANSFORMATION );
        }

        TransformChangedMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_TRANSFORMATION );
        }

        Vector3F Position;
        QuaternionF Orientation;
        Vector3F Scale;
    };

    using TransformChangedMsgPtr = SmartPtr<TransformChangedMsg>;

    //---------------------------------------------
    class PositionChangedMsg : public StateMessageStandard
    {
    public:
        PositionChangedMsg()
        {
            setType( CSCT_POSITION );
        }

        PositionChangedMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_POSITION );
        }

        PositionChangedMsg( u32 subjectId, const Vector3F &position ) :
            StateMessageStandard( subjectId ),
            Position( position )
        {
            setType( CSCT_POSITION );
        }

        Vector3F Position;
    };

    using PositionChangedMsgPtr = SmartPtr<PositionChangedMsg>;

    //---------------------------------------------
    class RotationChangedMsg : public StateMessageStandard
    {
    public:
        RotationChangedMsg()
        {
            setType( CSCT_ROTATION );
        }

        RotationChangedMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_ROTATION );
        }

        RotationChangedMsg( u32 subjectId, const Vector3F &rotation ) :
            StateMessageStandard( subjectId ),
            Rotation( rotation )
        {
            setType( CSCT_ROTATION );
        }

        Vector3F Rotation;
    };

    using RotationChangedMsgPtr = SmartPtr<RotationChangedMsg>;

    //---------------------------------------------
    class OrientationChangedMsg : public StateMessageStandard
    {
    public:
        OrientationChangedMsg()
        {
            setType( CSCT_ORIENTATION );
        }

        OrientationChangedMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_ORIENTATION );
        }

        OrientationChangedMsg( u32 subjectId, const QuaternionF &orientation ) :
            StateMessageStandard( subjectId ),
            Orientation( orientation )
        {
            setType( CSCT_ORIENTATION );
        }

        QuaternionF Orientation;
    };

    using OrientationChangedMsgPtr = SmartPtr<OrientationChangedMsg>;

    //---------------------------------------------
    class ScaleChangedMsg : public StateMessageStandard
    {
    public:
        ScaleChangedMsg()
        {
            setType( CSCT_SCALE );
        }

        ScaleChangedMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_SCALE );
        }

        ScaleChangedMsg( u32 subjectId, const Vector3F &scale ) :
            StateMessageStandard( subjectId ),
            Scale( scale )
        {
            setType( CSCT_SCALE );
        }

        Vector3F Scale;
    };

    using ScaleChangedMsgPtr = SmartPtr<ScaleChangedMsg>;

    //---------------------------------------------
    class VelocityChangedMsg : public StateMessageStandard
    {
    public:
        VelocityChangedMsg()
        {
            setType( CSCT_VELOCITY );
        }

        VelocityChangedMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_VELOCITY );
        }

        VelocityChangedMsg( u32 subjectId, const Vector3F &velocity ) :
            StateMessageStandard( subjectId ),
            Velocity( velocity )
        {
            setType( CSCT_VELOCITY );
        }

        Vector3F Velocity;
    };

    using VelocityChangedMsgPtr = SmartPtr<VelocityChangedMsg>;

    //---------------------------------------------
    class SetMaterialMsg : public StateMessageStandard
    {
    public:
        SetMaterialMsg()
        {
            setType( CSCT_SETMATERIAL );
        }

        SetMaterialMsg( u32 subjectId, const String &materialName ) :
            StateMessageStandard( subjectId ),
            MaterialName( materialName )
        {
            setType( CSCT_SETMATERIAL );
        }

        const String &getMaterialName() const
        {
            return MaterialName;
        }

    private:
        String MaterialName;
    };

    using SetMaterialMsgPtr = SmartPtr<SetMaterialMsg>;

    //---------------------------------------------
    class SetVisibleMsg : public StateMessageStandard
    {
    public:
        SetVisibleMsg()
        {
            setType( CSCT_SETVISIBLE );
        }

        SetVisibleMsg( u32 subjectId, bool isVisible ) :
            StateMessageStandard( subjectId ),
            IsVisible( isVisible )
        {
            setType( CSCT_SETVISIBLE );
        }

        bool isVisible() const
        {
            return IsVisible;
        }

    private:
        bool IsVisible;
    };

    using SetVisibleMsgPtr = SmartPtr<SetVisibleMsg>;

    //---------------------------------------------
    class SetStateMsg : public StateMessageStandard
    {
    public:
        SetStateMsg()
        {
            setType( CSCT_SETSTATE );
        }

        SetStateMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_SETSTATE );
        }

        u32 State;
    };

    using SetStateMsgPtr = SmartPtr<SetStateMsg>;

    //---------------------------------------------
    class SetFlagMsg : public StateMessageStandard
    {
    public:
        SetFlagMsg()
        {
            setType( CSCT_SETFLAG );
        }

        SetFlagMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_SETFLAG );
        }

        u32 Flag;
    };

    using SetFlagMsgPtr = SmartPtr<SetFlagMsg>;

    //---------------------------------------------
    class SetMaterialNameMsg : public StateMessageStandard
    {
    public:
        SetMaterialNameMsg()
        {
            setType( CSCT_SETFLAG );
        }

        SetMaterialNameMsg( u32 subjectId ) : StateMessageStandard( subjectId )
        {
            setType( CSCT_SETFLAG );
        }

        String MaterialName;
    };

    using SetMaterialNameMsgPtr = SmartPtr<SetMaterialNameMsg>;
}  // end namespace fb

#endif
