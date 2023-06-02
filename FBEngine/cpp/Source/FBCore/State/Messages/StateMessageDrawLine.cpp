#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageDrawLine.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageDrawLine, StateMessage );
    const hash_type StateMessageDrawLine::DRAW_LINE_HASH = StringUtil::getHash( "DrawLine" );

    Vector3<real_Num> StateMessageDrawLine::getStart() const
    {
        return m_start;
    }

    void StateMessageDrawLine::setStart( const Vector3<real_Num> &start )
    {
        m_start = start;
    }

    Vector3<real_Num> StateMessageDrawLine::getEnd() const
    {
        return m_end;
    }

    void StateMessageDrawLine::setEnd( const Vector3<real_Num> &end )
    {
        m_end = end;
    }

    s32 StateMessageDrawLine::getLineId() const
    {
        return m_lineId;
    }

    void StateMessageDrawLine::setLineId( s32 lineId )
    {
        m_lineId = lineId;
    }

    u32 StateMessageDrawLine::getColour() const
    {
        return m_colour;
    }

    void StateMessageDrawLine::setColour( u32 colour )
    {
        m_colour = colour;
    }
}  // end namespace fb
