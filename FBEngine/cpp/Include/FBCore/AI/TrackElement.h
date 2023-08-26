#ifndef CTrackElement_h__
#define CTrackElement_h__

#include <FBCore/Interface/Ai/ITrackElement.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    class TrackElement : public ITrackElement
    {
    public:
        TrackElement();
        ~TrackElement() override;

        Vector3F getCenter() const override;
        void setCenter( const Vector3F &val ) override;

        Vector3F getStart() const override;
        void setStart( const Vector3F &val ) override;

        Vector3F getEnd() const override;
        void setEnd( const Vector3F &val ) override;

        Vector3F getDirection() const override;
        void setDirection( const Vector3F &val ) override;

        Vector3F getExtents() const override;
        void setExtents( const Vector3F &val ) override;

    protected:
        Vector3F m_center;
        Vector3F m_start;
        Vector3F m_end;
        Vector3F m_direction;
        Vector3F m_extents;
    };
}  // end namespace fb

#endif  // CTrackElement_h__
