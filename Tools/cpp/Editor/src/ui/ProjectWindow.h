#ifndef EntityWindow_h__
#define EntityWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace editor
    {
        
        class ProjectWindow : public BaseWindow
        {
        public:
            enum
            {
                MENU_ADD_SCRIPT_ID,
                MENU_ADD_BUILDER_ID,
                MENU_ADD_COMPONENT_ID,
                MENU_ADD_FSM_ID,
                ENTITY_TREE_ID,

                MENU_ADD_CAMERA,
                MENU_REMOVE_CAMERA,

                APPLICATION_MENU_ADD_EVENT,

                PW_ADD_NEW_ENTITY,
                PW_ADD_EXISTING_ENTITY,

                PW_ADD_ENT_SCENE_NODE,
                PW_ADD_ENT_GFX_OBJ,
                PW_REMOVE_ENT_GFX_OBJ,
                PW_REMOVE_ENT_SCENE_NODE,

                PW_ADD_ENT_SHAPE,
                PW_ADD_ENT_BODY,
                PW_REMOVE_ENT_SHAPE,
                PW_REMOVE_ENT_BODY,

                PW_ADD_ENT_SOUND,
                PW_REMOVE_ENT_SOUND,

                PW_ENTITY_REMOVE,
                PW_GENERATE_SCRIPT,

                PW_ADD_NEW_GUI,
                PW_ADD_EXISTING_GUI,

                PW_COMPONENT_REMOVE,

                PW_FSM_REMOVE,

                PW_EVENT_ADD,
                PW_EVENT_REMOVE,
                PW_EVENT_MODIFY,

                PW_FSM_EVENT_ADD,
                PW_FSM_EVENT_REMOVE,
                PW_FSM_EVENT_MODIFY,

                PW_FSM_STATE_ADD,
                PW_FSM_STATE_REMOVE,

                PW_ENT_SCRIPT_REMOVE,

                PW_ADD_NEW_SCRIPT,
                PW_ADD_EXISTING_SCRIPT,
                PW_ADD_SCRIPT_FOLDER,
                PW_REMOVE_SCRIPT,
                PW_RENAME_SCRIPT,

                PW_ADD_TERRAIN,
                PW_ADD_MESH,
                PW_ADD_DESTRUCTIBLE_MESH,
                PW_ADD_PARTICLE_SYSTEM,

                PW_ADD_SCENE,

                PW_ADD_VEHICLE_4_WHEEL,
                PW_ADD_VEHICLE_AIRPLANE,
                PW_ADD_VEHICLE_HELICOPTER,
                PW_ADD_VEHICLE_TANK,

                PW_ADD_GOAL,
                PW_ADD_EVALUATOR,

                PW_ADD_MAP,
                PW_MAP_REFRESH,

                PW_ADD_MATERIAL,
                PW_ADD_TECHNIQUE,
            };

            ProjectWindow();
            ~ProjectWindow();

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void update( time_interval t, time_interval dt );

            // void addFolderToTree(wxTreeItemId parent, SmartPtr<IFolderExplorer> listing);

            void buildTree();

            void saveTreeState();

            void restoreTreeState();

            bool isValid() const;

        protected:
            void handleTreeSelectionChanged();

            void OnContextMenu();

            void OnActivateItem();

            void populateCameras();

            void populateApplication();

            void populateProjectResources();

            int getItemState( String itemName );

            void saveItemState();

            void restoreItemState();

            SmartPtr<ProjectAssetsWindow> m_projectAssetsWindow;
            SmartPtr<FileWindow> m_fileWindow;

            SmartPtr<ISharedObject> m_selectedObject;
            SmartPtr<ISharedObject> m_selectedEntity;
            //SmartPtr<TemplateFilter> m_parentFilter;

            std::map<String, bool> treeState;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // EntityWindow_h__
