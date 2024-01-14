#include <FBCore/FBCorePCH.h>
#include <FBCore/State/Messages/StateMessageDrawLine.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateMessageDrawLine, StateMessage );
    const hash_type StateMessageDrawLine::DRAW_LINE_HASH = StringUtil::getHash( "DrawLine" );

    auto StateMessageDrawLine::getStart() const -> Vector3<real_Num>
    {
        return m_start;
    }

    void StateMessageDrawLine::setStart( const Vector3<real_Num> &start )
    {
        m_start = start;
    }

    auto StateMessageDrawLine::getEnd() const -> Vector3<real_Num>
    {
        return m_end;
    }

    void StateMessageDrawLine::setEnd( const Vector3<real_Num> &end )
    {
        m_end = end;
    }

    auto StateMessageDrawLine::getLineId() const -> s32
    {
        return m_lineId;
    }

    void StateMessageDrawLine::setLineId( s32 lineId )
    {
        m_lineId = lineId;
    }

    auto StateMessageDrawLine::getColour() const -> u32
    {
        return m_colour;
    }

    void StateMessageDrawLine::setColour( u32 colour )
    {
        m_colour = colour;
    }
}  // end namespace fb
