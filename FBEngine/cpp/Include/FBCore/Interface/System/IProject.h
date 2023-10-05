#ifndef IProject_h__
#define IProject_h__

#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /** Interface for a project. */
    class IProject : public IResource
    {
    public:
        /** Destructor. */
        ~IProject() override = default;

        virtual String getApplicationFilePath() const = 0;
        virtual void setApplicationFilePath( const String &applicationFilePath ) = 0;

        virtual String getProjectFilePath() const = 0;
        virtual void setProjectFilePath( const String &val ) = 0;

        virtual String getPath() const = 0;
        virtual void setPath( const String &val ) = 0;

        virtual Array<String> getScriptFilePaths() const = 0;
        virtual void setScriptFilePaths( const Array<String> &val ) = 0;

        virtual Array<String> getResourceFolders() const = 0;
        virtual void setResourceFolders( const Array<String> &val ) = 0;

        virtual String getApplicationType() const = 0;
        virtual void setApplicationType( const String &val ) = 0;

        virtual bool isArchive() const = 0;
        virtual void setArchive( bool val ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IProject_h__
