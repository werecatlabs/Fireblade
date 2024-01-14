#ifndef CResourceDatabase_h__
#define CResourceDatabase_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/System/Job.h>
#include <FBCore/Core/UtilityTypes.h>
#include <FBCore/Core/Map.h>
#include <FBCore/Core/UnorderedMap.h>

namespace fb
{
    /** ResourceDatabase
     *  @brief
     *    ResourceDatabase is a class that manages all resources in the engine.
     *    It is responsible for loading and unloading resources, as well as
     *    creating new resources.
     */
    class ResourceDatabase : public IResourceDatabase
    {
    public:
        class ImportFileJob : public Job
        {
        public:
            ImportFileJob();
            ~ImportFileJob() override;

            void execute() override;

            void setFilePath( const String &filePath );
            String getFilePath() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_filePath;
        };

        /** Constructor. */
        ResourceDatabase();

        /** Destructor. */
        ~ResourceDatabase() override;

        /** @copydoc ISharedObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc ISharedObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        void build() override;
        void refresh() override;

        void optimise();

        void clean();

        void deleteCache();

        bool hasResource( SmartPtr<IResource> resource ) override;
        void addResource( SmartPtr<IResource> resource ) override;
        void removeResource( SmartPtr<IResource> resource ) override;

        void removeResourceFromPath( const String &path );

        SmartPtr<IResource> findResource( u32 type, const String &path ) override;
        SmartPtr<IResource> cloneResource( u32 type, SmartPtr<IResource> resource,
                                           const String &path ) override;

        void importFolder( SmartPtr<IFolderExplorer> folderListing ) override;

        void importFolder( const String &path ) override;
        void importFile( const String &filePath ) override;

        void importCache() override;
        void importAssets() override;
        void reimportAssets() override;
        void calculateDependencies() override;

        Array<SmartPtr<scene::IDirector>> getResourceData() const override;

        Array<SmartPtr<IResource>> getResources() const override;

        void createActor( SmartPtr<scene::IActor> parent, SmartPtr<Properties> properties );
        SmartPtr<IResource> loadResource( hash64 id ) override;
        SmartPtr<IResource> loadResource( const String &path ) override;

        SmartPtr<scene::IDirector> loadDirector( const String &filePath ) override;

        SmartPtr<scene::IDirector> loadDirectorFromResourcePath( const String &filePath ) override;
        SmartPtr<scene::IDirector> loadDirectorFromResourcePath( const String &filePath,
                                                                 u32 type ) override;

        SmartPtr<IResource> loadResourceById( const String &uuid ) override;

        SmartPtr<IResource> createOrRetrieveResource( SmartPtr<IDatabaseQuery> query,
                                                      bool bLoadResource );

        SmartPtr<IDatabaseManager> getDatabaseManager() const override;
        void setDatabaseManager( SmartPtr<IDatabaseManager> databaseManager ) override;

        /** @copydoc IResourceDatabase::createOrRetrieve */
        Pair<SmartPtr<IResource>, bool> createOrRetrieveFromDirector(
            hash_type type, const String &path, SmartPtr<scene::IDirector> director ) override;

        /** @copydoc IResourceDatabase::createOrRetrieve */
        Pair<SmartPtr<IResource>, bool> createOrRetrieve( hash_type type, const String &path ) override;

        /** @copydoc IResourceDatabase::createOrRetrieve */
        Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path ) override;

        SmartPtr<ISharedObject> getObject( const String &uuid ) override;

        SmartPtr<Properties> loadAssetNode( SmartPtr<IDatabaseManager> db, SmartPtr<Properties> parent,
                                            hash64 id );
        Array<SmartPtr<ISharedObject>> getSceneObjects() const;
        Array<SmartPtr<ISharedObject>> getReferenceObjects() const;

        SmartPtr<ISharedObject> getObjectByFileId( const String &fileId ) const override;

        SharedPtr<Array<SmartPtr<IResource>>> getInstancesPtr() const;
        void setInstancesPtr( SharedPtr<Array<SmartPtr<IResource>>> instances );

        Array<String> getTextureTypes() const;

        void setTextureTypes( const Array<String> &textureTypes );

        Array<String> getAudioTypes() const;

        void setAudioTypes( const Array<String> &audioTypes );

        FB_CLASS_REGISTER_DECL;

    protected:
        SmartPtr<Properties> loadAssetProperties( hash64 id );

        void getSceneObjects( SmartPtr<scene::IActor> actor,
                              Array<SmartPtr<ISharedObject>> &objects ) const;

        std::unordered_map<hash_type, std::unordered_map<String, WeakPtr<IResource>>> m_resourceMap;
        SharedPtr<Array<SmartPtr<IResource>>> m_instances;
        Array<SmartPtr<ISharedObject>> m_objects;

        Array<String> m_textureTypes;
        Array<String> m_audioTypes;

        SmartPtr<IDatabaseManager> m_databaseManager;

        mutable RecursiveMutex m_mutex;

        static atomic_s32 m_numJobs;
    };
}  // end namespace fb

#endif  // CResourceDatabase_h__
