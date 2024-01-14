#ifndef IProjectManager_h__
#define IProjectManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /** Interface for a project manager. */
    class IProjectManager : public ISharedObject
    {
    public:
        /** Destructor. */
        ~IProjectManager() override = default;

        virtual void generateProject() = 0;

        virtual void addIncludeFolder( const String &includeFolder ) = 0;

        virtual void removeIncludeFolder( const String &includeFolder ) = 0;

        virtual void addLibraryFolder( const String &libraryFolder ) = 0;

        virtual void removeLibraryFolder( const String &libraryFolder ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // IProjectManager_h__
