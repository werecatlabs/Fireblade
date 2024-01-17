#include <FBCore/FBCorePCH.h>
#include <FBCore/Database/AssetDatabaseManager.h>
#include <FBCore/Interface/Database/IDatabase.h>
#include <FBCore/Interface/Database/IDatabaseQuery.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Memory/IData.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/IComponent.h>
#include <FBCore/Scene/Director.h>
#include <FBCore/Core/FileInfo.h>
#include <FBCore/Core/LogManager.h>

#include "FBCore/Scene/Directors/ResourceDirector.h"

namespace fb
{

    AssetDatabaseManager::AssetDatabaseManager() = default;

    AssetDatabaseManager::~AssetDatabaseManager()
    {
        unload( nullptr );
    }

    void AssetDatabaseManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            DatabaseManager::load( data );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void AssetDatabaseManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( m_database )
            {
                m_database->close();
                m_database = nullptr;
            }

            DatabaseManager::unload( data );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void AssetDatabaseManager::clearDatabase()
    {
        try
        {
            auto dropTableSql = "DROP TABLE `resources`";
            executeQuery( dropTableSql );

            auto createTableSql =
                "CREATE TABLE 'resources'( id INTEGER PRIMARY KEY UNIQUE, uuid VARCHAR, path VARCHAR, "
                "type "
                "VARCHAR );";
            executeQuery( createTableSql );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto AssetDatabaseManager::hasResourceEntry( SmartPtr<ISharedObject> object ) -> bool
    {
        FB_ASSERT( object );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto handle = object->getHandle();
        FB_ASSERT( handle );

        auto filePath = String( "" );
        if( object->isDerived<IResource>() )
        {
            auto resource = fb::static_pointer_cast<IResource>( object );
            auto fileSystemId = resource->getFileSystemId();

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( fileSystemId, fileInfo ) )
            {
                filePath = fileInfo.filePath;
            }

            if( StringUtil::isNullOrEmpty( filePath ) )
            {
                filePath = resource->getFilePath();
            }
        }
        else
        {
            filePath = String( "scene" );
        }

        if( object->isDerived<scene::IActor>() || object->isDerived<scene::IComponent>() )
        {
            auto uuidSql = String( "select * from resources where uuid = '" ) + handle->getUUID() + "'";
            auto uuidQuery = executeQuery( uuidSql );
            if( uuidQuery )
            {
                if( !uuidQuery->eof() )
                {
                    return true;
                }
            }
        }
        else
        {
            auto uuidSql = String( "select * from resources where uuid = '" ) + handle->getUUID() + "'";
            auto uuidQuery = executeQuery( uuidSql );
            if( uuidQuery )
            {
                if( !uuidQuery->eof() )
                {
                    return true;
                }
            }

            auto sql = String( "select * from resources where path = '" ) + filePath + "'";
            auto query = executeQuery( sql );
            if( query )
            {
                if( !query->eof() )
                {
                    return true;
                }
            }
        }

        return false;
    }

    void AssetDatabaseManager::addResourceEntry( SmartPtr<ISharedObject> object )
    {
        try
        {
            if( !object )
            {
                return;
            }

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto handle = object->getHandle();
            FB_ASSERT( handle );

            auto uuid = handle->getUUID();
            if( StringUtil::isNullOrEmpty( uuid ) )
            {
                uuid = StringUtil::getUUID();
                handle->setUUID( uuid );
            }

            //FB_ASSERT( hasResourceById( uuid ) == false );

            auto filePath = String( "" );
            if( object->isDerived<IResource>() )
            {
                auto resource = fb::static_pointer_cast<IResource>( object );
                auto fileSystemId = resource->getFileSystemId();

                FileInfo fileInfo;
                if( fileSystem->findFileInfo( fileSystemId, fileInfo ) )
                {
                    filePath = fileInfo.filePath.c_str();
                }
            }
            else
            {
                filePath = String( "scene" );
            }

            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            auto typeInfo = object->getTypeInfo();
            auto dataStr = typeManager->getName( typeInfo );

            if( object->isDerived<scene::IActor>() )
            {
                dataStr = "Actor";
            }
            else if( object->isDerived<render::IMaterial>() )
            {
                dataStr = "Material";
            }
            else if( object->isDerived<render::ITexture>() )
            {
                dataStr = "Texture";
            }

            auto sql = "INSERT INTO `resources`(`id`,`uuid`,`path`,`type`) VALUES (NULL, '" + uuid +
                       "', '" + filePath + "', '" + dataStr + "')";
            executeQuery( sql );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void AssetDatabaseManager::updateResourceEntry( SmartPtr<ISharedObject> object )
    {
        FB_ASSERT( object );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto handle = object->getHandle();
        FB_ASSERT( handle );

        auto uuid = handle->getUUID();
        FB_ASSERT( !StringUtil::isNullOrEmpty( uuid ) );

        auto filePath = String( "" );
        if( object->isDerived<IResource>() )
        {
            auto resource = fb::static_pointer_cast<IResource>( object );
            auto fileSystemId = resource->getFileSystemId();

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( fileSystemId, fileInfo ) )
            {
                filePath = fileInfo.filePath.c_str();
            }
        }
        else
        {
            filePath = String( "scene" );
        }

        auto sql =
            String( "select * from resources where uuid = '" ) + uuid + "' or path = '" + filePath + "'";
        auto query = executeQuery( sql );
        if( query )
        {
            if( !query->eof() )
            {
                auto updateSql = String( "UPDATE `resources` SET `path`='" ) + filePath +
                                 String( "' WHERE `uuid`='" ) + uuid + String( "'" );
                executeQuery( updateSql );
            }
        }
    }

    void AssetDatabaseManager::removeResourceEntry( SmartPtr<ISharedObject> object )
    {
        FB_ASSERT( object );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto handle = object->getHandle();
        FB_ASSERT( handle );

        auto filePath = String( "" );
        if( object->isDerived<IResource>() )
        {
            auto resource = fb::static_pointer_cast<IResource>( object );
            auto fileSystemId = resource->getFileSystemId();

            FileInfo fileInfo;
            if( fileSystem->findFileInfo( fileSystemId, fileInfo ) )
            {
                filePath = fileInfo.filePath.c_str();
            }
        }
        else
        {
            filePath = String( "scene" );
        }

        auto sql = String( "select * from resources where path = '" ) + filePath + "'";
        auto query = executeQuery( sql );
        if( query )
        {
            if( !query->eof() )
            {
                auto deleteSql = String( "DELETE FROM my_table WHERE path = '" ) + filePath + "'";
                executeQuery( deleteSql );
            }
        }
    }

    void AssetDatabaseManager::removeResourceEntryFromPath( const String &path )
    {
        auto sql = String( "select * from resources where path = '" ) + path + "'";
        auto query = executeQuery( sql );
        if( query )
        {
            if( !query->eof() )
            {
                auto deleteSql = String( "DELETE FROM resources WHERE path = '" ) + path + "'";
                executeQuery( deleteSql );
            }
        }
    }

    auto AssetDatabaseManager::hasResourceById( const String &uuid ) -> bool
    {
        auto sql = static_cast<String>( "select * from 'resources' where uuid='" + uuid + "'" );
        auto query = executeQuery( sql );
        if( query )
        {
            if( !query->eof() )
            {
                return true;
            }
        }

        return false;
    }

    SmartPtr<scene::IDirector> AssetDatabaseManager::getResourceEntry( const String &uuid )
    {
        auto sql = String( "select * from resources where uuid = '" ) + uuid + String( "'" );
        auto query = executeQuery( sql );

        auto idStr = query->getFieldValue( "id" );
        auto uuidStr = query->getFieldValue( "uuid" );
        auto pathStr = query->getFieldValue( "path" );
        auto typeStr = query->getFieldValue( "type" );

        auto director = fb::make_ptr<scene::ResourceDirector>();
        director->setResourcePath( pathStr );
        director->setResourceUUID( uuidStr );
        return director;
    }

    SmartPtr<scene::IDirector> AssetDatabaseManager::getResourceEntryFromPath( const String &path )
    {
        auto sql = String( "select * from resources where path = '" ) + path + String( "'" );
        auto query = executeQuery( sql );
        if( query )
        {
            auto idStr = query->getFieldValue( "id" );
            auto uuidStr = query->getFieldValue( "uuid" );
            auto pathStr = query->getFieldValue( "path" );
            auto typeStr = query->getFieldValue( "type" );

            auto director = fb::make_ptr<scene::ResourceDirector>();
            director->setResourcePath( pathStr );
            director->setResourceUUID( uuidStr );
            return director;
        }

        return nullptr;
    }
}  // end namespace fb
