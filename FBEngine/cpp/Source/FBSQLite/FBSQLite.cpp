#include <FBSQLite/FBSQLite.h>
#include <FBSQLite/SQLiteDatabase.h>
#include <FBCore/FBCore.h>

namespace fb
{
    void SQLitePlugin::load( SmartPtr<ISharedObject> data )
    {
        createFactories();
    }

    void SQLitePlugin::unload( SmartPtr<ISharedObject> data )
    {
    }

    void SQLitePlugin::createFactories()
    {
        FactoryUtil::addFactory<SQLiteDatabase>();

#ifdef _DEBUG
        const auto typeManager = TypeManager::instance();
        const auto type = SQLiteDatabase::typeInfo();
        const auto baseType = IDatabase::typeInfo();
        const auto sharedType = SharedObject<IDatabase>::typeInfo();
        const auto sharedBaseType = typeManager->getBaseType( sharedType );
        const auto managerBaseType = typeManager->getBaseType( type );
        const auto derivedTypes = typeManager->getDerivedTypes( baseType );

        FB_ASSERT( SQLiteDatabase::typeInfo() != 0 );
        //FB_ASSERT( typeManager->isDerived( IDatabase::typeInfo(), SQLiteDatabase::typeInfo() ) );
        FB_ASSERT( typeManager->isDerived( SQLiteDatabase::typeInfo(), IDatabase::typeInfo() ) );
        FB_ASSERT( std::find( derivedTypes.begin(), derivedTypes.end(), type ) != derivedTypes.end() );
#endif
    }

    SmartPtr<IDatabase> SQLitePlugin::createSQLiteDB()
    {
        return fb::make_ptr<SQLiteDatabase>();
    }
}  // end namespace fb
