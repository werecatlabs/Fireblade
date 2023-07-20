#ifndef ICommandMgrListener_H
#define ICommandMgrListener_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class ICommandManagerListener : public ISharedObject
    {
    public:
        ~ICommandManagerListener() override = default;

        //! Occurs when a command added
        virtual void OnAddCommand( s32 numCommands ) = 0;

        //! Occurs when the next command it retrieved
        virtual void OnGetNextCommand( s32 numNextCommands, s32 numPrevCommands ) = 0;

        //! Occurs when the previous command it retrieved
        virtual void OnGetPreviousCommand( s32 numNextCommands, s32 numPrevCommands ) = 0;
    };

}  // end namespace fb

#endif
