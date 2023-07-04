#ifndef Project_h__
#define Project_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Resource/Resource.h>
#include <FBCore/Core/Map.h>

namespace fb
{
    namespace editor
    {

        class Project : public Resource<IResource>
        {
        public:
            static const String DEFAULT_PROJECT_NAME;
            static const String DEFAULT_MEDIA_PATH;
            static const String DEFAULT_SCRIPTS_PATH;
            static const String DEFAULT_ENTITIES_PATH;
            static const String DEFAULT_VERSION;

            Project();
            ~Project() override;

            const String &getEditableType() const;

            void create( const String &path );
            void load( const String &filePath );
            void save( const String &filePath );
            void save();

            String getLabel() const;
            void setLabel( const String &val );

            String getProjectDirectory() const;
            void setProjectDirectory( const String &val );

            String getProjectFilePath() const;
            void setProjectPath( String val );

            String getWorkingDirectory() const;
            void setWorkingDirectory( const String &val );

            String getApplicationFilePath() const;
            void setApplicationFilePath( const String &val );

            String getEntitiesPath() const;
            void setEntitiesPath( const String &val );

            Array<String> getPaths() const;
            void setPaths( Array<String> val );
            void addPath( const String &val );

            void setProperties( const Properties &properties );
            void getProperties( Properties &properties ) const;

            Array<String> getMediaPaths() const;
            void setMediaPaths( Array<String> val );

            bool isDirty() const;
            void setDirty( bool dirty );

            String getSelectedProjectPath() const;
            void setSelectedProjectPath( const String &val );

            void applyDefaults();

            SmartPtr<ISharedObject> getOwner() const;
            void getOwner( SmartPtr<ISharedObject> owner );

            String getCurrentScenePath() const;
            void setCurrentScenePath( const String &val );

            SmartPtr<ISharedObject> getDefaultData() const;

            /** Get object data as a structure. */
            SmartPtr<ISharedObject> toData() const override;

            /** Set object data from a structure. */
            void fromData( SmartPtr<ISharedObject> data ) override;

            void compile();

        protected:
            void addDefaultTasks();

            String m_currentScenePath;

            String m_uuid;

            String m_productName;
            String m_companyName;

            ///
            String m_label;

            /// The working directory
            String m_projectDirectory;

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
