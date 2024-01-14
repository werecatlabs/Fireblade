#include <FBCore/FBCorePCH.h>
#include <FBCore/AI/TrackElement.h>

namespace fb
{
    TrackElement::TrackElement() = default;

    TrackElement::~TrackElement() = default;

    auto TrackElement::getCenter() const -> Vector3F
    {
        return m_center;
    }

    void TrackElement::setCenter( const Vector3F &val )
    {
        m_center = val;
    }

    auto TrackElement::getStart() const -> Vector3F
    {
        return m_start;
    }

    void TrackElement::setStart( const Vector3F &val )
    {
        m_start = val;
    }

    auto TrackElement::getEnd() const -> Vector3F
    {
        return m_end;
    }

    void TrackElement::setEnd( const Vector3F &val )
    {
        m_end = val;
    }

    auto TrackElement::getDirection() const -> Vector3F
    {
        return m_direction;
    }

    void TrackElement::setDirection( const Vector3F &val )
    {
        m_direction = val;
    }

    auto TrackElement::getExtents() const -> Vector3F
    {
        return m_extents;
    }

    void TrackElement::setExtents( const Vector3F &val )
    {
        m_extents = val;
    }
}  // namespace fb
