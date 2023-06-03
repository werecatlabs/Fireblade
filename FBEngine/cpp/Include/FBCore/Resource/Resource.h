#ifndef CResource_h__
#define CResource_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/System/Prototype.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Properties.h>

namespace fb
{
    template <class T>
    class Resource : public core::Prototype<T>
    {
    public:
        Resource();
        ~Resource() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        void saveToFile( const String &filePath );

        void loadFromFile( const String &filePath );

        /** Saves the resource. */
        void save() override;

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

        /** @copydoc IResource::getProperties */
        SmartPtr<Properties> getProperties() const override;

        /** @copydoc IResource::setProperties */
        void setProperties( SmartPtr<Properties> properties ) override;

        SmartPtr<IStateContext> getStateObject() const override;

        void setStateObject( SmartPtr<IStateContext> stateObject ) override;

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

        void _getObject( void **ppObject ) const override;

        FB_CLASS_REGISTER_TEMPLATE_DECL( Resource, T );

    protected:
        AtomicSmartPtr<IStateContext> m_stateObject;
        AtomicSmartPtr<IStateListener> m_stateListener;
        hash64 m_fileSystemId = 0;
        hash64 m_settingsFileSystemId = 0;
        IResource::ResourceType m_resourceType = IResource::ResourceType::None;
        String m_filePath;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, Resource, T, core::Prototype<T> );

    template <class T>
    Resource<T>::Resource()
    {
    }

    template <class T>
    Resource<T>::~Resource()
    {
        unload( nullptr );
    }

    template <class T>
    void Resource<T>::unload( SmartPtr<ISharedObject> data )
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
        return m_filePath;
    }

    template <class T>
    void Resource<T>::setFilePath( const String &filePath )
    {
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
    SmartPtr<Properties> Resource<T>::getProperties() const
    {
        auto properties = core::Prototype<T>::getProperties();

        const auto handle = this->getHandle();
        properties->setProperty( "name", handle->getName() );
        return properties;
    }

    template <class T>
    void Resource<T>::setProperties( SmartPtr<Properties> properties )
    {
        auto name = String();
        properties->getProperty( "name", name );

        auto handle = this->getHandle();
        handle->setName( name );
    }

    template <class T>
    SmartPtr<IStateContext> Resource<T>::getStateObject() const
    {
        return m_stateObject;
    }

    template <class T>
    void Resource<T>::setStateObject( SmartPtr<IStateContext> stateObject )
    {
        m_stateObject = stateObject;
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

}  // namespace fb

#endif  // CResource_h__
