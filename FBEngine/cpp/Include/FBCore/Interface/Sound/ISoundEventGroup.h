#ifndef ISoundEventGroup_h__
#define ISoundEventGroup_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class ISoundEventGroup : public ISharedObject
    {
    public:
        ~ISoundEventGroup() override = default;

        virtual void setVolume( f32 volume ) = 0;
        virtual void setMute( bool mute ) = 0;
    };
}  // end namespace fb

#endif  // ISoundEventGroup_h__
