#ifndef IResourceDatabase_h__
#define IResourceDatabase_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/UtilityTypes.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{

    /** Interface for a resource database. */
    class IResourceDatabase : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IResourceDatabase() override = default;

        /** Import assets. */
        virtual void importAssets() = 0;

        /** Reimport assets. */
        virtual void reimportAssets() = 0;

        /** Builds the resource database. */
        virtual void build() = 0;

        /** Refreshes the resource database. */
        virtual void refresh() = 0;

        /**
         * @brief Checks if the resource database contains the specified resource.
         * @param resource The resource to check.
         * @return True if the resource is in the database, false otherwise.
         */
        virtual bool hasResource( SmartPtr<IResource> resource ) = 0;

        /**
         * @brief Adds a resource to the database.
         * @param resource The resource to add.
         */
        virtual void addResource( SmartPtr<IResource> resource ) = 0;

        /**
         * @brief Removes a resource from the database.
         * @param resource The resource to remove.
         */
        virtual void removeResource( SmartPtr<IResource> resource ) = 0;

        /**
         * @brief Finds a resource with the given type and path.
         * @param type The type of the resource to find.
         * @param path The path of the resource to find.
         * @return A pointer to the resource if found, nullptr otherwise.
         */
        virtual SmartPtr<IResource> findResource( u32 type, const String &path ) = 0;

        /**
         * @brief Clones the specified resource with the given path.
         * @param type The type of the resource to clone.
         * @param resource The resource to clone.
         * @param path The path of the cloned resource.
         * @return A pointer to the cloned resource if successful, nullptr otherwise.
         */
        virtual SmartPtr<IResource> cloneResource( u32 type, SmartPtr<IResource> resource,
                                                   const String &path ) = 0;

        /**
         * @brief Gets an array of directors for each resource in the database.
         * @return An array of directors.
         */
        virtual Array<SmartPtr<scene::IDirector>> getResourceData() const = 0;

        /**
         * @brief Gets an array of all resources in the database.
         * @return An array of resources.
         */
        virtual Array<SmartPtr<IResource>> getResources() const = 0;

        /**
         * @brief Imports the file at the given path into the database.
         * @param filePath The path of the file to import.
         */
        virtual void importFile( const String &filePath ) = 0;

        /**
         * @brief Loads a resource with the given path.
         * @param path The path of the resource to load.
         * @return A pointer to the loaded resource if successful, nullptr otherwise.
         */
        virtual SmartPtr<IResource> loadResource( const String &path ) = 0;

        /**
         * @brief Loads a resource with the given UUID.
         * @param uuid The UUID of the resource to load.
         * @return A pointer to the loaded resource if successful, nullptr otherwise.
         */
        virtual SmartPtr<IResource> loadResourceById( const String &uuid ) = 0;

        /**
         * @brief Gets the database manager.
         * @return A pointer to the database manager.
         */
        virtual SmartPtr<IDatabaseManager> getDatabaseManager() const = 0;

        /**
         * @brief Sets the database manager.
         * @param databaseManager A pointer to the database manager.
         */
        virtual void setDatabaseManager( SmartPtr<IDatabaseManager> databaseManager ) = 0;

        /** Creates a resource or retrieves an existing a resource.
        @param director The build director to build the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual Pair<SmartPtr<IResource>, bool> createOrRetrieveFromDirector(
            hash_type type, const String &path, SmartPtr<scene::IDirector> director ) = 0;

        /** Creates a resource or retrieves an existing a resource.
            @param type The type of the resource.
            @param path The path of the resource.
            @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual Pair<SmartPtr<IResource>, bool> createOrRetrieve( hash_type type,
                                                                  const String &path ) = 0;

        /** Creates a resource or retrieves an existing a resource.
            @param path The path of the resource.
            @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path ) = 0;

        /** Gets an object using an id. */
        virtual SmartPtr<ISharedObject> getObject( const String &uuid ) = 0;

        /** Gets an object by it's file id. */
        virtual SmartPtr<ISharedObject> getObjectByFileId( const String &fileId ) const = 0;

        /** Gets an object by it's file id. */
        template <class T>
        SmartPtr<T> getObjectTypeByFileId( const String &fileId ) const;

        /** Finds a resource. */
        template <class T>
        SmartPtr<T> findResourceByType( const String &path );

        /** Finds a resource. */
        template <class T>
        SmartPtr<T> cloneResourceByType( SmartPtr<IResource> resource, const String &path );

        /** Loads a resource. */
        template <class T>
        SmartPtr<T> loadResourceByType( const String &path );

        /** Gets resources by type. */
        template <class T>
        Array<SmartPtr<T>> getResourcesByType() const;

        /** Creates a resource or retrieves an existing a resource.
            @param director The build director to build the resource.
            @returns The resource instance. Can be null if the resource does not exist.
        */
        template <class T>
        Pair<SmartPtr<T>, bool> createOrRetrieveFromDirector( const String &path,
                                                              SmartPtr<scene::IDirector> director );

        /** Creates a resource or retrieves an existing a resource.
           @param path The path of the resource.
           @returns The resource instance. Can be null if the resource does not exist.
        */
        template <class T>
        Pair<SmartPtr<T>, bool> createOrRetrieveByType( const String &path );

        FB_CLASS_REGISTER_DECL;
    };

    template <class T>
    SmartPtr<T> IResourceDatabase::getObjectTypeByFileId( const String &fileId ) const
    {
        auto obj = getObjectByFileId( fileId );
        return fb::dynamic_pointer_cast<T>( obj );
    }

    template <class T>
    SmartPtr<T> IResourceDatabase::findResourceByType( const String &path )
    {
        auto type = T::typeInfo();
        auto resource = findResource( type, path );
        return fb::static_pointer_cast<T>( resource );
    }

    template <class T>
    SmartPtr<T> IResourceDatabase::cloneResourceByType( SmartPtr<IResource> resource,
                                                        const String &path )
    {
        auto type = T::typeInfo();
        auto newResource = cloneResource( type, resource, path );
        return fb::static_pointer_cast<T>( newResource );
    }

    template <class T>
    SmartPtr<T> IResourceDatabase::loadResourceByType( const String &path )
    {
        auto resource = loadResource( path );
        return fb::static_pointer_cast<T>( resource );
    }

    template <class T>
    Array<SmartPtr<T>> IResourceDatabase::getResourcesByType() const
    {
        const auto resources = getResources();

        Array<SmartPtr<T>> resourcesByType;
        resourcesByType.reserve( resources.size() );

        for( auto &r : resources )
        {
            if( r->isDerived<T>() )
            {
                resourcesByType.push_back( r );
            }
        }

        return resourcesByType;
    }

    template <class T>
    Pair<SmartPtr<T>, bool> IResourceDatabase::createOrRetrieveFromDirector(
        const String &path, SmartPtr<scene::IDirector> director )
    {
        auto type = T::typeInfo();
        auto result = createOrRetrieveFromDirector( type, path, director );
        return Pair<SmartPtr<T>, bool>( result.first, result.second );
    }

    template <class T>
    Pair<SmartPtr<T>, bool> IResourceDatabase::createOrRetrieveByType( const String &path )
    {
        auto type = T::typeInfo();
        auto result = createOrRetrieve( type, path );
        return Pair<SmartPtr<T>, bool>( result.first, result.second );
    }

}  // end namespace fb

#endif  // IResourceDatabase_h__
