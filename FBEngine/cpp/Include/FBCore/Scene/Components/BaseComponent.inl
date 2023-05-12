namespace fb
{
    namespace scene
    {

        template <class T>
        T *BaseComponent::getDataPtrByType() const
        {
            return static_cast<T *>( m_dataPtr );
        }

    }  // namespace scene
}  // end namespace fb
