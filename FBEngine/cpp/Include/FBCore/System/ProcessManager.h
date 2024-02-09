#ifndef CProcessManager_h__
#define CProcessManager_h__

#include <FBCore/Interface/System/IProcessManager.h>

namespace fb
{

    /** @class ProcessManager
     * @brief
     * This class is used to manage processes.
     * It can create a process, shell execute a process, check if a process is running and
     * terminate a process. It is a singleton class.
     */
    class ProcessManager : public IProcessManager
    {
    public:
        /** Constructor */
        ProcessManager();

        /** Destructor */
        ~ProcessManager() override;

        /** @copydoc IProcessManager::createProcess */
        void createProcess( const String &applicationName ) override;

        /** @copydoc IProcessManager::createProcess */
        void createProcess( const StringW &applicationName ) override;

        /** @copydoc IProcessManager::shellExecute */
        void shellExecute( const String &applicationName ) override;

        /** @copydoc IProcessManager::shellExecute */
        void shellExecute( const StringW &applicationName, const Array<StringW> &args ) override;

        /** @copydoc IProcessManager::shellExecute */
        void shellExecute( const StringW &applicationName, const StringW &directory,
                           const Array<StringW> &args ) override;

        /** @copydoc IProcessManager::isProcessRunning */
        bool isProcessRunning( const String &processName ) override;

        /** @copydoc IProcessManager::terminateProcess */
        bool terminateProcess( const String &processName ) override;

        /** @copydoc IProcessManager::isProcessRunning */
        bool isProcessRunning( const StringW &processName ) override;

        /** @copydoc IProcessManager::terminateProcess */
        bool terminateProcess( const StringW &processName ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        bool internalTerminateProcess( unsigned long dwProcessId, u32 uExitCode );
    };
}  // end namespace fb

#endif  // CProcessManager_h__
