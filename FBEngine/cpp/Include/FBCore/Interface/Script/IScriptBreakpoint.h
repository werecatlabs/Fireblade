#ifndef IScriptBreakpoint_h__
#define IScriptBreakpoint_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * Interface for a script breakpoint.
     */
    class IScriptBreakpoint : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IScriptBreakpoint() override = default;

        /** Gets the line number where the breakpoint is set. */
        virtual s32 getLineNumber() const = 0;

        /** Sets the line number where the breakpoint is set. */
        virtual void setLineNumber( s32 lineNumber ) = 0;

        /** Gets the file path where the breakpoint is set. */
        virtual String getFilePath() const = 0;

        /** Sets the file path where the breakpoint is set. */
        virtual void setFilePath( const String &filePath ) = 0;
    };

}  // end namespace fb

#endif  // IScriptBreakpoint_h__
