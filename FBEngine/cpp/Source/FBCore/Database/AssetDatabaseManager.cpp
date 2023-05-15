#include <FBCore/FBCorePCH.h>
#include <FBCore/Database/AssetDatabaseManager.h>
#include <FBCore/FBCore.h>

namespace fb
{

    AssetDatabaseManager::AssetDatabaseManager()
    {
        try
        {
            // CppSQLite3DB* db = new CppSQLite3DB;
            // m_database = db;

            // if (sqlite3_threadsafe() == 0)
            //{
            //	FB_LOG_ERROR("Error: sqlite compiled without thread safety.");
            // }

#if FB_USE_DB_CUSTOM_ALLOCATOR
            sqlite3_mem_methods my_malloc_implementation;
            memset( &my_malloc_implementation, 0, sizeof( sqlite3_mem_methods ) );
            my_malloc_implementation.xMalloc = xMalloc;
            my_malloc_implementation.xFree = xFree;
            my_malloc_implementation.xRealloc = xRealloc;
            my_malloc_implementation.xSize = xSize;
            my_malloc_implementation.xRoundup = xRoundup;
            my_malloc_implementation.xInit = xInit;
            my_malloc_implementation.xShutdown = xShutdown;
            sqlite3_config( SQLITE_CONFIG_MALLOC, &my_malloc_implementation );
#endif

            // const int KBuferSize = 128;
            // const int KSize = 10 * 1024;
            // const int KBufferCount = 8;
            // static u3264_t sqliteScratchBuffer[KSize*KBuferSize / sizeof(u3264_t)];

            // sqlite3_config(
            //	SQLITE_CONFIG_SCRATCH,
            //	&sqliteScratchBuffer[0],
            //	KSize,
            //	KBufferCount);

            //// Max DB page size is 16KB. SQLite doc says to add 40 bytes for page header.
            // const int KPageSize = 16 * 1024 + 40;
            // const int KPageCount = 512;
            // static u3264_t sqlitePageCache[KPageSize*KPageCount / sizeof(u3264_t)];
            // sqlite3_config(SQLITE_CONFIG_PAGECACHE, sqlitePageCache, KPageSize, KPageCount);
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    AssetDatabaseManager::~AssetDatabaseManager()
    {
        unload( nullptr );
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
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void AssetDatabaseManager::setAssetDataFromPath( const String &path, SmartPtr<IData> data )
    {
        //auto importerData = data->getDataAsType<data::importer_data>();
        //auto dataStr = String( "" );

        //if( importerData->importerType == "mesh" )
        //{
        //    auto meshData = data->getDataAsType<data::mesh_importer_data>();
        //    dataStr = DataUtil::toString( meshData );
        //}
        //else
        //{
        //    dataStr = DataUtil::toString( importerData );
        //}

        //auto sql = String( "UPDATE `assets` SET `data`='" ) + dataStr + String( "' WHERE `name`='" ) +
        //           path + String( "'" );
        //executeQuery( sql );
    }

    SmartPtr<IData> AssetDatabaseManager::getAssetDataFromPath( const String &path )
    {
        auto sql = String( "select * from assets where name = '" ) + path + String( "'" );
        auto query = executeQuery( sql );
        auto dataStr = query->getFieldValue( "data" );

        //data::importer_data importerData;
        //DataUtil::parse( dataStr, &importerData );

        //if( importerData.importerType == "mesh" )
        //{
        //    data::mesh_importer_data meshImporterData;
        //    DataUtil::parse( dataStr, &meshImporterData );

        //    auto pData = fb::make_ptr<Data<data::mesh_importer_data>>();
        //    pData->setData( &meshImporterData );
        //    return pData;
        //}

        //auto pData = fb::make_ptr<Data<data::importer_data>>();
        //pData->setData( &importerData );
        //return pData;

        return nullptr;
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

    bool AssetDatabaseManager::hasResourceEntry( SmartPtr<ISharedObject> object )
    {
        FB_ASSERT( object );

        auto applicationManager = core::IApplicationManager::instance();
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

        if( object->isDerived<scene::IActor>() || 
            object->isDerived<scene::IComponent>())
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
            FB_ASSERT( object );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto handle = object->getHandle();
            FB_ASSERT( handle );

            auto uuid = handle->getUUID();
            if( StringUtil::isNullOrEmpty( handle->getUUID() ) )
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

    void AssetDatabaseManager::removeResourceEntry( SmartPtr<ISharedObject> object )
    {
        FB_ASSERT( object );

        auto applicationManager = core::IApplicationManager::instance();
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

    SmartPtr<IResource> AssetDatabaseManager::loadResourceById( const String &uuid )
    {
        return nullptr;
    }

    SmartPtr<IResource> AssetDatabaseManager::loadResource( const String &filePath )
    {
        return nullptr;
    }

    bool AssetDatabaseManager::hasResourceById( const String &uuid )
    {
        auto sql = String( "select * from 'resources' where uuid='" + uuid + "'" );
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
}  // end namespace fb
