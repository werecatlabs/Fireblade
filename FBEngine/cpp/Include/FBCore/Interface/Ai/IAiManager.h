#ifndef _IAiManager_H
#define _IAiManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    
    /** A manager class used to create and manager AI objects.
     */
    class IAiManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IAiManager() override = default;

        virtual SmartPtr<IPathfinder2> getPathfinder2() const = 0;
        virtual void setPathfinder2( SmartPtr<IPathfinder2> pathfinder ) = 0;
    };
}  // end namespace fb

#endif
