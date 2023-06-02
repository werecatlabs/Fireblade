#ifndef IProject_h__
#define IProject_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    
    class IProject : public ISharedObject
    {
    public:
        /** */
        ~IProject() override = default;

        /** */
        virtual void load( const String &filePath ) = 0;

        virtual String getApplicationFilePath() const = 0;
        virtual void setApplicationFilePath( const String &val ) = 0;

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
    };
}  // end namespace fb

#endif  // IProject_h__
