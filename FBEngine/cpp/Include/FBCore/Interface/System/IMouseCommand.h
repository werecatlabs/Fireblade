#ifndef IMouseCommand_H
#define IMouseCommand_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/ICommand.h>

namespace fb
{
    //--------------------------------------------
    class IMouseCommand : public ICommand
    {
    public:
        ~IMouseCommand() override = default;

        /** */
        virtual bool OnEvent( const SmartPtr<IInputEvent> &event ) = 0;
    };
}  // end namespace fb

#endif
