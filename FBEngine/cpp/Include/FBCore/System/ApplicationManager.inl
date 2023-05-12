namespace fb
{
    namespace core
    {
        inline SmartPtr<ILogManager> ApplicationManager::getLogManager() const
        {
            return m_logManager;
        }

        inline SmartPtr<IFactoryManager> ApplicationManager::getFactoryManager() const
        {
            return m_factoryManager;
        }
    } // namespace core
}     // end namespace fb
