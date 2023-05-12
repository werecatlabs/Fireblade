#ifndef IEditorManager_h__
#define IEditorManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for the editor manager. */
    class IEditorManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IEditorManager() override = default;

        /** Import the assets. */
        virtual void importAssets() = 0;

        /** Get the project path.
        @return A string with the project path.
        */
        virtual String getProjectPath() const = 0;

        /** Set the project path.
        @param path A string to the project path.
        */
        virtual void setProjectPath( const String &path ) = 0;

        /** Get the cache path.
        @return A string with the cache path.
        */
        virtual String getCachePath() const = 0;

        /** Set the cache path.
        @param path A string to the cache path.
        */
        virtual void setCachePath( const String &path ) = 0;
    };

}  // end namespace fb

#endif  // IEditorManager_h__
