namespace fb
{

    //--------------------------------------------
    inline f64 TimerMT::getDeltaTime() const
    {
        auto task = static_cast<s32>(Thread::getCurrentTask());
        return m_deltaTime[task];
    }

    //--------------------------------------------
    inline f64 TimerMT::getTime() const
    {
        auto task = static_cast<s32>(Thread::getCurrentTask());
        return m_time[task];
    }

    //--------------------------------------------
    inline f64 TimerMT::getTime( Thread::Task task ) const
    {
        return m_time[static_cast<s32>(task)];
    }

    //--------------------------------------------
    inline f64 TimerMT::getPreviousTime( Thread::Task task ) const
    {
        return m_prevTime[static_cast<s32>(task)];
    }

    //--------------------------------------------
    inline f64 TimerMT::getDeltaTime( Thread::Task task ) const
    {
        return m_deltaTime[static_cast<s32>(task)];
    }

    //--------------------------------------------
    inline void TimerMT::setSmoothTime( Thread::Task task, const f64 t )
    {
        m_smoothTime[static_cast<s32>(task)] = t;
    }

    //--------------------------------------------
    inline f64 TimerMT::getSmoothTime( Thread::Task task ) const
    {
        return m_smoothTime[static_cast<s32>(task)];
    }

    //--------------------------------------------
    inline f64 TimerMT::getPrevSmoothTime( Thread::Task task ) const
    {
        return m_prevSmoothTime[static_cast<s32>(task)];
    }

    //--------------------------------------------
    inline f64 TimerMT::getSmoothDeltaTime( Thread::Task task ) const
    {
        return m_smoothDeltaTime[static_cast<s32>(task)];
    }
} // end namespace fb
