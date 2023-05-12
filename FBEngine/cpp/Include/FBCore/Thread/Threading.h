#ifndef __FBThreading_H__
#define __FBThreading_H__

#include <FBCore/FBCoreTypes.h>

#if FB_USE_ONETBB
#    include <FBCore/Thread/ThreadingTBB.h>
#elif FB_USE_TBB
#    include <FBCore/Thread/ThreadingTBB.h>
#elif FB_USE_BOOST
#    include <FBCore/Thread/ThreadingBoost.h>
#else
#    if FB_ENABLE_THREADING
#        include <FBCore/Thread/ThreadingStd.h>
#    else
#        include <FBCore/Thread/ThreadingNone.h>
#    endif
#endif

#include <FBCore/Thread/ThreadTypes.h>

#endif
