#ifndef __FBPlugin_H__
#define __FBPlugin_H__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>

#ifdef FB_PLATFORM_WIN32
#    include <libloaderapi.h>
#endif

namespace fb
{
#if defined FB_PLATFORM_WIN32
    typedef HMODULE LibraryHandle;
    typedef FARPROC LibraryFunction;
#else
    typedef void *LibraryHandle;
    typedef void *LibraryFunction;
#endif

    /** Interface for a plugin.*/
    class IPlugin : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IPlugin() override = default;

        /** Gets the library handle. */
        virtual LibraryHandle getLibraryHandle() const = 0;

        /** Sets the library handle. */
        virtual void setLibraryHandle( LibraryHandle handle ) = 0;

        /** Gets a library function. */
        virtual LibraryFunction getFunction( const String &name ) const = 0;

        /** Get the filename of the plugin. */
        virtual StringW getFilePath() const = 0;

        /** Set the filename of the plugin. */
        virtual void setFilePath( const StringW &fileName ) = 0;
    };
}  // end namespace fb

#endif
