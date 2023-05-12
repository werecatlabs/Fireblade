#ifndef _IResourceGroupManager_H
#define _IResourceGroupManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Array.h>

namespace fb
{

    /**
     * An interface for managing resource groups.
     */
    class IResourceGroupManager : public ISharedObject
    {
    public:
        static const String DEFAULT_RESOURCE_GROUP_NAME;

        /** Destructor. */
        ~IResourceGroupManager() override = default;

        /**
         * Initializes all resource groups.
         */
        virtual void initialiseAllResourceGroups() = 0;

        /**
         * Initializes a resource group.
         *
         * @param groupName The name of the resource group.
         */
        virtual void initialiseResourceGroup( const String &groupName ) = 0;

        /**
         * Unloads a resource group.
         *
         * @param groupName The name of the resource group.
         */
        virtual void unloadResourceGroup( const String &groupName ) = 0;

        /**
         * Clears a resource group.
         *
         * @param groupName The name of the resource group.
         */
        virtual void clearResourceGroup( const String &groupName ) = 0;

        /**
         * Destroys a resource group.
         *
         * @param groupName The name of the resource group.
         */
        virtual void destroyResourceGroup( const String &groupName ) = 0;

        /**
         * Gets a pointer to the underlying object. This is dependent on the graphics library used.
         *
         * @param ppObject A pointer to the underlying object.
         */
        virtual void _getObject( void **ppObject ) const = 0;

        /**
         * Reloads resources in a resource group.
         *
         * @param groupName The name of the resource group.
         */
        virtual void reloadResources( const String &groupName ) = 0;

        /**
         * Parses scripts.
         *
         * @param scripts An array of script filenames to parse.
         */
        virtual void parseScripts( const Array<String> &scripts ) = 0;

        /**
         * Gets the state object associated with the resource group manager.
         *
         * @return The state object.
         */
        virtual SmartPtr<IStateContext> getStateObject() const = 0;

        /**
         * Sets the state object associated with the resource group manager.
         *
         * @param stateObject The state object to set.
         */
        virtual void setStateObject( SmartPtr<IStateContext> stateObject ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif
