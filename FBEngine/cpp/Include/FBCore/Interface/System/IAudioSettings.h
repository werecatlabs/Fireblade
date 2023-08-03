#ifndef IAudioSettings_h__
#define IAudioSettings_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IAudioSettings : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAudioSettings() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif // IAudioSettings_h__
