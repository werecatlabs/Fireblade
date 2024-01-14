#ifndef ProjectAssetsWindow_h__
#define ProjectAssetsWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/EditorWindow.h"
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/UI/IUIDragSource.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>
#include <FBCore/System/Job.h>

namespace fb
{
    namespace editor
    {
        class ProjectAssetsWindow : public EditorWindow
        {
        public:
            enum class MenuId
            {
                AddMaterial,
                AddScript,
                AddTerrainDirector,
                Remove,
                Import,
                Reimport,

                Count
            };

            ProjectAssetsWindow();
            ~ProjectAssetsWindow() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<ui::IUITreeNode> addFolderToTree( SmartPtr<ui::IUITreeNode> parent,
                                                       SmartPtr<IFolderExplorer> listing );

            void build();

            void saveTreeState();

            void restoreTreeState();

            SmartPtr<ui::IUIWindow> getParentWindow() const override;
            void setParentWindow( SmartPtr<ui::IUIWindow> parentWindow ) override;

            bool isValid() const override;

            SmartPtr<ui::IUITreeCtrl> getTree() const;

            void setTree( SmartPtr<ui::IUITreeCtrl> tree );

        protected:
            class TreeCtrlListener : public IEventListener
            {
            public:
                TreeCtrlListener() = default;
                ~TreeCtrlListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                ProjectAssetsWindow *getOwner() const;
                void setOwner( ProjectAssetsWindow *val );

            private:
                ProjectAssetsWindow *m_owner = nullptr;
            };

            class WindowListener : public IEventListener
            {
            public:
                WindowListener() = default;
                ~WindowListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                ProjectAssetsWindow *getOwner() const;
                void setOwner( ProjectAssetsWindow *val );

            private:
                ProjectAssetsWindow *m_owner = nullptr;
            };

            class DragSource : public ui::IUIDragSource
            {
            public:
                DragSource() = default;
                ~DragSource() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                String handleDrag( const Vector2I &position, SmartPtr<ui::IUIElement> element );

                ProjectAssetsWindow *getOwner() const;
                void setOwner( ProjectAssetsWindow *val );

            private:
                ProjectAssetsWindow *m_owner = nullptr;
            };

            class DropTarget : public ui::IUIDropTarget
            {
            public:
                DropTarget() = default;
                ~DropTarget() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                bool handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> src,
                                 SmartPtr<ui::IUIElement> dst, const String &data );

                ProjectAssetsWindow *getOwner() const;

                void setOwner( ProjectAssetsWindow *owner );

            private:
                ProjectAssetsWindow *m_owner = nullptr;
            };

            class BuildTreeJob : public Job
            {
            public:
                BuildTreeJob() = default;
                ~BuildTreeJob() override = default;

                void execute() override;

                SmartPtr<ProjectAssetsWindow> getOwner() const;

                void setOwner( SmartPtr<ProjectAssetsWindow> owner );

            private:
                SmartPtr<ProjectAssetsWindow> m_owner;
            };

            void buildTree();

            void handleTreeSelectionActivated( SmartPtr<ui::IUITreeNode> node );
            void handleTreeNodeDoubleClicked( SmartPtr<ui::IUITreeNode> node );

            void OnActivateItem();

            int getItemState( String itemName );

            void saveItemState();

            void restoreItemState();

            String getSelectedPath() const;

            SmartPtr<ui::IUIWindow> m_window;
            SmartPtr<ui::IUIWindow> m_sceneWindow;

            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<IEventListener> m_treeListener;

            SmartPtr<IEventListener> m_menuListener;
            SmartPtr<ui::IUIMenu> m_applicationMenu;
            SmartPtr<ui::IUIMenu> m_applicationAddMenu;

            SmartPtr<ISharedObject> m_selectedObject;

            SmartPtr<ISharedObject> m_selectedEntity;

            //SmartPtr<TemplateFilter> m_parentFilter;

            SmartPtr<ui::IUIWindow> m_parentWindow;

            std::map<String, bool> treeState;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // EntityWindow_h__
