namespace fb
{
    inline time_interval WorkerThread::getTargetFPS() const
    {
        FB_ASSERT( !Math<time_interval>::equals( m_targetFPS, 0.0 ) );
        FB_ASSERT( Math<time_interval>::isFinite( m_targetFPS ) );
        return m_targetFPS;
    }

    inline IWorkerThread::State WorkerThread::getState() const
    {
        return m_state;
    }

}  // end namespace fb
