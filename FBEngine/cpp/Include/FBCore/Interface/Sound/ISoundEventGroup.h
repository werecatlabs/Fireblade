#ifndef ISoundEventGroup_h__
#define ISoundEventGroup_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for a sound event group. */
    class ISoundEventGroup : public ISharedObject
    {
    public:
        /** Destructor. */
        ~ISoundEventGroup() override = default;

        /**
         * Set the volume of the sound event group.
         * @param volume The volume to set.
         */
        virtual void setVolume( f32 volume ) = 0;

        /**
         * Set the mute state of the sound event group.
         * @param mute The mute state to set.
         */
        virtual void setMute( bool mute ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // ISoundEventGroup_h__
