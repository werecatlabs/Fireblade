#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ProcessManager.h>
#include <FBCore/Core/StringUtil.h>

#if defined FB_PLATFORM_WIN32
#    include <processthreadsapi.h>
#    include <windef.h>
#    include <shellapi.h>
#    include <io.h>
#    include <windows.h>
#    include <tlhelp32.h>
#endif

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, ProcessManager, IProcessManager );

    ProcessManager::ProcessManager() = default;

    ProcessManager::~ProcessManager() = default;

    void ProcessManager::createProcess( const String &applicationName )
    {
#ifdef FB_PLATFORM_WIN32
        // additional information
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        // set the size of the structures
        ZeroMemory( &si, sizeof( si ) );
        si.cb = sizeof( si );
        ZeroMemory( &pi, sizeof( pi ) );

        // start the program up
        CreateProcessA( applicationName.c_str(),  // the path
                        nullptr,                  // Command line
                        nullptr,                  // Process handle not inheritable
                        nullptr,                  // Thread handle not inheritable
                        FALSE,                    // Set handle inheritance to FALSE
                        0,                        // No creation flags
                        nullptr,                  // Use parent's environment block
                        nullptr,                  // Use parent's starting directory
                        &si,                      // Pointer to STARTUPINFO structure
                        &pi );                    // Pointer to PROCESS_INFORMATION structure

        // Close process and thread handles.
        // CloseHandle(pi.hProcess);
        // CloseHandle(pi.hThread);
#else
        return;
#endif
    }

    void ProcessManager::createProcess( const StringW &applicationName )
    {
#ifdef FB_PLATFORM_WIN32
        // additional information
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;

        // set the size of the structures
        ZeroMemory( &si, sizeof( si ) );
        si.cb = sizeof( si );
        ZeroMemory( &pi, sizeof( pi ) );

        // start the program up
        CreateProcessW( applicationName.c_str(),  // the path
                        nullptr,                  // Command line
                        nullptr,                  // Process handle not inheritable
                        nullptr,                  // Thread handle not inheritable
                        FALSE,                    // Set handle inheritance to FALSE
                        0,                        // No creation flags
                        nullptr,                  // Use parent's environment block
                        nullptr,                  // Use parent's starting directory
                        &si,                      // Pointer to STARTUPINFO structure
                        &pi );                    // Pointer to PROCESS_INFORMATION structure

        // Close process and thread handles.
        // CloseHandle(pi.hProcess);
        // CloseHandle(pi.hThread);
#else
        return;
#endif
    }

    void ProcessManager::shellExecute( const String &applicationName )
    {
#ifdef FB_PLATFORM_WIN32
        SHELLEXECUTEINFOA shExInfo = { 0 };
        shExInfo.cbSize = sizeof( shExInfo );
        shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExInfo.hwnd = nullptr;
        shExInfo.lpVerb = "runas";                  // Operation to perform
        shExInfo.lpFile = applicationName.c_str();  // Application to start
        shExInfo.lpParameters = "";                 // Additional parameters
        shExInfo.lpDirectory = nullptr;
        shExInfo.nShow = SW_SHOW;
        shExInfo.hInstApp = nullptr;

        if( ShellExecuteExA( &shExInfo ) )
        {
            WaitForSingleObject( shExInfo.hProcess, INFINITE );
            CloseHandle( shExInfo.hProcess );
        }
#else
        return;
#endif
    }

    void ProcessManager::shellExecute( const StringW &applicationName, const Array<StringW> &args )
    {
#ifdef FB_PLATFORM_WIN32
        StringW argStr;

        for( const auto &arg : args )
        {
            argStr += arg + L" ";
        }

        SHELLEXECUTEINFOW shExInfo = { 0 };
        shExInfo.cbSize = sizeof( shExInfo );
        shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExInfo.hwnd = nullptr;
        shExInfo.lpVerb = L"runas";                 // Operation to perform
        shExInfo.lpFile = applicationName.c_str();  // Application to start
        shExInfo.lpParameters = argStr.c_str();     // Additional parameters
        shExInfo.lpDirectory = nullptr;
        shExInfo.nShow = SW_SHOW;
        shExInfo.hInstApp = nullptr;

        if( ShellExecuteExW( &shExInfo ) )
        {
            // WaitForSingleObject(shExInfo.hProcess, INFINITE);
            CloseHandle( shExInfo.hProcess );
        }
#else
        return;
#endif
    }

    void ProcessManager::shellExecute( const StringW &applicationName, const StringW &directory,
                                       const Array<StringW> &args )
    {
#ifdef FB_PLATFORM_WIN32
        StringW argStr;

        for( const auto &arg : args )
        {
            argStr += arg + L" ";
        }

        SHELLEXECUTEINFOW shExInfo = { 0 };
        shExInfo.cbSize = sizeof( shExInfo );
        shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExInfo.hwnd = nullptr;
        shExInfo.lpVerb = L"runas";                 // Operation to perform
        shExInfo.lpFile = applicationName.c_str();  // Application to start
        shExInfo.lpParameters = argStr.c_str();     // Additional parameters
        shExInfo.lpDirectory = directory.c_str();
        shExInfo.nShow = SW_SHOW;
        shExInfo.hInstApp = nullptr;

        if( ShellExecuteExW( &shExInfo ) )
        {
            // WaitForSingleObject(shExInfo.hProcess, INFINITE);
            CloseHandle( shExInfo.hProcess );
        }
#else
        return;
#endif
    }

    auto ProcessManager::isProcessRunning( const String &processName ) -> bool
    {
        return isProcessRunning( StringUtil::toStringW( processName ) );
    }

    auto ProcessManager::isProcessRunning( const StringW &processName ) -> bool
    {
#ifdef FB_PLATFORM_WIN32
        bool exists = false;
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof( PROCESSENTRY32W );

        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

        if( Process32FirstW( snapshot, &entry ) )
        {
            while( Process32NextW( snapshot, &entry ) )
            {
                if( StringW( entry.szExeFile ) == processName )
                {
                    exists = true;
                }
            }
        }

        CloseHandle( snapshot );
        return exists;
#else
        return false;
#endif
    }

    auto ProcessManager::terminateProcess( const String &processName ) -> bool
    {
        return terminateProcess( StringUtil::toStringW( processName ) );
    }

    auto ProcessManager::terminateProcess( const StringW &processName ) -> bool
    {
#ifdef FB_PLATFORM_WIN32
        bool exists = false;
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof( PROCESSENTRY32W );

        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

        if( Process32FirstW( snapshot, &entry ) )
        {
            while( Process32NextW( snapshot, &entry ) )
            {
                if( StringW( entry.szExeFile ) != processName )
                {
                    internalTerminateProcess( entry.th32ProcessID, 0 );
                    exists = true;
                }
            }
        }

        CloseHandle( snapshot );
        return exists;
#else
        return false;
#endif
    }

    auto ProcessManager::internalTerminateProcess( unsigned long dwProcessId, u32 uExitCode ) -> bool
    {
#ifdef FB_PLATFORM_WIN32
        DWORD dwDesiredAccess = PROCESS_TERMINATE;
        BOOL bInheritHandle = FALSE;
        HANDLE hProcess = OpenProcess( dwDesiredAccess, bInheritHandle, dwProcessId );
        if( hProcess == nullptr )
        {
            return false;
        }

        BOOL result = TerminateProcess( hProcess, uExitCode );

        CloseHandle( hProcess );

        return result == TRUE;
#else
        return false;
#endif
    }
}  // end namespace fb
