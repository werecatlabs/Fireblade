#ifndef __FB_ProjectManager_h__
#define __FB_ProjectManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class ProjectManager : public ISharedObject
    {
    public:
        ProjectManager();
        ~ProjectManager() override;

        void generateProject();

        bool isSharedLibrary() const;
        void setSharedLibrary( bool sharedLibrary );

	    String getProjectName() const;

        void setProjectName( const String& projectName );

	    String getEnginePath() const;

        void setEnginePath( const String& enginePath );

    protected:
        String getPlatformOptions() const;
        String getToolsetOptions() const;

        mutable RecursiveMutex m_mutex;
        String m_projectName;
        String m_enginePath;
        bool m_isSharedLibrary = true;
    };
}  // end namespace fb

#endif  // ProjectManager_h__
