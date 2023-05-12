namespace fb
{
    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, BaseStateT, T, T );

    
    template <class T>
    BaseStateT<T>::BaseStateT()
    {
    }

    template <class T>
    BaseStateT<T>::~BaseStateT()
    {
    }
    
    template <class T>
    time_interval BaseStateT<T>::getTime() const
    {
        return m_time;
    }

    template <class T>
    void BaseStateT<T>::setTime( time_interval time )
    {
        m_time = time;
    }

    template <class T>
    bool BaseStateT<T>::isDirty() const
    {
        return m_dirty;
    }

    template <class T>
    void BaseStateT<T>::setDirty( bool dirty )
    {
        m_dirty = dirty;
    }

    template <class T>
    SmartPtr<IStateContext> BaseStateT<T>::getStateContext() const
    {
        return m_context.lock();
    }

    template <class T>
    void BaseStateT<T>::setStateContext( SmartPtr<IStateContext> context )
    {
        m_context = context;
    }

    template <class T>
    Thread::Task BaseStateT<T>::getTaskId() const
    {
        return m_taskId;
    }

    template <class T>
    void BaseStateT<T>::setTaskId( Thread::Task task )
    {
        m_taskId = task;
    }

    template <class T>
    SmartPtr<Properties> BaseStateT<T>::getProperties() const
    {
        auto properties = fb::make_ptr<Properties>();

        auto time = static_cast<f32>( m_time );
        properties->setProperty( "time", time );

        return properties;
    }

    template <class T>
    void BaseStateT<T>::setProperties( SmartPtr<Properties> properties )
    {
        auto time = 0.f;
        properties->getPropertyValue( "time", time );
        m_time = time;
    }

    template <class T>
    SmartPtr<ISharedObject> BaseStateT<T>::getOwner() const
    {
        return m_owner.lock();
    }

    template <class T>
    void BaseStateT<T>::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    template <class T>
    SmartPtr<IState> BaseStateT<T>::clone() const
    {
        FB_ASSERT( false );  // derived clone not implemented
        //auto state = fb::make_ptr<BaseStateT<T>>();
        return nullptr;
    }

    template <class T>
    void BaseStateT<T>::assign( SmartPtr<IState> state )
    {
    }
    
}  // end namespace fb
