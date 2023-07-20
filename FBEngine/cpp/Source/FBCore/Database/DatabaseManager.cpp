#include <FBCore/FBCorePCH.h>
#include <FBCore/Database/DatabaseManager.h>
#include <FBCore/FBCore.h>

namespace fb
{

    DatabaseManager::DatabaseManager()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            m_database = factoryManager->make_object<IDatabase>();
            FB_ASSERT( m_database );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    DatabaseManager::~DatabaseManager()
    {
        DatabaseManager::unload( nullptr );
    }

    void DatabaseManager::open()
    {
        try
        {
            if( m_database )
            {
                m_database->loadFromFile( m_databasePath );
            }

            for( auto db : m_attached )
            {
                attach( db );
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void DatabaseManager::close()
    {
        try
        {
            if( m_database )
            {
                m_database->close();
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void DatabaseManager::loadFromHandle( size_t handle )
    {
        // m_database->mpDB = reinterpret_cast<sqlite3*>(handle);
        // m_database->setBusyTimeout(60000);
    }

    void DatabaseManager::loadFromFile( const String &filePath )
    {
        try
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );
            FB_LOG( "Database::load: " + filePath );

            m_databasePath = filePath;

            m_database->loadFromFile( m_databasePath );

            if( Path::isExistingFile( filePath ) )
            {
                FB_LOG( "Database::load: database exists at " + filePath );
            }

#if SQLITE_HAS_CODEC
            if( Path::getFileExtension( filePath ) == ".dll" )
            {
                char key[] = { "w3l1k3r34l4sw3ll" };
                sqlite3_key( m_database->mpDB, key, strlen( key ) );
            }
#endif

#if FB_PROFILE_DB
            m_timer = FB_NEW TimerBoost;

            static int nameExt = 0;
            String newfilePath = filePath + StringUtil::toString( nameExt++ ) + String( ".log" );
            m_log.open( newfilePath );
#endif
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void DatabaseManager::loadFromFile( const StringW &filePath )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  

            FB_ASSERT( !StringUtilW::isNullOrEmpty( filePath ) );
            FB_LOG( L"Database::load: " + filePath );

            if( PathW::isExistingFile( filePath ) )
            {
                FB_LOG( L"Database::load: database exists at " + filePath );
            }

            m_databasePath = StringUtil::toStringC( filePath );

            m_database->loadFromFile( m_databasePath );

#if SQLITE_HAS_CODEC
            if( PathW::getFileExtension( filePath ) == L".dll" )
            {
                char key[] = { "w3l1k3r34l4sw3ll" };
                sqlite3_key( m_database->mpDB, key, strlen( key ) );
            }
#endif

#if FB_PROFILE_DB
            m_timer = FB_NEW TimerBoost;

            static int nameExt = 0;
            String newfilePath =
                StringUtil::toStringC( filePath ) + StringUtil::toString( nameExt++ ) + String( ".log" );
            m_log.open( newfilePath );
#endif
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void DatabaseManager::unload( SmartPtr<ISharedObject> data )
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

    void DatabaseManager::attach( const String &filePath )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  

            FB_ASSERT( !StringUtil::isNullOrEmpty( filePath ) );

            FB_LOG( "Database::load: " + filePath );

            if( Path::isExistingFile( filePath ) )
            {
                FB_LOG( "Database::load: database exists at " + filePath );
            }

            // ATTACH DATABASE '%s' AS encrypted KEY '%s'"

#if SQLITE_HAS_CODEC
#    if FB_CUSTOMER_FINAL
            char key[] = { "w3l1k3r34l4sw3ll" };
            String sql = "attach database '" + filePath + "' as refdb KEY '" + key + "' ";
            executeDML( sql );
#    else  // if FB_CUSTOMER_FINAL
            executeDML( "attach database '" + filePath + "' as refdb; " );
#    endif
#else
            executeDML( "attach database '" + filePath + "' as refdb; " );
#endif

            // auto rs = executeQuery("select * from refdb.sim_states");
            // if (rs)
            //{
            //	while (!rs->eof())
            //	{
            //		String xml = rs->toXML();
            //		printf("%s\n", xml.c_str());
            //		rs->nextRow();
            //	}
            // }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void DatabaseManager::attach( const StringW &filePath )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  

            FB_ASSERT( !StringUtilW::isNullOrEmpty( filePath ) );
            FB_LOG( L"Database::load: " + filePath );

            if( PathW::isExistingFile( filePath ) )
            {
                FB_LOG( L"Database::load: database exists at " + filePath );
            }

#if SQLITE_HAS_CODEC
#    if FB_CUSTOMER_FINAL
            StringW key = L"w3l1k3r34l4sw3ll";
            StringW sql = L"attach database '" + filePath + L"' as refdb KEY '" + key + L"' ";

#        ifdef FB_PLATFORM_WINDOWS
            executeDML( sql );
#        else
            String sqlStr = StringUtil::toUTF16to8( sql );
            executeDML( sqlStr );
#        endif
#    else
            executeDML( L"attach database '" + filePath + L"' as 'refdb'; " );
#    endif
#else
            executeDML( L"attach database '" + filePath + L"' as 'refdb'; " );
#endif

            // auto rs = executeQuery("select * from refdb.sim_states");
            // if (rs)
            //{
            //	while (!rs->eof())
            //	{
            //		String xml = rs->toXML();
            //		printf("%s\n", xml.c_str());
            //		rs->nextRow();
            //	}
            // }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    SmartPtr<IDatabaseQuery> DatabaseManager::executeQuery( const String &queryStr )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  
            FB_LOG( "SQL: " + queryStr );
            return m_database->query( queryStr );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    SmartPtr<IDatabaseQuery> DatabaseManager::executeQueryAsync( const String &queryStr )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto jobQueue = applicationManager->getJobQueue();
            FB_ASSERT( jobQueue );

            SmartPtr<QueryJob> job( new QueryJob( this, queryStr ) );
            jobQueue->addJob( job );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    int DatabaseManager::executeDML( const String &dml )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  
            FB_LOG( "SQL: " + dml );
            m_database->queryDML( dml );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return 0;
    }

    int DatabaseManager::executeDML( const StringW &dml )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );  
            FB_LOG( "SQL: " + StringUtil::toStringC( dml ) );
            m_database->queryDML( dml );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return 0;
    }

    int DatabaseManager::executeAsyncDML( const String &tag, const String &val )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto jobQueue = applicationManager->getJobQueue();

            SmartPtr<DMLQueryJob> job( new DMLQueryJob( this, tag, val ) );
            jobQueue->addJob( job );
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return 0;
    }

    void DatabaseManager::runScript( const String &filePath )
    {
        try
        {
            // RecursiveMutex::ScopedLock lock(Mutex);

            // std::fstream* stream = OGRE_NEW_T(std::fstream(filePath, std::fstream::in |
            // std::fstream::binary), 	Ogre::MEMCATEGORY_GENERAL); if (!stream->is_open())
            //{
            //	throw Exception("File open error.");
            // }

            // Ogre::SmartPtr<IStream> dataStream(OGRE_NEW Ogre::FileStreamDataStream(stream, true));
            // String data = dataStream->getAsString();

            // int ret;
            // if (SQLITE_OK != (ret = sqlite3_exec(m_database->mpDB, data.c_str(), nullptr, nullptr,
            // nullptr)))
            //{
            //	String message = String("Failed to execute SQLite script. Error: ") + String(
            //		sqlite3_errmsg(m_database->mpDB));
            //	throw Exception(message);
            // }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void DatabaseManager::setSetting( const String &name, const String &value )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );  

        auto sql = String( "select * from settings where param = '" ) + name + "'";
        auto queryResult = executeQuery( sql );
        auto hasSetting = !queryResult->eof();
        if( hasSetting )
        {
            String insertSql = "update settings set value='" + value + "' where param = '" + name + "'";
            executeQuery( insertSql );
        }
        else
        {
            String insertSql =
                "insert into settings(param, value) VALUES('" + name + "', '" + value + "')";
            executeQuery( insertSql );
        }
    }

    void DatabaseManager::setSettingAsBool( const String &name, bool bValue )
    {
        String value = bValue ? "true" : "false";
        setSetting( name, value );
    }

    void DatabaseManager::setSettingAsInt( const String &name, s32 iValue )
    {
        String value = StringUtil::toString( iValue );
        setSetting( name, value );
    }

    void DatabaseManager::setSettingAsFloat( const String &name, f32 fValue )
    {
        String value = StringUtil::toString( fValue );
        setSetting( name, value );
    }

    String DatabaseManager::getSetting( const String &name, String value /*= ""*/ )
    {
        auto sql = "select * from settings where param = '" + name + "'";
        auto queryResult = executeQuery( sql );
        while( !queryResult->eof() )
        {
            value = queryResult->getFieldValue( "value" );
            queryResult->nextRow();
        }

        return value;
    }

    String DatabaseManager::getSettingRef( const String &name, String value /*= ""*/ )
    {
        auto sql = "select * from refdb.settings where param = '" + name + "'";
        auto queryResult = executeQuery( sql );
        while( !queryResult->eof() )
        {
            value = queryResult->getFieldValue( "value" );
            queryResult->nextRow();
        }

        return value;
    }

    s32 DatabaseManager::getSettingAsInt( const String &name, s32 defaultValue /*= 0*/ )
    {
        String val = getSetting( name );
        if( !StringUtil::isNullOrEmpty( val ) )
        {
            return StringUtil::parseInt( val );
        }

        return defaultValue;
    }

    bool DatabaseManager::getSettingAsBool( const String &name, bool defaultValue /*= false*/ )
    {
        String val = getSetting( name );
        if( !StringUtil::isNullOrEmpty( val ) )
        {
            return StringUtil::parseBool( val );
        }

        return defaultValue;
    }

    f32 DatabaseManager::getSettingAsFloat( const String &name, f32 defaultValue /*= 0.0f*/ )
    {
        String val = getSetting( name );
        if( !StringUtil::isNullOrEmpty( val ) )
        {
            return StringUtil::parseFloat( val );
        }

        return defaultValue;
    }

    bool DatabaseManager::hasStateValue( const String &name )
    {
        String sql = "select * from sim_states where param = '" + name + "';";
        auto query = executeQuery( sql );
        if( query && !query->eof() )
        {
            return true;
        }

        return false;
    }

    void DatabaseManager::setStateValue( const String &name, const String &value )
    {
        if( hasStateValue( name ) )
        {
            String sql = "update sim_states set value = '" + value + "' where param = '" + name + "';";
            executeQuery( sql );
        }
        else
        {
            String sql = "INSERT INTO sim_states(param, value) VALUES('" + name + "', '" + value + "');";
            executeQuery( sql );
        }
    }

    void DatabaseManager::setStateValueAsBool( const String &name, bool val )
    {
        setStateValue( name, StringUtil::toString( val ) );
    }

    void DatabaseManager::setStateValueAsInt( const String &name, int val )
    {
        setStateValue( name, StringUtil::toString( val ) );
    }

    void DatabaseManager::setStateValueAsFloat( const String &name, float val )
    {
        setStateValue( name, StringUtil::toString( val ) );
    }

    String DatabaseManager::getStateValue( const String &name )
    {
        String sql = "select * from sim_states where param = '" + name + "';";
        auto query = executeQuery( sql );
        if( query && !query->eof() )
        {
            return query->getFieldValue( "value" );
        }

        return "";
    }

    bool DatabaseManager::getStateValueAsBool( const String &name )
    {
        auto sql = "select * from sim_states where param = '" + name + "';";
        auto query = executeQuery( sql );
        if( query && !query->eof() )
        {
            return StringUtil::parseBool( query->getFieldValue( "value" ), false );
        }

        return false;
    }

    s32 DatabaseManager::getStateValueAsInt( const String &name )
    {
        String sql = "select * from sim_states where param = '" + name + "';";
        auto query = executeQuery( sql );
        if( query && !query->eof() )
        {
            return StringUtil::parseInt( query->getFieldValue( "value" ), 0 );
        }

        return 0;
    }

    f32 DatabaseManager::getStateValueAsFloat( const String &name )
    {
        String sql = "select * from sim_states where param = '" + name + "';";
        auto query = executeQuery( sql );
        if( query && !query->eof() )
        {
            return StringUtil::parseFloat( query->getFieldValue( "value" ), 0.0f );
        }

        return 0.0f;
    }

    void DatabaseManager::optimise()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );  

        executeQuery( "PRAGMA foreign_keys = ON;" );
        executeQuery( "PRAGMA count_changes = OFF;" );
        executeQuery( "PRAGMA synchronous=OFF;" );
        // executeQuery("PRAGMA page_size = 8192;");
        executeQuery( "PRAGMA cache_size = 10000;" );
        executeQuery( "PRAGMA journal_mode=memory;" );
        // executeQuery("PRAGMA locking_mode=EXCLUSIVE;");
        executeQuery( "PRAGMA temp_store=MEMORY;" );
        // executeQuery("PRAGMA threads = 4;");

        // sqlite3_exec(m_database->mpDB, "VACUUM;", nullptr, nullptr, nullptr);
    }

    DatabaseManager::QueryJob::QueryJob( SmartPtr<DatabaseManager> database, const String &query ) :
        m_query( query ),
        m_database( database )
    {
    }

    DatabaseManager::QueryJob::QueryJob()
    {
    }

    DatabaseManager::QueryJob::~QueryJob()
    {
    }

    void DatabaseManager::QueryJob::execute()
    {
        try
        {
            // m_database->executeQuery(m_query);
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    DatabaseManager::DMLQueryJob::DMLQueryJob( SmartPtr<DatabaseManager> database, const String &tag,
                                               const String &query ) :
        m_tag( tag ),
        m_query( query ),
        m_database( database )
    {
    }

    DatabaseManager::DMLQueryJob::DMLQueryJob()
    {
    }

    DatabaseManager::DMLQueryJob::~DMLQueryJob()
    {
    }

    void DatabaseManager::DMLQueryJob::execute()
    {
        // try
        //{
        //	m_database->executeDML(m_query);

        //	auto applicationManager = core::IApplicationManager::instance();
        //	auto pluginInterface = applicationManager->getPluginInterface();

        //	auto pluginEvent = fb::make_ptr<core::PluginEvent>();
        //	pluginEvent->setArg1(m_tag);

        //	static const String sQueryResult = "queryResult";
        //	pluginInterface->sendEvent(sQueryResult, pluginEvent);
        //}
        // catch (Exception& e)
        //{
        //	FB_LOG_EXCEPTION(e);
        //}
    }

    void DatabaseManager::dropAllTables()
    {
        auto cursor = executeQuery( "SELECT name FROM sqlite_master WHERE type='table'" );
        while( !cursor->eof() )
        {
            auto name = cursor->getFieldValue( "name" );
            executeQuery( "DROP TABLE IF EXISTS " + name );
            cursor->nextRow();
        }
    }

    void DatabaseManager::rekey( const String &key )
    {
#if SQLITE_HAS_CODEC
        if( !StringUtil::isNullOrEmpty( key ) )
        {
            sqlite3_rekey( m_database->mpDB, key.c_str(), key.size() );
        }
#else
        FB_LOG_ERROR( String( "Not compiled with codec" ) );
#endif
    }

    void *DatabaseManager::getPtr() const
    {
        return nullptr;  // m_database->mpDB;
    }

    String DatabaseManager::getResourceValue( int id )
    {
        auto parent_id = -1;

#if FB_BUILD_EDITOR_PLUGIN
        auto modelSql = "Select * From configured_models where id = " + StringUtil::toString( id );
#else
        auto modelSql = "Select * From refdb.configured_models where id = " + StringUtil::toString( id );
#endif

        auto modelResult = executeQuery( modelSql );
        if( modelResult )
        {
            if( !modelResult->eof() )
            {
                return modelResult->getFieldValue( "resource_name" );
            }
        }

        return "";
    }

    String DatabaseManager::getResourceValue( const String &name )
    {
#if FB_BUILD_EDITOR_PLUGIN
        auto modelSql = "select * from resourcemap where name = '" + name + "'";
#else
        auto modelSql = "select * from refdb.resourcemap where name = '" + name + "'";
#endif

        auto modelResult = executeQuery( modelSql );
        if( modelResult )
        {
            if( !modelResult->eof() )
            {
                return modelResult->getFieldValue( "name" );
            }
        }

        return name;
    }

    SmartPtr<IDatabase> DatabaseManager::getDatabase() const
    {
        return m_database;
    }

    void DatabaseManager::setDatabase( SmartPtr<IDatabase> database )
    {
        m_database = database;
    }

    void DatabaseManager::setDatabasePath( const String &databasePath )
    {
        m_databasePath = databasePath;
    }

    Array<String> DatabaseManager::getAttached() const
    {
        return m_attached;
    }

    void DatabaseManager::setAttached( const Array<String> &attached )
    {
        m_attached = attached;
    }

    void DatabaseManager::addAttached( const String &attached )
    {
        m_attached.push_back( attached );
    }

    String DatabaseManager::getDatabasePath() const
    {
        return m_databasePath;
    }
}  // end namespace fb
