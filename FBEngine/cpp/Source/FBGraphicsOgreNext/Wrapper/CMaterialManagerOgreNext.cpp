#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialOgreNext.h>
#include <FBCore/FBCore.h>
#include <OgreMaterialManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialManagerOgreNext, CMaterialManager );

        CMaterialManagerOgreNext::CMaterialManagerOgreNext()
        {
            m_materials.reserve( 1024 );
        }

        CMaterialManagerOgreNext::~CMaterialManagerOgreNext()
        {
            unload( nullptr );
        }

        void CMaterialManagerOgreNext::load( SmartPtr<ISharedObject> data )
        {
        }

        void CMaterialManagerOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                for( auto &material : m_materials )
                {
                    if( material )
                    {
                        material->unload( nullptr );
                    }
                }

                m_materials.clear();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IMaterial> CMaterialManagerOgreNext::cloneMaterial( SmartPtr<IMaterial> material,
                                                             const String &clonedMaterialName )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto newMaterial = factoryManager->make_ptr<CMaterialOgreNext>();

            newMaterial->setParentPrototype( material );

            auto data = material->toData();
            newMaterial->fromData( data );

            auto handle = newMaterial->getHandle();
            handle->setName( clonedMaterialName );

            newMaterial->load( nullptr );

            m_materials.push_back( newMaterial );

            return newMaterial;
        }

        SmartPtr<IMaterial> CMaterialManagerOgreNext::cloneMaterial( const String &name,
                                                             const String &clonedMaterialName )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto materialResource = getByName( name );
            if( materialResource )
            {
                auto material = factoryManager->make_ptr<CMaterialOgreNext>();

                auto data = materialResource->toData();
                material->fromData( data );

                auto handle = material->getHandle();
                handle->setName( name );

                m_materials.push_back( material );

                return material;
            }

            return nullptr;
        }

        SmartPtr<IResource> CMaterialManagerOgreNext::create( const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto material = factoryManager->make_ptr<CMaterialOgreNext>();
            FB_ASSERT( material );

            FB_ASSERT( material->getHandle() );
            if( auto handle = material->getHandle() )
            {
                handle->setName( name );

                auto uuid = StringUtil::getUUID();
                handle->setUUID( uuid );
            }

            material->setFilePath( name );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( name, fileInfo ) )
            {
                material->setFileSystemId( fileInfo.fileId );
            }

            m_materials.push_back( material );

            return material;
        }

        SmartPtr<IResource> CMaterialManagerOgreNext::create( const String &uuid, const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto material = factoryManager->make_ptr<CMaterialOgreNext>();
            FB_ASSERT( material );

            FB_ASSERT( material->getHandle() );

            if( auto handle = material->getHandle() )
            {
                handle->setName( name );
                handle->setUUID( uuid );
            }

            material->setFilePath( name );

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( name, fileInfo ) )
            {
                material->setFileSystemId( fileInfo.fileId );
            }

            m_materials.push_back( material );

            return material;
        }

        void CMaterialManagerOgreNext::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

    }  // end namespace render
}  // end namespace fb
