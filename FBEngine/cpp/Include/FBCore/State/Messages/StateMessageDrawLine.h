#ifndef StateMessageDrawLine_h__
#define StateMessageDrawLine_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    //---------------------------------------------
    class StateMessageDrawLine : public StateMessage
    {
    public:
        static const hash_type DRAW_LINE_HASH;

        StateMessageDrawLine() = default;
        StateMessageDrawLine( const StateMessageDrawLine &other ) = delete;
        ~StateMessageDrawLine() override = default;

        Vector3<real_Num> getStart() const;
        void setStart( const Vector3<real_Num> &start );

        Vector3<real_Num> getEnd() const;
        void setEnd( const Vector3<real_Num> &end );

        s32 getLineId() const;

        void setLineId( s32 lineId );

        u32 getColour() const;
        void setColour( u32 colour );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector3<real_Num> m_start;
        Vector3<real_Num> m_end;
        s32 m_lineId = 0;
        u32 m_colour = 0;
    };
}  // namespace fb

#endif  // StateMessageDrawLine_h__
