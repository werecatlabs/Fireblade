#ifndef FBSQLiteManager_h__
#define FBSQLiteManager_h__

#include <FBSQLite/FBSQLitePrerequisites.h>
#include <FBCore/Interface/Database/IDatabase.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{

    class SQLiteDatabase : public CSharedObject<IDatabase>
    {
    public:
        SQLiteDatabase();
        ~SQLiteDatabase() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        void loadFromFile( const String &filePath ) override;

        void loadFromFile( const String &filePath, const String &key ) override;

        void setKey( const String &key ) override;

        SmartPtr<IDatabaseQuery> query( const String &queryStr ) override;

        SmartPtr<IDatabaseQuery> query( const StringW &queryStr ) override;

        void queryDML( const String &queryStr ) override;

        void queryDML( const StringW &queryStr ) override;

        void close() override;

        FB_CLASS_REGISTER_DECL;

    protected:
        SharedPtr<CppSQLite3DB> m_database;
    };
}  // end namespace fb

#endif  // FBSQLiteManager_h__
