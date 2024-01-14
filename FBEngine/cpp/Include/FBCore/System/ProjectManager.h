#ifndef __FB_ProjectManager_h__
#define __FB_ProjectManager_h__

#include <FBCore/Interface/System/IProjectManager.h>

namespace fb
{
    /** The ProjectManager class is responsible for generating a project file for the engine. */
    class ProjectManager : public IProjectManager
    {
    public:
        ProjectManager();
        ~ProjectManager() override;

        void generateProject() override;

        bool isSharedLibrary() const;
        void setSharedLibrary( bool sharedLibrary );

        String getProjectName() const;

        void setProjectName( const String &projectName );

        String getEnginePath() const;

        void setEnginePath( const String &enginePath );

        void addIncludeFolder( const String &includeFolder ) override;

        void removeIncludeFolder( const String &includeFolder ) override;

        void addLibraryFolder( const String &libraryFolder ) override;

        void removeLibraryFolder( const String &libraryFolder ) override;

        Array<String> getIncludeFolders() const;

        void setIncludeFolders( const Array<String> &includeFolders );

        Array<String> getLibraryFolders() const;

        void setLibraryFolders( const Array<String> &libraryFolders );

        void addLibrary( const String &library );

        void removeLibrary( const String &library );

        Array<String> getLibraries() const;

        void setLibraries( const Array<String> &libraries );

        void clearLibraries();

    protected:
        String getPlatformOptions() const;
        String getToolsetOptions() const;

        mutable RecursiveMutex m_mutex;
        String m_projectName;
        String m_enginePath;

        Array<String> m_includeFolders;
        Array<String> m_libraryFolders;
        Array<String> m_libraries;

        bool m_isSharedLibrary = true;
    };
}  // end namespace fb

#endif  // ProjectManager_h__
