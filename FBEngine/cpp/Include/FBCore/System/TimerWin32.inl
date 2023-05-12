#ifdef FB_PLATFORM_WIN32

namespace fb
{
    //---------------------------------------------
    inline u32 TimerWin32::getTimeMilliseconds() const
    {
        if(isStopped())
            return LastVirtualTime;

        return LastVirtualTime + static_cast<u32>(( StaticTime - StartRealTime ) * VirtualTimerSpeed);
    }


    //---------------------------------------------
    inline bool TimerWin32::isStopped() const
    {
        return VirtualTimerStopCounter != 0;
    }
} // end namespace fb


#endif
