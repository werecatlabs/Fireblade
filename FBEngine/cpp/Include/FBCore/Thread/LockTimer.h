#ifndef LockTimer_h__
#define LockTimer_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Thread/RecursiveMutex.h>
#include <string>

namespace fb
{
    class LockTimer
    {
    public:
        LockTimer( RecursiveMutex &mutex, const char *file = __FILE__, int line = __LINE__ );
        ~LockTimer();

    protected:
        u32 m_startTime;
        u32 m_endTime;
        u32 m_timeTaken;
        std::string m_file;
        int m_line;

        RecursiveMutex &m_mutex;
    };
}  // end namespace fb

#endif  // LockTimer_h__
