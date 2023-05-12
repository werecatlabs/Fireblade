#ifndef _FBCoreAutolink_H
#define _FBCoreAutolink_H

#include <FBCore/FBCoreConfig.h>

#if FB_USE_AUTO_LINK
#    if defined _FB_STATIC_LIB_
#        ifdef _DEBUG
#            ifdef FB_EXPORTS
#            else
#                pragma comment( lib, "FBCore.lib" )
#            endif  // FB_EXPORT
#        elif NDEBUG
#            ifdef FB_EXPORTS
#            else
#                pragma comment( lib, "FBCore.lib" )
#            endif  // FB_EXPORT
#        else
#            ifdef FB_EXPORTS
#            else
#                pragma comment( lib, "FBCore.lib" )
#            endif  // FB_EXPORT
#        endif
#    endif
#endif

#endif
