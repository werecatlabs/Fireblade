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

        Vector3<real_Num> getCenter() const override;
        void setCenter( const Vector3<real_Num> &val ) override;

        Vector3<real_Num> getStart() const override;
        void setStart( const Vector3<real_Num> &val ) override;

        Vector3<real_Num> getEnd() const override;
        void setEnd( const Vector3<real_Num> &val ) override;

        Vector3<real_Num> getDirection() const override;
        void setDirection( const Vector3<real_Num> &val ) override;

        Vector3<real_Num> getExtents() const override;
        void setExtents( const Vector3<real_Num> &val ) override;

    protected:
        Vector3<real_Num> m_center;
        Vector3<real_Num> m_start;
        Vector3<real_Num> m_end;
        Vector3<real_Num> m_direction;
        Vector3<real_Num> m_extents;
    };
}  // end namespace fb

#endif  // CTrackElement_h__
