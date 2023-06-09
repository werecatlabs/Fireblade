#ifndef CProcessManager_h__
#define CProcessManager_h__

#include <FBCore/Interface/System/IProcessManager.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    
    class ProcessManager : public SharedObject<IProcessManager>
    {
    public:
        ProcessManager();
        ~ProcessManager() override;

        void createProcess( const String &applicationName ) override;

        void createProcess( const StringW &applicationName ) override;

        void shellExecute( const String &applicationName ) override;

        void shellExecute( const StringW &applicationName, const Array<StringW> &args ) override;

        void shellExecute( const StringW &applicationName, const StringW &directory,
                           const Array<StringW> &args ) override;

        bool isProcessRunning( const String &processName ) override;

        bool terminateProcess( const String &processName ) override;

        bool isProcessRunning( const StringW &processName ) override;

        bool terminateProcess( const StringW &processName ) override;

    protected:
        bool internalTerminateProcess( unsigned long dwProcessId, u32 uExitCode );
    };
}  // end namespace fb

#endif  // CProcessManager_h__
