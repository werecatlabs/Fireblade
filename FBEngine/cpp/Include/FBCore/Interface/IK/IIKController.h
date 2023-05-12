#ifndef _IIKCONTROLLER_H
#define _IIKCONTROLLER_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    
    class IIKController : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IIKController() override = default;
    };

}  // end namespace fb

#endif
