#ifndef ILearning_h__
#define ILearning_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class ILearning : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~ILearning() override = default;
    };
}  // end namespace fb

#endif  // ILearning_h__
