#ifndef __FB_IResource_h__
#define __FB_IResource_h__

#include <FBCore/Interface/IPrototype.h>

namespace fb
{

    /**
     * Interface for a resource.
     *
     * This class defines an interface for a resource, which is an asset that can be used by the application.
     * Resources can be loaded from and saved to a file, and their state can be managed through state objects.
     * This class also defines methods for setting and getting the resource type and file system ID.
     * Resource implementations should derive from this class and provide their own implementation for the methods
     * defined here.
     *
     * @author Zane Desir
     * @version 1.0
     */
    class IResource : public core::IPrototype
    {
    public:
        /**
         * Enum that defines the possible types of resources.
         */
        enum class ResourceType
        {
            None,      /**< No resource type. */
            Material,  /**< Material resource type. */
            Object,    /**< Object resource type. */
            Actor,     /**< Actor resource type. */
            Component, /**< Component resource type. */
            Texture,   /**< Texture resource type. */
            Font,      /**< Font resource type. */
            Count      /**< Number of resource types. */
        };

        /** Destructor. */
        ~IResource() override = default;

        /**
         * Saves the resource to a file.
         *
         * @param filePath The file path of the resource.
         */
        virtual void saveToFile( const String &filePath ) = 0;

        /**
         * Loads the resource from a file.
         *
         * @param filePath The file path of the resource.
         */
        virtual void loadFromFile( const String &filePath ) = 0;

        /** Saves the resource. */
        virtual void save() = 0;

        /** Imports the resource. */
        virtual void import() = 0;

        /** Reimports the resource. */
        virtual void reimport() = 0;

        /**
         * Gets the file system ID of the resource.
         *
         * @return The file system ID.
         */
        virtual hash64 getFileSystemId() const = 0;

        /**
         * Sets the file system ID of the resource.
         *
         * @param id The file system ID to set.
         */
        virtual void setFileSystemId( hash64 id ) = 0;

        /**
         * Gets the file path of the resource.
         *
         * @return The file path of the resource.
         */
        virtual String getFilePath() const = 0;

        /**
         * Sets the file path of the resource.
         *
         * @param filePath The file path to set.
         */
        virtual void setFilePath( const String &filePath ) = 0;

        /**
         * Gets the file system ID of the resource's settings.
         *
         * @return The file system ID of the resource's settings.
         */
        virtual hash64 getSettingsFileSystemId() const = 0;

        /**
         * Sets the file system ID of the resource's settings.
         *
         * @param id The file system ID to set.
         */
        virtual void setSettingsFileSystemId( hash64 id ) = 0;

        /**
         * Gets the state object for the resource.
         *
         * @return The state object. Can be null.
         */
        virtual SmartPtr<IStateContext> getStateObject() const = 0;

        /**
         * Sets the state object for the resource.
         *
         * @param stateObject The state object to set. Can be null.
         */
        virtual void setStateObject( SmartPtr<IStateContext> stateObject ) = 0;

        /**
         * Gets the resource type.
         *
         * @return The resource type.
         */
        virtual ResourceType getResourceType() const = 0;

        /**
         * Sets the type of the resource.
         *
         * @param resourceType The type of the resource.
         */
        virtual void setResourceType( ResourceType resourceType ) = 0;

        /**
         * Gets the name of the resource type.
         *
         * @return The name of the resource type.
         */
        virtual String getResourceTypeByName() const = 0;

        /**
         * Sets the name of the resource type.
         *
         * @param resourceTypeName The name of the resource type.
         */
        virtual void setResourceTypeByName( const String &resourceTypeName ) = 0;

        /**
         * Gets a pointer to the specific implementation object for the resource.
         *
         * @param ppObject A pointer to store the object.
         */
        virtual void _getObject( void **ppObject ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IResource_h__
