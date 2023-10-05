#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CMaterialManagerOgre.h>
#include <FBGraphicsOgre/Wrapper/CMaterialOgre.h>
#include <FBCore/FBCore.h>
#include <OgreMaterialManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CMaterialManagerOgre, IMaterialManager );

        CMaterialManagerOgre::CMaterialManagerOgre()
        {
            m_materials.reserve( 1024 );
        }

        CMaterialManagerOgre::~CMaterialManagerOgre()
        {
            unload( nullptr );
        }

        void CMaterialManagerOgre::load( SmartPtr<ISharedObject> data )
        {
        }

        void CMaterialManagerOgre::unload( SmartPtr<ISharedObject> data )
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

        SmartPtr<IMaterial> CMaterialManagerOgre::cloneMaterial( SmartPtr<IMaterial> material,
                                                             const String &clonedMaterialName )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto newMaterial = factoryManager->make_ptr<CMaterialOgre>();

                newMaterial->setParentPrototype( material );

                auto data = material->toData();
                newMaterial->fromData( data );

                if( auto handle = newMaterial->getHandle() )
                {
                    handle->setName( clonedMaterialName );
                }

                m_materials.push_back( newMaterial );

                return newMaterial;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IMaterial> CMaterialManagerOgre::cloneMaterial( const String &name,
                                                             const String &clonedMaterialName )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto materialResource = getByName( name );
            if( materialResource )
            {
                auto material = factoryManager->make_ptr<CMaterialOgre>();

                auto data = materialResource->toData();
                material->fromData( data );

                auto handle = material->getHandle();
                handle->setName( clonedMaterialName );

                m_materials.push_back( material );

                return material;
            }

            return nullptr;
        }

        SmartPtr<IResource> CMaterialManagerOgre::create( const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto material = factoryManager->make_ptr<CMaterialOgre>();
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

        SmartPtr<IResource> CMaterialManagerOgre::create( const String &uuid, const String &name )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto material = factoryManager->make_ptr<CMaterialOgre>();
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

        Pair<SmartPtr<IResource>, bool> CMaterialManagerOgre::createOrRetrieve( const String &uuid,
                                                                            const String &path,
                                                                            const String &type )
        {
            auto materialResource = getById( uuid );
            if( materialResource )
            {
                auto material = fb::static_pointer_cast<CMaterialOgre>( materialResource );
                return std::pair<SmartPtr<IResource>, bool>( material, false );
            }

            auto material = create( uuid, path );
            FB_ASSERT( material );

            return Pair<SmartPtr<IResource>, bool>( material, true );
        }

        Pair<SmartPtr<IResource>, bool> CMaterialManagerOgre::createOrRetrieve( const String &path )
        {
            auto materialResource = getByName( path );
            if( materialResource )
            {
                auto material = fb::static_pointer_cast<CMaterialOgre>( materialResource );
                return std::pair<SmartPtr<IResource>, bool>( material, false );
            }

            auto material = create( path );
            FB_ASSERT( material );

            return Pair<SmartPtr<IResource>, bool>( material, true );
        }

        void CMaterialManagerOgre::saveToFile( const String &filePath, SmartPtr<IResource> resource )
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

        SmartPtr<IResource> CMaterialManagerOgre::loadFromFile( const String &filePath )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

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

                        auto materialData = fb::make_ptr<Properties>();
                        DataUtil::parse( materialStr, materialData.get() );

                        auto material = create( materialName );

                        FileInfo fileInfo;
                        if( fileSystem->findFileInfo( filePath, fileInfo ) )
                        {
                            auto fileId = fileInfo.fileId;
                            material->setFileSystemId( fileId );
                        }

                        material->fromData( materialData );
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

        SmartPtr<IResource> CMaterialManagerOgre::load( const String &name )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                auto materialResource = getByName( name );
                if( materialResource )
                {
                    auto material = fb::static_pointer_cast<CMaterialOgre>( materialResource );
                    material->load( nullptr );
                    return material;
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IResource> CMaterialManagerOgre::getByName( const String &name )
        {
            try
            {
                FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

                for( auto &material : m_materials )
                {
                    auto handle = material->getHandle();
                    if( handle->getName() == name )
                    {
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

        SmartPtr<IResource> CMaterialManagerOgre::getById( const String &uuid )
        {
            try
            {
                for( auto &material : m_materials )
                {
                    auto handle = material->getHandle();
                    if( handle->getUUID() == uuid )
                    {
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

        void CMaterialManagerOgre::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }
    }  // end namespace render
}  // end namespace fb
