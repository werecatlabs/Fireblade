namespace fb
{
    namespace core
    {
        inline SmartPtr<ILogManager> ApplicationManagerMT::getLogManager() const
        {
            return m_logManager;
        }

        inline SmartPtr<IFactoryManager> ApplicationManagerMT::getFactoryManager() const
        {
            return m_factoryManager;
        }
    } // namespace core
}     // end namespace fb
