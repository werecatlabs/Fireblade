#ifndef __ApplicationFrame_H
#define __ApplicationFrame_H

#include <GameEditorPrerequisites.h>
#include <FBCore/Core/Map.h>
#include <FBCore/Core/Singleton.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    namespace editor
    {

        class ApplicationFrame : public SharedObject<ISharedObject>
        {
        public:
            // IDs for the controls and the menu commands
            enum
            {
                Minimal_Quit,
                Minimal_About,

                BatchAllBtnId,
                AppPropertiesId,

                NewProjectId,
                OpenProjectId,
                SaveProjectId,

                NewSceneId,
                OpenSceneId,
                SaveId,
                SaveAllId,
                GenerateCMakeProjectId,
                CreatePackageId,
                ProjectSettingsId,

                LoadProceduralSceneId,
                SaveProceduralSceneId,

                LuaEditConfigDialogId,

                UndoId,
                RedoId,
                GotoId,

                ImportJsonSceneId,

                ShowAllOverlaysId,
                HideAllOverlaysId,
                CreateOverlayTestId,
                CreateOverlayTextTestId,
                CreateOverlayButtonTestId,

                CreateRigidStaticMeshId,
                CreateRigidDynamicMeshId,
                CreateConstraintId,

                CreateProceduralTestId,

                CreateDefaultCarId,
                CreateDefaultTruckId,

                ConvertCSharpId,

                PhysicsEnableId,

                ID_CustomizeToolbar,

                RunId,

                AssetImportId,
                AssetReimportId,

                ID_SampleItem,
            };

            ApplicationFrame();
            ~ApplicationFrame();

            void update( s32 task, time_interval t, time_interval dt );

            //LuaEdit *openScript( SmartPtr<ScriptTemplate> scriptTemplate );

            void removeLuaTextWindow( LuaEdit *luaTextWindow );

            void OnOpenProject();
            void OnSaveProject();

            void createNewScene();
            void saveApplication();

            bool saveProject();

            void OnOpen();
            void OnSave();
            void OnSaveAll();

            void OnGoto();
            void OnEdit();

            void OnUndo();
            void OnRedo();
            void OnOpenScene();

            void createNewProject();
            void OnQuit();
            void OnAbout();
            // void OnPropertyChange();
            void OnRun();

            void OnLuaEditConfigDialog();

            void OnPhysicsEnable();

            void generateCMakeProject();
            void packageProject();
            void projectSettings();

            void importAssets();
            void reimportAssets();

            void loadProceduralScene();
            void saveProceduralScene();

            void OnClose();

            void toolbarButtonClick();

        private:
            // void DosetSize(int x, int y,
            //	int width, int height,
            //	int WXUNUSED(sizeFlags = wxSIZE_AUTO));

            void createFileMenu();
            void createWindows();

            void createToolbars();

            void importJsonScene();

            void showAllOverlays();
            void hideAllOverlays();

            void createOverlayPanelTest();
            void createOverlayTextTest();
            void createOverlayButtonTest();

            void createRigidStaticMesh();
            void createRigidDynamicMesh();
            void createConstraint();

            void createProceduralTest();

            void createDefaultCar();
            void createDefaultTruck();

            void createConvertCSharp();

            // wxAuiManager*		m_aui = nullptr;

            // wxAuiNotebook*		m_bookProject = nullptr;
            // wxAuiNotebook*		m_bookProps = nullptr;
            // wxAuiNotebook*		m_bookComponents = nullptr;
            // wxAuiNotebook*		m_bookMain = nullptr;
            // wxAuiNotebook*		m_bookAnimation = nullptr;
            // wxAuiNotebook*		m_bookFiles = nullptr;

            // OutputWindow*		m_outputWindow = nullptr;
            // ActorWindow* m_actorWindow = nullptr;

            typedef Map<String, LuaEdit *> LuaEditMap;
            LuaEditMap m_luaEditMap;

            Array<SmartPtr<BaseWindow>> m_windows;
        };

    }  // end namespace editor
}  // end namespace fb

#endif
