#include <FBCore/FBCorePCH.h>
#include <FBCore/System/OutputManagerConsole.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

#ifdef FB_PLATFORM_WIN32
#    include <windows.h>
#endif

namespace fb
{
    OutputManagerConsole::OutputManagerConsole()
    {
    }

    OutputManagerConsole::~OutputManagerConsole()
    {
    }

    void OutputManagerConsole::output( const String &outputStr )
    {
#ifdef FB_PLATFORM_WIN32
        using namespace std;
        wcout << "" << endl;
        wcout << outputStr.c_str() << endl;
#endif
    }
}  // end namespace fb
