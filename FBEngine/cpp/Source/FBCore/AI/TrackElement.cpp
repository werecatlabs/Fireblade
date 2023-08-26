#include <FBCore/FBCorePCH.h>
#include <FBCore/AI/TrackElement.h>

namespace fb
{
    TrackElement::TrackElement()
    {
    }

    TrackElement::~TrackElement()
    {
    }

    Vector3F TrackElement::getCenter() const
    {
        return m_center;
    }

    void TrackElement::setCenter( const Vector3F &val )
    {
        m_center = val;
    }

    Vector3F TrackElement::getStart() const
    {
        return m_start;
    }

    void TrackElement::setStart( const Vector3F &val )
    {
        m_start = val;
    }

    Vector3F TrackElement::getEnd() const
    {
        return m_end;
    }

    void TrackElement::setEnd( const Vector3F &val )
    {
        m_end = val;
    }

    Vector3F TrackElement::getDirection() const
    {
        return m_direction;
    }

    void TrackElement::setDirection( const Vector3F &val )
    {
        m_direction = val;
    }

    Vector3F TrackElement::getExtents() const
    {
        return m_extents;
    }

    void TrackElement::setExtents( const Vector3F &val )
    {
        m_extents = val;
    }
}  // namespace fb
