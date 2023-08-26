#ifndef CTrack_h__
#define CTrack_h__

#include <FBCore/Interface/Ai/ITrack.h>

namespace fb
{
    class Track : public ITrack
    {
    public:
        Track();
        ~Track() override;

    protected:
        Array<SmartPtr<ITrackElement>> m_elements;
    };

}  // end namespace fb

#endif  // CTrack_h__
