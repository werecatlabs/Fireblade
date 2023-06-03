#ifndef __DatabaseManager_h__
#define __DatabaseManager_h__

#include <FBCore/Interface/Database/IDatabaseManager.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/System/Job.h>

namespace fb
{

    class DatabaseManager : public SharedObject<IDatabaseManager>
    {
    public:
        DatabaseManager();
        ~DatabaseManager() override;

        void open() override;
        void close() override;

        void loadFromHandle( size_t handle ) override;

        void loadFromFile( const String &filePath ) override;
        void loadFromFile( const StringW &filePath ) override;

        void optmise() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        void attach( const String &filePath ) override;
        void attach( const StringW &filePath ) override;

        SmartPtr<IDatabaseQuery> executeQuery( const String &query ) override;
        SmartPtr<IDatabaseQuery> executeQueryAsync( const String &query );

        s32 executeDML( const String &dml ) override;
        s32 executeDML( const StringW &dml ) override;

        s32 executeAsyncDML( const String &tag, const String &val ) override;

        void runScript( const String &filePath );

        void setSetting( const String &name, const String &value );
        void setSettingAsBool( const String &name, bool bValue );
        void setSettingAsInt( const String &name, s32 iValue );
        void setSettingAsFloat( const String &name, f32 fValue );

        String getSetting( const String &name, String value = "" );
        String getSettingRef( const String &name, String value = "" );

        bool getSettingAsBool( const String &name, bool defaultValue = false );
        s32 getSettingAsInt( const String &name, s32 defaultValue = 0 );
        f32 getSettingAsFloat( const String &name, f32 defaultValue = 0.0f );

        bool hasStateValue( const String &name );

        void setStateValue( const String &name, const String &value );
        void setStateValueAsBool( const String &name, bool val );
        void setStateValueAsInt( const String &name, int val );
        void setStateValueAsFloat( const String &name, float val );

        String getStateValue( const String &name );
        bool getStateValueAsBool( const String &name );
        int getStateValueAsInt( const String &name );
        float getStateValueAsFloat( const String &name );

        void dropAllTables();

        void rekey( const String &key );

        void *getPtr() const;

        String getResourceValue( const String &name );
        String getResourceValue( int id );

        SmartPtr<IDatabase> getDatabase() const override;
        void setDatabase( SmartPtr<IDatabase> database ) override;

        String getDatabasePath() const;
        void setDatabasePath( const String &databasePath );

        Array<String> getAttached() const override;

        void setAttached( const Array<String> &attached ) override;

        void addAttached( const String &attached ) override;

    protected:
        class QueryJob : public Job
        {
        public:
            QueryJob();
            QueryJob( SmartPtr<DatabaseManager> database, const String &query );
            ~QueryJob() override;

            void execute() override;

            String m_query;

        private:
            SmartPtr<DatabaseManager> m_database;
        };

        class DMLQueryJob : public Job
        {
        public:
            DMLQueryJob();
            DMLQueryJob( SmartPtr<DatabaseManager> database, const String &tag, const String &query );
            ~DMLQueryJob() override;

            void execute() override;

            String m_tag;
            String m_query;
            SmartPtr<DatabaseManager> m_database;
        };

        SmartPtr<IDatabase> m_database;

        String m_databasePath;
        Array<String> m_attached;

        RecursiveMutex m_mutex;  

#if FB_PROFILE_DB
        SmartPtr<Timer> m_timer;
        std::ofstream m_log;
#endif
    };
}  // end namespace fb

#endif  // Database_h__
