#include <FBGraphics/Wrapper/CMaterialManager.h>
#include <FBGraphics/Wrapper/CMaterial.h>
#include <FBCore/FBCore.h>
#include <OgreMaterialManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialManager, IMaterialManager );

        CMaterialManager::CMaterialManager()
        {
            m_materials.reserve( 1024 );
        }

        CMaterialManager::~CMaterialManager()
        {
            unload( nullptr );
        }

        void CMaterialManager::load( SmartPtr<ISharedObject> data )
        {
        }

        void CMaterialManager::unload( SmartPtr<ISharedObject> data )
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

        SmartPtr<IMaterial> CMaterialManager::cloneMaterial( SmartPtr<IMaterial> material,
                                                             const String &clonedMaterialName )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto newMaterial = factoryManager->make_ptr<CMaterial>();

            newMaterial->setParentPrototype( material );

            auto data = material->toData();
            newMaterial->fromData( data );

            auto handle = newMaterial->getHandle();
            handle->setName( clonedMaterialName );

            newMaterial->load( nullptr );

            m_materials.push_back( newMaterial );

            return newMaterial;
        }

        SmartPtr<IMaterial> CMaterialManager::cloneMaterial( const String &name,
                                                             const String &clonedMaterialName )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto materialResource = getByName( name );
            if( materialResource )
            {
                auto material = factoryManager->make_ptr<CMaterial>();

                auto data = materialResource->toData();
                material->fromData( data );

                auto handle = material->getHandle();
                handle->setName( name );

                m_materials.push_back( material );

                return material;
            }

            return nullptr;
        }

        SmartPtr<IResource> CMaterialManager::create( const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto material = factoryManager->make_ptr<CMaterial>();
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

        SmartPtr<IResource> CMaterialManager::create( const String &uuid, const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto material = factoryManager->make_ptr<CMaterial>();
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

        Pair<SmartPtr<IResource>, bool> CMaterialManager::createOrRetrieve( const String &uuid,
                                                                            const String &path,
                                                                            const String &type )
        {
            if( auto materialResource = getById( uuid ) )
            {
                return std::pair<SmartPtr<IResource>, bool>( materialResource, false );
            }

            auto material = create( uuid, path );
            FB_ASSERT( material );

            return Pair<SmartPtr<IResource>, bool>( material, true );
        }

        Pair<SmartPtr<IResource>, bool> CMaterialManager::createOrRetrieve( const String &path )
        {
            if( auto materialResource = getByName( path ) )
            {
                return std::pair<SmartPtr<IResource>, bool>( materialResource, false );
            }

            auto material = create( path );
            FB_ASSERT( material );

            return Pair<SmartPtr<IResource>, bool>( material, true );
        }

        void CMaterialManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );
                FB_ASSERT( resource );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto material = fb::static_pointer_cast<IMaterial>( resource );
                auto pData = fb::static_pointer_cast<Properties>( material->toData() );

                auto materialStr = DataUtil::toString( pData.get(), true );
                fileSystem->writeAllText( filePath, materialStr );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IResource> CMaterialManager::loadFromFile( const String &filePath )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto materialName = Path::getFileNameWithoutExtension( filePath );

                for( auto material : m_materials )
                {
                    auto handle = material->getHandle();
                    auto currentMaterialName = handle->getName();

                    if( materialName == currentMaterialName )
                    {
                        FileInfo fileInfo;
                        if( fileSystem->findFileInfo( filePath, fileInfo ) )
                        {
                            auto currentMaterialPath = String( fileInfo.filePath.c_str() );
                            auto currentFileId = material->getFileSystemId();

                            if( filePath == currentMaterialPath && fileInfo.fileId == currentFileId )
                            {
                                return material;
                            }
                        }
                    }
                }

                auto ext = Path::getFileExtension( filePath );
                if( ext == ".mat" )
                {
                    auto stream = fileSystem->open( filePath, true, false, false, false, false );
                    if( !stream )
                    {
                        stream = fileSystem->open( filePath, true, false, false, true, true );
                    }

                    if( stream )
                    {
                        auto materialStr = stream->getAsString();

                        auto materialData = DataUtil::parseJson( materialStr );

                        auto material = create( materialName );

                        FileInfo fileInfo;
                        if( fileSystem->findFileInfo( filePath, fileInfo ) )
                        {
                            auto fileId = fileInfo.fileId;
                            material->setFileSystemId( fileId );
                        }

                        material->setFilePath( filePath );

                        material->fromData( materialData );

                        //graphicsSystem->loadObject( material );
                        material->load( nullptr );

                        m_materials.push_back( material );

                        return material;
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CMaterialManager::load( const String &name )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                if( auto materialResource = getByName( name ) )
                {
                    materialResource->load( nullptr );
                    return materialResource;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CMaterialManager::getByName( const String &name )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                for( auto &material : m_materials )
                {
                    if( auto handle = material->getHandle() )
                    {
                        if( handle->getName() == name )
                        {
                            return material;
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CMaterialManager::getById( const String &uuid )
        {
            try
            {
                for( auto &material : m_materials )
                {
                    if( auto handle = material->getHandle() )
                    {
                        if( handle->getUUID() == uuid )
                        {
                            return material;
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void CMaterialManager::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

    }  // end namespace render
}  // end namespace fb
