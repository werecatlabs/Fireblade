#ifndef IConsole_h__
#define IConsole_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for an interactive console. */
    class IConsole : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IConsole() override = default;
    };
}  // end namespace fb

#endif  // IConsole_h__
