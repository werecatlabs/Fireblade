#ifndef IAnimationTimeIndex_h__
#define IAnimationTimeIndex_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class IAnimationTimeIndex : public ISharedObject
    {
    public:
        ~IAnimationTimeIndex() override = default;

        virtual bool hasKeyIndex() const = 0;

        virtual f32 getTimePos() const = 0;

        virtual u32 getKeyIndex() const = 0;
    };

}  // end namespace fb

#endif  // IAnimationTimeIndex_h__
