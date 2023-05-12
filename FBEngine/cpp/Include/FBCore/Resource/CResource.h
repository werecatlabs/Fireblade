#ifndef CResource_h__
#define CResource_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/System/Prototype.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/Properties.h>

namespace fb
{
    template <class T>
    class CResource : public core::Prototype<T>
    {
    public:
        CResource();
        ~CResource() override;

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

        FB_CLASS_REGISTER_TEMPLATE_DECL( CResource, T );

    protected:
        AtomicSmartPtr<IStateContext> m_stateObject;
        AtomicSmartPtr<IStateListener> m_stateListener;
        hash64 m_fileSystemId = 0;
        hash64 m_settingsFileSystemId = 0;
        IResource::ResourceType m_resourceType = IResource::ResourceType::None;
        String m_filePath;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CResource, T, core::Prototype<T> );

}  // namespace fb

#include <FBCore/Resource/CResource.inl>

#endif  // CResource_h__
