#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ProcessManager.h>
#include <FBCore/Base/StringUtil.h>

#if defined FB_PLATFORM_WIN32
#    include <shellapi.h>
#    include <io.h>
#    include <windows.h>
#    include <tlhelp32.h>
#endif

namespace fb
{
    ProcessManager::ProcessManager()
    {
    }

    ProcessManager::~ProcessManager()
    {
    }

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
        CreateProcessA( applicationName.c_str(), // the path
                        nullptr,                 // Command line
                        nullptr,                 // Process handle not inheritable
                        nullptr,                 // Thread handle not inheritable
                        FALSE,                   // Set handle inheritance to FALSE
                        0,                       // No creation flags
                        nullptr,                 // Use parent's environment block
                        nullptr,                 // Use parent's starting directory
                        &si,                     // Pointer to STARTUPINFO structure
                        &pi );                   // Pointer to PROCESS_INFORMATION structure

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
        CreateProcessW( applicationName.c_str(), // the path
                        nullptr,                 // Command line
                        nullptr,                 // Process handle not inheritable
                        nullptr,                 // Thread handle not inheritable
                        FALSE,                   // Set handle inheritance to FALSE
                        0,                       // No creation flags
                        nullptr,                 // Use parent's environment block
                        nullptr,                 // Use parent's starting directory
                        &si,                     // Pointer to STARTUPINFO structure
                        &pi );                   // Pointer to PROCESS_INFORMATION structure

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
        shExInfo.lpVerb = "runas";                 // Operation to perform
        shExInfo.lpFile = applicationName.c_str(); // Application to start
        shExInfo.lpParameters = "";                // Additional parameters
        shExInfo.lpDirectory = nullptr;
        shExInfo.nShow = SW_SHOW;
        shExInfo.hInstApp = nullptr;

        if(ShellExecuteExA( &shExInfo ))
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

        for(size_t i = 0; i < args.size(); ++i)
        {
            argStr += args[i] + L" ";
        }

        SHELLEXECUTEINFOW shExInfo = { 0 };
        shExInfo.cbSize = sizeof( shExInfo );
        shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExInfo.hwnd = nullptr;
        shExInfo.lpVerb = L"runas";                // Operation to perform
        shExInfo.lpFile = applicationName.c_str(); // Application to start
        shExInfo.lpParameters = argStr.c_str();    // Additional parameters
        shExInfo.lpDirectory = nullptr;
        shExInfo.nShow = SW_SHOW;
        shExInfo.hInstApp = nullptr;

        if(ShellExecuteExW( &shExInfo ))
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

        for(size_t i = 0; i < args.size(); ++i)
        {
            argStr += args[i] + L" ";
        }

        SHELLEXECUTEINFOW shExInfo = { 0 };
        shExInfo.cbSize = sizeof( shExInfo );
        shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExInfo.hwnd = nullptr;
        shExInfo.lpVerb = L"runas";                // Operation to perform
        shExInfo.lpFile = applicationName.c_str(); // Application to start
        shExInfo.lpParameters = argStr.c_str();    // Additional parameters
        shExInfo.lpDirectory = directory.c_str();
        shExInfo.nShow = SW_SHOW;
        shExInfo.hInstApp = nullptr;

        if(ShellExecuteExW( &shExInfo ))
        {
            // WaitForSingleObject(shExInfo.hProcess, INFINITE);
            CloseHandle( shExInfo.hProcess );
        }
#else
        return;
#endif
    }

    bool ProcessManager::isProcessRunning( const String &processName )
    {
        return isProcessRunning( StringUtil::toStringW( processName ) );
    }

    bool ProcessManager::isProcessRunning( const StringW &processName )
    {
#ifdef FB_PLATFORM_WIN32
        bool exists = false;
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof( PROCESSENTRY32W );

        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

        if(Process32FirstW( snapshot, &entry ))
        {
            while(Process32NextW( snapshot, &entry ))
            {
                if(StringW( entry.szExeFile ) == processName)
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

    bool ProcessManager::terminateProcess( const String &processName )
    {
        return terminateProcess( StringUtil::toStringW( processName ) );
    }

    bool ProcessManager::terminateProcess( const StringW &processName )
    {
#ifdef FB_PLATFORM_WIN32
        bool exists = false;
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof( PROCESSENTRY32W );

        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

        if(Process32FirstW( snapshot, &entry ))
        {
            while(Process32NextW( snapshot, &entry ))
            {
                if(StringW( entry.szExeFile ) != processName)
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

    bool ProcessManager::internalTerminateProcess( unsigned long dwProcessId, u32 uExitCode )
    {
#ifdef FB_PLATFORM_WIN32
        DWORD dwDesiredAccess = PROCESS_TERMINATE;
        BOOL bInheritHandle = FALSE;
        HANDLE hProcess = OpenProcess( dwDesiredAccess, bInheritHandle, dwProcessId );
        if(hProcess == nullptr)
            return false;

        BOOL result = TerminateProcess( hProcess, uExitCode );

        CloseHandle( hProcess );

        return result == TRUE;
#else
        return false;
#endif
    }
} // end namespace fb
