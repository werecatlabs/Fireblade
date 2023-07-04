#include <FBCore/FBCorePCH.h>
#include <FBCore/System/MessageBox.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/StringUtil.h>

#if defined FB_PLATFORM_WIN32
#    ifdef _MSC_VER
//#define WIN32_LEAN_AND_MEAN
#        include <wtypes.h>
//#include <winnt.h>
#        include <WinUser.h>
#    endif
#endif

namespace fb
{
    void MessageBoxUtil::show( const String &text )
    {
        show( text.c_str() );
    }

    void MessageBoxUtil::show( const char *text )
    {
#ifdef FB_PLATFORM_WIN32
        auto str = StringUtil::getWString( text );
        MessageBoxW( nullptr, str.c_str(), L"", MB_OK );
        FB_LOG( text );
#else
        FB_LOG( text );
#endif
    }
}  // end namespace fb
