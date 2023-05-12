namespace fb
{

    template <class T>
    CResource<T>::CResource()
    {
    }

    template <class T>
    CResource<T>::~CResource()
    {
        unload( nullptr );
    }

    template <class T>
    void CResource<T>::unload( SmartPtr<ISharedObject> data )
    {
        const auto &loadingState = core::Prototype<T>::getLoadingState();
        if( loadingState != LoadingState::Unloaded )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto stateManager = applicationManager->getStateManager() )
            {
                if( auto stateObject = getStateObject() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateObject->removeStateListener( stateListener );
                    }

                    stateManager->removeStateObject( stateObject );

                    stateObject->unload( nullptr );
                    setStateObject( nullptr );
                }

                if( auto stateListener = getStateListener() )
                {
                    stateListener->unload( nullptr );
                    setStateListener( nullptr );
                }
            }
        }
    }

    template <class T>
    void CResource<T>::saveToFile( const String &filePath )
    {
    }

    template <class T>
    void CResource<T>::loadFromFile( const String &filePath )
    {
    }

    template <class T>
    void CResource<T>::save()
    {
    }

    template <class T>
    void CResource<T>::import()
    {
    }

    template <class T>
    void CResource<T>::reimport()
    {
    }

    template <class T>
    hash64 CResource<T>::getFileSystemId() const
    {
        return m_fileSystemId;
    }

    template <class T>
    void CResource<T>::setFileSystemId( hash64 id )
    {
        m_fileSystemId = id;
    }

    template <class T>
    String CResource<T>::getFilePath() const
    {
        return m_filePath;
    }

    template <class T>
    void CResource<T>::setFilePath( const String &filePath )
    {
        m_filePath = filePath;
    }

    template <class T>
    hash64 CResource<T>::getSettingsFileSystemId() const
    {
        return m_settingsFileSystemId;
    }

    template <class T>
    void CResource<T>::setSettingsFileSystemId( hash64 id )
    {
        m_settingsFileSystemId = id;
    }

    template <class T>
    SmartPtr<Properties> CResource<T>::getProperties() const
    {
        auto properties = core::Prototype<T>::getProperties();

        const auto handle = this->getHandle();
        properties->setProperty( "name", handle->getName() );
        return properties;
    }

    template <class T>
    void CResource<T>::setProperties( SmartPtr<Properties> properties )
    {
        auto name = String();
        properties->getProperty( "name", name );

        auto handle = this->getHandle();
        handle->setName( name );
    }

    template <class T>
    SmartPtr<IStateContext> CResource<T>::getStateObject() const
    {
        return m_stateObject;
    }

    template <class T>
    void CResource<T>::setStateObject( SmartPtr<IStateContext> stateObject )
    {
        m_stateObject = stateObject;
    }

    template <class T>
    SmartPtr<IStateListener> CResource<T>::getStateListener() const
    {
        return m_stateListener;
    }

    template <class T>
    void CResource<T>::setStateListener( SmartPtr<IStateListener> stateListener )
    {
        m_stateListener = stateListener;
    }

    template <class T>
    IResource::ResourceType CResource<T>::getResourceType() const
    {
        return m_resourceType;
    }

    template <class T>
    void CResource<T>::setResourceType( IResource::ResourceType resourceType )
    {
        m_resourceType = resourceType;
    }

    template <class T>
    String CResource<T>::getResourceTypeByName() const
    {
        auto type = getResourceType();
        switch( type )
        {
        case IResource::ResourceType::None:
            return "None";
        case IResource::ResourceType::Material:
            return "Material";
        case IResource::ResourceType::Object:
            return "Object";
        case IResource::ResourceType::Actor:
            return "Actor";
        case IResource::ResourceType::Component:
            return "Component";
        case IResource::ResourceType::Texture:
            return "Texture";
        }

        return "";
    }

    template <class T>
    void CResource<T>::setResourceTypeByName( const String &resourceTypeName )
    {
        if( resourceTypeName == "None" )
        {
            setResourceType( IResource::ResourceType::None );
        }
        else if( resourceTypeName == "Material" )
        {
            setResourceType( IResource::ResourceType::Material );
        }
        else if( resourceTypeName == "Object" )
        {
            setResourceType( IResource::ResourceType::Object );
        }
        else if( resourceTypeName == "Actor" )
        {
            setResourceType( IResource::ResourceType::Actor );
        }
        else if( resourceTypeName == "Component" )
        {
            setResourceType( IResource::ResourceType::Component );
        }
        else if( resourceTypeName == "Texture" )
        {
            setResourceType( IResource::ResourceType::Texture );
        }
        else
        {
            setResourceType( IResource::ResourceType::None );
        }
    }

    template <class T>
    void CResource<T>::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }

}  // namespace fb
