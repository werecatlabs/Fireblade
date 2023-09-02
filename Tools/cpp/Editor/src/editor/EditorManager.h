#ifndef __EditorManager_h__
#define __EditorManager_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/System/IEditorManager.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/HashMap.h>

namespace fb
{
    namespace editor
    {
        class EditorManager : public IEditorManager
        {
        public:
            EditorManager();
            ~EditorManager() override;

            void unload( SmartPtr<ISharedObject> data ) override;

            void update( time_interval t, time_interval dt );

            /** Import the assets. */
            void importAssets() override;

            void loadProject( const String &filePath );

            /** Get the project path.
            @return A string with the project path.
            */
            String getProjectPath() const override;

            /** Set the project path.
            @param path A string to the project path.
            */
            void setProjectPath( const String &path ) override;

            /** Get the cache path.
            @return A string with the cache path.
            */
            String getCachePath() const override;

            /** Set the cache path.
            @param path A string to the cache path.
            */
            void setCachePath( const String &path ) override;

            SmartPtr<ProjectManager> getProjectManager() const;
            void setProjectManager( SmartPtr<ProjectManager> projectManager );

            SmartPtr<Project> getProject() const;
            void setProject( SmartPtr<Project> val );

            SmartPtr<UIManager> getUI() const;
            void setUI( SmartPtr<UIManager> ui );

            SmartPtr<render::IDecalCursor> getDecalCursor() const;
            void setDecalCursor( SmartPtr<render::IDecalCursor> val );

            bool getEditTerrain() const;
            void setEditTerrain( bool val );

            bool getEditFoliage() const;
            void setEditFoliage( bool val );

            bool getFileSaved() const;
            void setFileSaved( bool val );

            void previewAsset( const String &path );

            SmartPtr<TranslateManipulator> getTranslateManipulator() const;
            void setTranslateManipulator( SmartPtr<TranslateManipulator> val );

            SmartPtr<RotateManipulator> getRotateManipulator() const;
            void setRotateManipulator( SmartPtr<RotateManipulator> val );

            SmartPtr<ScaleManipulator> getScaleManipulator() const;
            void setScaleManipulator( SmartPtr<ScaleManipulator> val );

            static SmartPtr<EditorManager> getSingletonPtr();
            static void setSingletonPtr( SmartPtr<EditorManager> editorManager );

        protected:
            /// A pointer to the singleton
            static SmartPtr<EditorManager> m_singleton;

            /// The project path.
            String m_projectPath;

            /// The cache path.
            String m_cachePath;

            ///
            SmartPtr<ProjectManager> m_projectManager;

            ///
            AtomicSmartPtr<Project> m_project;

            ///
            SmartPtr<UIManager> m_ui;

            ///
            SmartPtr<render::IDecalCursor> m_decalCursor;

            ///
            bool m_editTerrain;

            ///
            bool m_editFoliage;

            ///
            bool m_fileSaved;

            ///
            atomic_bool m_enablePhysics;

            ///
            EditorGrid *m_editorGrid = nullptr;

            /// the translate gizmo
            SmartPtr<TranslateManipulator> m_translateManipulator;

            /// the rotate gizmo
            SmartPtr<RotateManipulator> m_rotateManipulator;

            /// the scale gizmo
            SmartPtr<ScaleManipulator> m_scaleManipulator;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // AppRoot_h__
