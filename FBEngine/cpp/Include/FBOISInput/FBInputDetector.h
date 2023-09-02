#ifndef FBInputDetector_h__
#define FBInputDetector_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class InputDetector : public ISharedObject
    {
    public:
        ~InputDetector() override;

        /** Sets the event id this triggers. */
        virtual void setEventId( u32 eventId );

        /** Gets the event id this triggers. */
        virtual u32 getEventId() const;
    };
}  // end namespace fb

#endif  // FBInputDetector_h__
