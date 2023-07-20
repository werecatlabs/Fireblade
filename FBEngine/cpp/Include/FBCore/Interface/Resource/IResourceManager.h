#ifndef _IResourceManager_H
#define _IResourceManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/UtilityTypes.h>

namespace fb
{

    /** Interface for a resource manager. */
    class IResourceManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IResourceManager() override = default;

        /** Creates a resource.
        @param name The name of the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual SmartPtr<IResource> create( const String &name ) = 0;

        /** Creates a resource.
        @param name The name of the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual SmartPtr<IResource> create( const String &uuid, const String &name ) = 0;

        /** Creates a resource or retrieves an existing a resource.
        @param uuid The uuid of the resource.
        @param path The path of the resource.
        @param type The type of the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &uuid, const String &path,
                                                                  const String &type ) = 0;

        /** Creates a resource or retrieves an existing a resource.
        @param uuid The uuid of the resource.
        @param path The path of the resource.
        @param type The type of the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual Pair<SmartPtr<IResource>, bool> createOrRetrieve( const String &path ) = 0;

        /** Saves a resource to file.
        @param filePath The filePath of the resource.
        @param resource The resource instance.
        */
        virtual void saveToFile( const String &filePath, SmartPtr<IResource> resource ) = 0;

        /** Loads a resource from a file.
        @param filePath The file path of the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual SmartPtr<IResource> loadFromFile( const String &filePath ) = 0;

        /** Loads a resource.
        @param name The name of the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual SmartPtr<IResource> load( const String &name ) = 0;

        /** Gets an existing resource.
        @param name The name of the resource.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual SmartPtr<IResource> getByName( const String &name ) = 0;

        /** Gets an existing resource by passing the hash id.
        @param hash The name of the resource as a hash value.
        @returns The resource instance. Can be null if the resource does not exist.
        */
        virtual SmartPtr<IResource> getById( const String &uuid ) = 0;

        /** Gets a pointer to the underlying object.
        @param ppObject A pointer to store the object.
        */
        virtual void _getObject( void **ppObject ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif
