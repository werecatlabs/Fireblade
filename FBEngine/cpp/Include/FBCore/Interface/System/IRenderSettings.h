#ifndef IRenderSettings_h__
#define IRenderSettings_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IRenderSettings : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IRenderSettings() override = default;

        FB_CLASS_REGISTER_DECL;
    };

}  // namespace fb

#endif  // IRenderSettings_h__
