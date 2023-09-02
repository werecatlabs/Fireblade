#ifndef FBSQLite_h__
#define FBSQLite_h__

#include <FBCore/Interface/Database/IDatabase.h>
#include <FBCore/Interface/System/IPlugin.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class SQLitePlugin : public ISharedObject
    {
    public:
        SQLitePlugin() = default;
        ~SQLitePlugin() override = default;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void createFactories();

        SmartPtr<IDatabase> createSQLiteDB();
    };
} // end namespace fb

#endif  // FBSQLite_h__
