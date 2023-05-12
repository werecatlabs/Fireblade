#ifndef _IIKMANAGER_H
#define _IIKMANAGER_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    
    class IIKManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IIKManager() override = default;
    };

}  // end namespace fb

#endif
