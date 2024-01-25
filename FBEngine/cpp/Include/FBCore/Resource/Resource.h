#ifndef CResource_h__
#define CResource_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/System/Prototype.h>
#include <FBCore/Core/Properties.h>

namespace fb
{

    /** Resource class. A template class to provide the basic functionality of a resource. */
    template <class T>
    class Resource : public core::Prototype<T>
    {
    public:
        /** Constructor. */
        Resource();

        /** Destructor. */
        ~Resource() override;

        /** Unloads the resource.
         * @copydoc IResource::unload
         */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** Saves the resource to a file. */
        void saveToFile( const String &filePath );

        /** Loads the resource from a file. */
        void loadFromFile( const String &filePath );

        /** Saves the resource.
         * @copydoc IResource::save
         */
        void save() override;

        /** Imports the resource.
         * @copydoc IResource::import
         */
        void import() override;

        /** Reimport the resource. */
        void reimport() override;

        /** Gets the id of the resource in the system. */
        hash64 getFileSystemId() const override;

        /** Sets the id of the resource stored in the system. */
        void setFileSystemId( hash64 id ) override;

        /** Gets the file path. */
        String getFilePath() const;

        /** Sets the file path. */
        void setFilePath( const String &filePath );

        /** Gets the id of the resource settings in the system. */
        hash64 getSettingsFileSystemId() const override;

        /** Sets the id of the resource settings stored in the system. */
        void setSettingsFileSystemId( hash64 id ) override;

        SmartPtr<ISharedObject> toData() const override;
        void fromData( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IResource::getProperties */
        SmartPtr<Properties> getProperties() const override;

        /** @copydoc IResource::setProperties */
        void setProperties( SmartPtr<Properties> properties ) override;

        SmartPtr<IStateContext> getStateContext() const override;

        void setStateContext( SmartPtr<IStateContext> stateContext ) override;

        SmartPtr<IStateListener> getStateListener() const;

        void setStateListener( SmartPtr<IStateListener> stateListener );

        /** @copydoc IResource::getResourceType */
        virtual IResource::ResourceType getResourceType() const override;

        /** @copydoc IResource::setResourceType */
        virtual void setResourceType( IResource::ResourceType resourceType ) override;

        /** @copydoc IResource::getResourceTypeByName */
        virtual String getResourceTypeByName() const;

        /** @copydoc IResource::setResourceTypeByName */
        virtual void setResourceTypeByName( const String &resourceTypeName );

        /** @brief Gets the object. Return nullptr.
         * @copydoc IResource::_getObject
         */
        void _getObject( void **ppObject ) const override;

        /** @copydoc IResource::getDependencies */
        Array<SmartPtr<IResource>> getDependencies() const override;

        FB_CLASS_REGISTER_TEMPLATE_DECL( Resource, T );

    protected:
        SmartPtr<IStateContext> m_stateContext;
        SmartPtr<IStateListener> m_stateListener;
        atomic_s64 m_fileSystemId = 0;
        atomic_s64 m_settingsFileSystemId = 0;
        IResource::ResourceType m_resourceType = IResource::ResourceType::None;
        String m_filePath;

        mutable RecursiveMutex m_resourceMutex;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, Resource, T, core::Prototype<T> );

    template <class T>
    Resource<T>::Resource() = default;

    template <class T>
    Resource<T>::~Resource() = default;

    template <class T>
    void Resource<T>::unload( SmartPtr<ISharedObject> data )
    {
        const auto &loadingState = core::Prototype<T>::getLoadingState();
        if( loadingState != LoadingState::Unloaded )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto stateManager = applicationManager->getStateManager() )
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                    }

                    stateManager->removeStateObject( stateContext );

                    stateContext->unload( nullptr );
                    setStateContext( nullptr );
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
    void Resource<T>::saveToFile( const String &filePath )
    {
    }

    template <class T>
    void Resource<T>::loadFromFile( const String &filePath )
    {
    }

    template <class T>
    void Resource<T>::save()
    {
    }

    template <class T>
    void Resource<T>::import()
    {
    }

    template <class T>
    void Resource<T>::reimport()
    {
    }

    template <class T>
    hash64 Resource<T>::getFileSystemId() const
    {
        return m_fileSystemId;
    }

    template <class T>
    void Resource<T>::setFileSystemId( hash64 id )
    {
        m_fileSystemId = id;
    }

    template <class T>
    String Resource<T>::getFilePath() const
    {
        RecursiveMutex::ScopedLock lock( m_resourceMutex );
        return m_filePath;
    }

    template <class T>
    void Resource<T>::setFilePath( const String &filePath )
    {
        RecursiveMutex::ScopedLock lock( m_resourceMutex );
        m_filePath = filePath;
    }

    template <class T>
    hash64 Resource<T>::getSettingsFileSystemId() const
    {
        return m_settingsFileSystemId;
    }

    template <class T>
    void Resource<T>::setSettingsFileSystemId( hash64 id )
    {
        m_settingsFileSystemId = id;
    }

    template <class T>
    SmartPtr<ISharedObject> Resource<T>::toData() const
    {
        auto properties = getProperties();
        if( auto p = fb::dynamic_pointer_cast<ISharedObject>( properties ) )
        {
            return p;
        }

        return nullptr;
    }

    template <class T>
    void Resource<T>::fromData( SmartPtr<ISharedObject> data )
    {
        if( auto properties = fb::dynamic_pointer_cast<Properties>( data ) )
        {
            setProperties( properties );
        }
    }

    template <class T>
    SmartPtr<Properties> Resource<T>::getProperties() const
    {
        auto properties = core::Prototype<T>::getProperties();

        if( const auto handle = Resource<T>::getHandle() )
        {
            auto name = handle->getName();
            properties->setProperty( IResource::nameStr, name );
        }

        return properties;
    }

    template <class T>
    void Resource<T>::setProperties( SmartPtr<Properties> properties )
    {
        auto name = String();
        properties->getPropertyValue( IResource::nameStr, name );

        if( const auto handle = Resource<T>::getHandle() )
        {
            handle->setName( name );
        }
    }

    template <class T>
    SmartPtr<IStateContext> Resource<T>::getStateContext() const
    {
        return m_stateContext;
    }

    template <class T>
    void Resource<T>::setStateContext( SmartPtr<IStateContext> stateContext )
    {
        m_stateContext = stateContext;
    }

    template <class T>
    SmartPtr<IStateListener> Resource<T>::getStateListener() const
    {
        return m_stateListener;
    }

    template <class T>
    void Resource<T>::setStateListener( SmartPtr<IStateListener> stateListener )
    {
        m_stateListener = stateListener;
    }

    template <class T>
    IResource::ResourceType Resource<T>::getResourceType() const
    {
        return m_resourceType;
    }

    template <class T>
    void Resource<T>::setResourceType( IResource::ResourceType resourceType )
    {
        m_resourceType = resourceType;
    }

    template <class T>
    String Resource<T>::getResourceTypeByName() const
    {
        switch( getResourceType() )
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
        default:
            break;
        }

        return "";
    }

    template <class T>
    void Resource<T>::setResourceTypeByName( const String &resourceTypeName )
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
    void Resource<T>::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }

    template <class T>
    Array<SmartPtr<IResource>> Resource<T>::getDependencies() const
    {
        return Array<SmartPtr<IResource>>();
    }

}  // namespace fb

#endif  // CResource_h__
