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

        virtual String getPath() const = 0;
        virtual void setPath( const String &path ) = 0;

        virtual Array<String> getScriptFilePaths() const = 0;
        virtual void setScriptFilePaths( const Array<String> &scriptFilePaths ) = 0;

        virtual Array<String> getResourceFolders() const = 0;
        virtual void setResourceFolders( const Array<String> &resourceFolders ) = 0;

        virtual String getApplicationType() const = 0;
        virtual void setApplicationType( const String &applicationType ) = 0;

        virtual bool isArchive() const = 0;
        virtual void setArchive( bool archive ) = 0;

        virtual bool isDirty() const = 0;
        virtual void setDirty( bool dirty ) = 0;

        virtual SmartPtr<IPlugin> getPlugin() const = 0;

        virtual void setPlugin( SmartPtr<IPlugin> plugin ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IProject_h__
