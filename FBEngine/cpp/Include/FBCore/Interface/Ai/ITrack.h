#ifndef ITrack_h__
#define ITrack_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class ITrack : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ITrack() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // ITrack_h__
