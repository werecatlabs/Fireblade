#ifndef CResourceDatabaseMT_h__
#define CResourceDatabaseMT_h__

#include <FBCore/Resource/CResourceDatabase.h>

namespace fb
{
    class CResourceDatabaseMT : public CResourceDatabase
    {
    public:
        CResourceDatabaseMT();
        ~CResourceDatabaseMT() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        void build() override;
        void refresh() override;

        void importFolder( SmartPtr<IFolderExplorer> folderListing );
        void importFile( const String &filePath ) override;

        void importAssets() override;
        void reimportAssets() override;

        Array<SmartPtr<IResource>> getResources() const override;

        SmartPtr<IResource> loadResource( const String &path ) override;

        SmartPtr<IDatabaseManager> getDatabaseManager() const;
        void setDatabaseManager( SmartPtr<IDatabaseManager> databaseManager );

        /** @copydoc IResourceDatabase::createOrRetrieve */
        Pair<SmartPtr<IResource>, bool> createOrRetrieve( hash_type type, const String &path ) override;

        /** @copydoc IResourceDatabase::createOrRetrieve */
        Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path ) override;

        SmartPtr<ISharedObject> getObject( const String &uuid ) override;

        Array<SmartPtr<ISharedObject>> getSceneObjects() const;

        SmartPtr<ISharedObject> getObjectByFileId( const String &fileId ) const;

    protected:
        class BuildResourceDatabaseJob : public Job
        {
        public:
            BuildResourceDatabaseJob() = default;
            ~BuildResourceDatabaseJob() override = default;

            void execute() override;

            SmartPtr<CResourceDatabaseMT> getOwner() const;
            void setOwner( SmartPtr<CResourceDatabaseMT> owner );

        private:
            SmartPtr<CResourceDatabaseMT> m_owner;
        };

        void getSceneObjects( SmartPtr<scene::IActor> actor, Array<SmartPtr<ISharedObject>> &objects ) const;

        Array<SmartPtr<ISharedObject>> m_objects;

        SmartPtr<IDatabaseManager> m_databaseManager;
    };
}  // end namespace fb

#endif  // CResourceDatabase_h__
