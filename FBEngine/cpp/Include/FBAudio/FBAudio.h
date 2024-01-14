#ifndef FBAudio_h__
#define FBAudio_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class FBAudio : public ISharedObject
    {
    public:
        FBAudio() = default;
        ~FBAudio() override = default;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;
    };

}  // end namespace fb

#endif  // FBAudio_h__
