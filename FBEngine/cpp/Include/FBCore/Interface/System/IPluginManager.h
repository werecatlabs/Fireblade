#ifndef __IPluginManager_H__
#define __IPluginManager_H__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    /** Interface to manage dynamically loaded libraries. */
    class IPluginManager : public ISharedObject
    {
    public:
        /** Default destructor.
         */
        ~IPluginManager() override = default;

        /** Loads a library from the filename provided.
        @param filename The name of the library.
        */
        virtual SmartPtr<IPlugin> loadPlugin( const String &filename ) = 0;

        /** Unloads the library.
        @param plugin The plugin to unload.
        */
        virtual void unloadPlugin( SmartPtr<IPlugin> plugin ) = 0;
    };
}  // end namespace fb

#endif
