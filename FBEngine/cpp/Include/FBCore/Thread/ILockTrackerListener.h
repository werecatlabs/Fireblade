#ifndef ILockTrackerListener_h__
#define ILockTrackerListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Thread/Threading.h>

#if FB_USE_TBB
#else
#    include <mutex>
#endif

namespace fb
{
    class ILockTrackerListener : public ISharedObject
    {
    public:
        ~ILockTrackerListener() override = default;

#if FB_USE_ONETBB
        virtual void lock( std::recursive_mutex &mutex, const char *file = __FILE__,
                           int line = __LINE__ ) = 0;
        virtual void unlock( std::recursive_mutex &mutex, u32 timeTaken = 0, const char *file = __FILE__,
                             int line = __LINE__ ) = 0;
#elif FB_USE_TBB
        virtual void lock( tbb::recursive_mutex &mutex, const char *file = __FILE__,
                           int line = __LINE__ ) = 0;
        virtual void unlock( tbb::recursive_mutex &mutex, u32 timeTaken = 0, const char *file = __FILE__,
                             int line = __LINE__ ) = 0;
#else
        virtual void lock( std::recursive_mutex &mutex, const char *file = __FILE__,
                           int line = __LINE__ ) = 0;
        virtual void unlock( std::recursive_mutex &mutex, u32 timeTaken = 0, const char *file = __FILE__,
                             int line = __LINE__ ) = 0;
#endif
    };
}  // end namespace fb

#endif  // ILockTrackerListener_h__
