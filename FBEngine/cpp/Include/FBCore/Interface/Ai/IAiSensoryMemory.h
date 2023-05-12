#ifndef IAiSensoryMemory_h__
#define IAiSensoryMemory_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    
    class IAiSensoryMemory : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAiSensoryMemory() override = default;
    };

}  // end namespace fb

#endif  // IAiSensoryMemory_h__
