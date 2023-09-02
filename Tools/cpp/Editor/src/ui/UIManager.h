#ifndef __UIManager_h__
#define __UIManager_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/System/Job.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {

        /** The editor's ui manager. */
        class UIManager : public ISharedObject
        {
        public:
            /** Widget ids. */
            enum class WidgetId
            {
                None,

                Exit,
                About,

                NewProjectDialog,
                OpenProjectDialog,
                SaveProjectDialog,
                NewSceneDialog,
                OpenSceneDialog,
                OpenSaveSceneDialog,
                OpenSaveAsSceneDialog,
                ImportJsonSceneDialog,
                LoadProceduralSceneDialog,
                SaveProceduralSceneDialog,

                BatchAllBtnId,
                AppPropertiesId,

                NewProjectId,
                OpenProjectId,
                SaveProjectId,

                NewSceneId,
                OpenSceneId,
                SaveId,
                SaveSceneAsId,
                SaveAllId,
                GenerateCMakeProjectId,
                CreatePackageId,
                CreateUnityBindings,
                ProjectSettingsId,

                LoadProceduralSceneId,
                SaveProceduralSceneId,

                AboutId,

                LuaEditConfigDialogId,

                ComponentsId,
                ResourcesId,
                InputWindowId,
                ProfilerWindowId,

                UndoId,
                RedoId,
                GotoId,

                ImportJsonSceneId,
                ImportUnityYamlId,

                ShowAllOverlaysId,
                HideAllOverlaysId,
                CreateOverlayTestId,
                CreateOverlayTextTestId,
                CreateOverlayButtonTestId,

                CreateRigidStaticMeshId,
                CreateRigidDynamicMeshId,
                CreateConstraintId,

                CreateProceduralTestId,
                GenerateSkyboxMaterialsId,

                OptimiseDatabasesId,
                CreateAssetFromDatabasesId,

                SetupMaterialsId,

                CreateDefaultCarId,
                CreateDefaultTruckId,

                ConvertCSharpId,

                PhysicsEnableId,

                ID_CustomizeToolbar,

                RunId,
                StopId,
                StatsId,
                ReloadScriptsId,
                ToggleEditorCameraId,

                AssetImportId,
                AssetReimportId,
                AssetDatabaseBuildId,

                FileBrowserId,
                CreateBoxTestId,

                CleanProjectId,

                ID_SampleItem,
            };

            UIManager();
            ~UIManager() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void update( time_interval t, time_interval dt );

            /** Returns a file path. */
            String saveEntity( const String &fileName );

            /** Returns a file path. */
            String saveScript( const String &fileName );

            SmartPtr<SceneWindow> getSceneWindow() const;
            void setSceneWindow( SmartPtr<SceneWindow> sceneWindow );

            SmartPtr<ActorWindow> getActorWindow() const;
            void setActorWindow( SmartPtr<ActorWindow> actorWindow );

            TerrainWindow *getTerrainWindow() const;
            void setTerrainWindow( TerrainWindow *val );

            PropertiesWindow *getPropertiesWindow() const;
            void setPropertiesWindow( PropertiesWindow *val );

            ApplicationFrame *getApplicationFrame() const;
            void setApplicationFrame( ApplicationFrame *val );

            void rebuildSceneTree();
            void rebuildActorTree();

            void rebuildResourceTree();

            void updateSelection();
            void updateActorSelection();
            void updateComponentSelection();

            FoliageWindow *getFoliageWindow() const;
            void setFoliageWindow( FoliageWindow *val );

            RoadFrame *getRoadWindow() const;
            void setRoadWindow( RoadFrame *val );

            HoudiniWindow *getHoudiniWindow() const;
            void setHoudiniWindow( HoudiniWindow *val );

            RenderWindow *getRenderWindow() const;
            void setRenderWindow( RenderWindow *val );

            RenderWindow *getGameWindow() const;
            void setGameWindow( RenderWindow *val );

            FileWindow *getFileWindow() const;
            void setFileWindow( FileWindow *val );

            SmartPtr<ProjectWindow> getProjectWindow() const;
            void setProjectWindow( SmartPtr<ProjectWindow> val );

            TextureWindow *getTextureWindow() const;
            void setTextureWindow( TextureWindow *textureWindow );

            MaterialWindow *getMaterialWindow() const;
            void setMaterialWindow( MaterialWindow *materialWindow );

            SmartPtr<ObjectWindow> getObjectWindow() const;
            void setObjectWindow( SmartPtr<ObjectWindow> objectWindow );

            SmartPtr<ObjectBrowserDialog> getObjectBrowserDialog() const;
            void setObjectBrowserDialog( SmartPtr<ObjectBrowserDialog> objectBrowserDialog );

            SmartPtr<ResourceDatabaseDialog> getResourceDatabaseDialog() const;
            void setResourceDatabaseDialog( SmartPtr<ResourceDatabaseDialog> resourceDatabaseDialog );

            SmartPtr<AboutDialog> getAboutDialog() const;
            void setAboutDialog( SmartPtr<AboutDialog> aboutDialog );

            SmartPtr<InputManagerWindow> getInputManagerWindow() const;

            void setInputManagerWindow( SmartPtr<InputManagerWindow> inputManagerWindow );

            SmartPtr<ProfilerWindow> getProfilerWindow() const;

            void setProfilerWindow( SmartPtr<ProfilerWindow> profilerWindow );

            bool isValid() const override;

        protected:
            class UpdateSelectionJob : public Job
            {
            public:
                UpdateSelectionJob() = default;
                ~UpdateSelectionJob() override = default;

                void execute() override;
            };

            class CUIMenuBarListener : public IEventListener
            {
            public:
                CUIMenuBarListener();
                ~CUIMenuBarListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                UIManager *getOwner() const;
                void setOwner( UIManager *owner );

            private:
                UIManager *m_owner = nullptr;
            };

            class ToolbarListener : public IEventListener
            {
            public:
                ToolbarListener() = default;
                ~ToolbarListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                UIManager *getOwner() const;

                void setOwner( UIManager *owner );

            private:
                UIManager *m_owner = nullptr;
            };

            class EventListener : public IEventListener
            {
            public:
                EventListener() = default;
                ~EventListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                UIManager *getOwner() const;

                void setOwner( UIManager *owner );

            private:
                UIManager *m_owner = nullptr;
            };

            SmartPtr<IEventListener> m_eventListener;

            SmartPtr<IJob> m_updateSelectionJob;

            SmartPtr<IFrameStatistics> m_frameStatistics;

            SmartPtr<AboutDialog> m_aboutDialog;

            ///
            RenderWindow *m_renderWindow = nullptr;

            RenderWindow *m_gameWindow = nullptr;

            FileWindow *m_fileWindow = nullptr;

            AtomicSmartPtr<ActorWindow> m_actorWindow;

            TerrainWindow *m_terrainWindow = nullptr;

            SmartPtr<ProjectWindow> m_projectWindow;

            PropertiesWindow *m_propertiesWindow = nullptr;
            ApplicationFrame *m_appFrame = nullptr;
            FoliageWindow *m_foliageWindow = nullptr;
            RoadFrame *m_roadWindow = nullptr;
            HoudiniWindow *m_houdiniWindow = nullptr;
            TextureWindow *m_textureWindow = nullptr;
            MaterialWindow *m_materialWindow = nullptr;

            SmartPtr<ResourceDatabaseDialog> m_resourceDatabaseDialog;
            SmartPtr<ObjectBrowserDialog> m_objectBrowserDialog;
            SmartPtr<ObjectWindow> m_objectWindow;

            SmartPtr<SceneWindow> m_sceneWindow;

            SmartPtr<InputManagerWindow> m_inputManagerWindow;

            SmartPtr<ProfilerWindow> m_profilerWindow;

            SmartPtr<IEventListener> m_menubarListener;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // GUIManager_h__
