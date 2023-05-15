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

        void unload( SmartPtr<ISharedObject> data ) override;

        void setAssetDataFromPath( const String &path, SmartPtr<IData> data );
        SmartPtr<IData> getAssetDataFromPath( const String &path );

        void clearDatabase();

        bool hasResourceEntry( SmartPtr<ISharedObject> resource );
        void addResourceEntry( SmartPtr<ISharedObject> resource );
        void removeResourceEntry( SmartPtr<ISharedObject> resource );

        SmartPtr<IResource> loadResourceById( const String &uuid );

        SmartPtr<IResource> loadResource( const String &filePath );

        bool hasResourceById( const String &uuid );

    protected:
    };
}  // end namespace fb

#endif  // ___AssetDatabaseManager_h__
