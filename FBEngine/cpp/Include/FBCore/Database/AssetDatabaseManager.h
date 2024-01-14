#ifndef ___AssetDatabaseManager_h__
#define ___AssetDatabaseManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Database/DatabaseManager.h>

namespace fb
{

    class AssetDatabaseManager : public DatabaseManager
    {
    public:
        AssetDatabaseManager();
        ~AssetDatabaseManager() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void clearDatabase();

        bool hasResourceEntry( SmartPtr<ISharedObject> object );
        void addResourceEntry( SmartPtr<ISharedObject> object );
        void updateResourceEntry( SmartPtr<ISharedObject> object );
        void removeResourceEntry( SmartPtr<ISharedObject> object );

        SmartPtr<scene::IDirector> getResourceEntry( const String &uuid );
        SmartPtr<scene::IDirector> getResourceEntryFromPath( const String &path );

        bool hasResourceById( const String &uuid );

    protected:
    };
}  // end namespace fb

#endif  // ___AssetDatabaseManager_h__
