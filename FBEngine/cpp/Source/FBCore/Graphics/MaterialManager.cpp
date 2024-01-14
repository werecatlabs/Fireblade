#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/MaterialManager.h>
#include <FBCore/Graphics/Material.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/FileInfo.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/Properties.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, MaterialManager, IMaterialManager );

    MaterialManager::MaterialManager() = default;

    MaterialManager::~MaterialManager()
    {
        unload( nullptr );
    }

    void MaterialManager::load( SmartPtr<ISharedObject> data )
    {
        m_materials.reserve( 1024 );
    }

    void MaterialManager::unload( SmartPtr<ISharedObject> data )
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

    auto MaterialManager::cloneMaterial( SmartPtr<IMaterial> material, const String &clonedMaterialName )
        -> SmartPtr<IMaterial>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto newMaterial = factoryManager->make_ptr<Material>();

        newMaterial->setParentPrototype( material );

        auto data = material->toData();
        newMaterial->fromData( data );

        auto handle = newMaterial->getHandle();
        handle->setName( clonedMaterialName );

        newMaterial->load( nullptr );

        m_materials.emplace_back( newMaterial );

        return newMaterial;
    }

    auto MaterialManager::cloneMaterial( const String &name, const String &clonedMaterialName )
        -> SmartPtr<IMaterial>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto materialResource = getByName( name );
        if( materialResource )
        {
            auto material = factoryManager->make_ptr<Material>();

            auto data = materialResource->toData();
            material->fromData( data );

            auto handle = material->getHandle();
            handle->setName( name );

            m_materials.emplace_back( material );

            return material;
        }

        return nullptr;
    }

    auto MaterialManager::create( const String &name ) -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto material = factoryManager->make_ptr<Material>();
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

        m_materials.emplace_back( material );

        return material;
    }

    auto MaterialManager::create( const String &uuid, const String &name ) -> SmartPtr<IResource>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto material = factoryManager->make_ptr<Material>();
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

        m_materials.emplace_back( material );

        return material;
    }

    auto MaterialManager::createOrRetrieve( const String &uuid, const String &path, const String &type )
        -> Pair<SmartPtr<IResource>, bool>
    {
        if( auto materialResource = getById( uuid ) )
        {
            return std::pair<SmartPtr<IResource>, bool>( materialResource, false );
        }

        if( !StringUtil::isNullOrEmpty( uuid ) )
        {
            auto material = create( uuid, path );
            FB_ASSERT( material );

            return Pair<SmartPtr<IResource>, bool>( material, true );
        }

        auto material = create( path );
        FB_ASSERT( material );

        return Pair<SmartPtr<IResource>, bool>( material, true );
    }

    auto MaterialManager::createOrRetrieve( const String &path ) -> Pair<SmartPtr<IResource>, bool>
    {
        if( auto materialResource = getByName( path ) )
        {
            return std::pair<SmartPtr<IResource>, bool>( materialResource, false );
        }

        auto material = create( path );
        FB_ASSERT( material );

        return Pair<SmartPtr<IResource>, bool>( material, true );
    }

    void MaterialManager::saveToFile( const String &filePath, SmartPtr<IResource> resource )
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );
            FB_ASSERT( resource );

            auto applicationManager = core::ApplicationManager::instance();
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

    auto MaterialManager::loadFromFile( const String &filePath ) -> SmartPtr<IResource>
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

            auto applicationManager = core::ApplicationManager::instance();
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

                    auto materialData = fb::make_ptr<Properties>();
                    DataUtil::parse( materialStr, materialData.get() );

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

                    m_materials.emplace_back( material );

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

    auto MaterialManager::loadResource( const String &name ) -> SmartPtr<IResource>
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

    auto MaterialManager::getByName( const String &name ) -> SmartPtr<IResource>
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

            for( auto &material : m_materials )
            {
                if( material )
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
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto MaterialManager::getById( const String &uuid ) -> SmartPtr<IResource>
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

    void MaterialManager::_getObject( void **ppObject ) const
    {
        *ppObject = nullptr;
    }

}  // namespace fb::render
