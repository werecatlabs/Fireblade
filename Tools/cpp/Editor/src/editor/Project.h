#ifndef Project_h__
#define Project_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/System/IProject.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Resource/Resource.h>
#include <FBCore/Core/Map.h>

namespace fb
{
    namespace editor
    {
        class Project : public Resource<IProject>
        {
        public:
            static const String DEFAULT_PROJECT_NAME;
            static const String DEFAULT_MEDIA_PATH;
            static const String DEFAULT_SCRIPTS_PATH;
            static const String DEFAULT_ENTITIES_PATH;
            static const String DEFAULT_VERSION;

            Project();
            ~Project() override;

            void load( SmartPtr<ISharedObject> data );
            void unload( SmartPtr<ISharedObject> data );

            void create( const String &path );
            void loadFromFile( const String &filePath ) override;
            void saveToFile( const String &filePath ) override;
            void save() override;

            String getLabel() const;
            void setLabel( const String &label );

            String getPath() const override;
            void setPath( const String &projectDirectory ) override;

            String getWorkingDirectory() const;
            void setWorkingDirectory( const String &workingDirectory );

            String getApplicationFilePath() const override;
            void setApplicationFilePath( const String &applicationFilePath ) override;

            Array<String> getPaths() const;
            void setPaths( const Array<String> &paths );
            void addPath( const String &path );

            void setProperties( const Properties &properties );
            void getProperties( Properties &properties ) const;

            Array<String> getMediaPaths() const;
            void setMediaPaths( const Array<String> &mediaPaths );

            bool isDirty() const;
            void setDirty( bool dirty );

            String getSelectedProjectPath() const;
            void setSelectedProjectPath( const String &selectedProjectPath );

            void applyDefaults();

            SmartPtr<ISharedObject> getOwner() const;
            void getOwner( SmartPtr<ISharedObject> owner );

            String getCurrentScenePath() const;
            void setCurrentScenePath( const String &currentScenePath );

            SmartPtr<ISharedObject> getDefaultData() const;

            /** Get object data as a structure. */
            SmartPtr<ISharedObject> toData() const override;

            /** Set object data from a structure. */
            void fromData( SmartPtr<ISharedObject> data ) override;

            void compile();

            String getPluginHeader() const;

            String getPluginSource() const;

            SmartPtr<IPlugin> getPlugin() const;

            void setPlugin( SmartPtr<IPlugin> plugin );

            Array<String> getScriptFilePaths() const override;

            void setScriptFilePaths( const Array<String> &val ) override;

            Array<String> getResourceFolders() const override;

            void setResourceFolders( const Array<String> &val ) override;

            String getApplicationType() const override;

            void setApplicationType( const String &val ) override;

            bool isArchive() const override;

            void setArchive( bool val ) override;

        protected:
            void addDefaultTasks();

            SmartPtr<IPlugin> m_plugin;

            String m_currentScenePath;

            String m_uuid;

            String m_productName;
            String m_companyName;

            ///
            String m_label;

            /// The working directory
            String m_projectDirectory;

            /// The working directory
            String m_workingDirectory;

            ///
            String m_projectPath;

            String m_selectedProjectPath;

            ///
            String m_applicationFilePath;

            /// The scripts path.
            String m_scriptsPath;

            ///
            String m_entitiesPath;

            ///
            String m_version;

            /// The media path.
            Array<String> m_mediaPaths;

            ///
            Array<String> m_paths;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // Project_h__
