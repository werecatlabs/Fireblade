#ifndef __SceneWindow_h__
#define __SceneWindow_h__

#include "ui/EditorWindow.h"
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>
#include <FBCore/Interface/UI/IUIDragSource.h>

namespace fb
{
    namespace editor
    {
        class SceneWindow : public EditorWindow
        {
        public:
            /**
             * Defines the menu IDs used in the SceneWindow class.
             */
            enum class MenuId
            {
                ADD_SCRIPT_ID,
                ADD_NEW_ENTITY,
                ADD_SKYBOX,
                ADD_NEW_TERRAIN,
                ADD_CAMERA,
                ADD_CAR,
                ADD_PARTICLESYSTEM,
                ADD_PARTICLESYSTEM_SMOKE,
                ADD_PARTICLESYSTEM_SAND,
                ADD_PLANE,
                ADD_CUBE,
                ADD_CUBE_MESH,
                ADD_CUBEMAP,
                ADD_PHYSICS_CUBE,
                ADD_DIRECTIONAL_LIGHT,
                ADD_POINT_LIGHT,

                ADD_BUTTON,
                ADD_SIMPLE_BUTTON,
                ADD_CANVAS,
                ADD_CHECKBOX,
                ADD_PANEL,
                ADD_SLIDER,
                ADD_TEXT,
                ADD_TOGGLE_BUTTON,


                SCENE_REMOVE_ACTOR
            };

            /**
             * Constructor.
             */
            SceneWindow();

            /**
             * Constructor.
             *
             * @param parent The parent window.
             */
            SceneWindow( SmartPtr<ui::IUIWindow> parent );

            /**
             * Destructor.
             */
            ~SceneWindow() override;

            /**
             * Loads the specified data.
             *
             * @param data The data to load.
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * Unloads the specified data.
             *
             * @param data The data to unload.
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * Builds the scene tree.
             */
            void buildTree();

	        void addObjectToTree( SmartPtr<ISharedObject> object, SmartPtr<ui::IUITreeNode> parentNode );

            /**
             * Adds the specified actor to the scene tree.
             *
             * @param actor The actor to add.
             * @param parentNode The parent node of the actor.
             */
            void addActorToTree( SmartPtr<scene::IActor> actor, SmartPtr<ui::IUITreeNode> parentNode );

            /**
             * Saves the state of the scene tree.
             */
            void saveTreeState();

            /**
             * Restores the state of the scene tree.
             */
            void restoreTreeState();

            /**
             * Gets the selected object.
             *
             * @return The selected object.
             */
            SmartPtr<ISharedObject> getSelectedObject() const;

            /**
             * Sets the selected object.
             *
             * @param selectedObject The object to select.
             */
            void setSelectedObject( SmartPtr<ISharedObject> selectedObject );

            /**
             * Deselects all objects.
             */
            void deselectAll();

            /**
             * Determines whether the window is valid.
             *
             * @return true if the window is valid; otherwise, false.
             */
            bool isValid() const override;

            /**
             * Gets the scene tree control.
             *
             * @return The scene tree control.
             */
            SmartPtr<ui::IUITreeCtrl> getTree() const;

            /**
             * Sets the scene tree control.
             *
             * @param tree The tree control to set.
             */
            void setTree( SmartPtr<ui::IUITreeCtrl> tree );

            /**
             * Gets the state of the scene tree.
             *
             * @return The state of the scene tree.
             */
            SharedPtr<std::map<String, bool>> getTreeState() const;

            /**
             * Sets the state of the scene tree.
             *
             * @param treeState The state of the scene tree.
             */
            void setTreeState( SharedPtr<std::map<String, bool>> treeState );

            SmartPtr<ICommand> getDragDropActorCmd() const;

            void setDragDropActorCmd( SmartPtr<ICommand> dragDropActorCmd );

        protected:
            class ApplicationEventListener : public IEventListener
            {
            public:
                ApplicationEventListener();
                ~ApplicationEventListener() override;

                void unload( SmartPtr<ISharedObject> data );

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                SmartPtr<SceneWindow> getOwner() const;

                void setOwner( SmartPtr<SceneWindow> owner );

            private:
                AtomicSmartPtr<SceneWindow> m_owner;
            };

            class TreeCtrlListener : public IEventListener
            {
            public:
                TreeCtrlListener();
                ~TreeCtrlListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                SceneWindow *getOwner() const;
                void setOwner( SceneWindow *owner );

            private:
                SceneWindow *m_owner = nullptr;
            };

            class SceneWindowListener : public IEventListener
            {
            public:
                SceneWindowListener() = default;
                ~SceneWindowListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                SceneWindow *getOwner() const;
                void setOwner( SceneWindow *val );

            private:
                SceneWindow *m_owner = nullptr;
            };

            class PromptListener : public IEventListener
            {
            public:
                PromptListener();

                ~PromptListener();

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                SceneWindow *getOwner() const;

                void setOwner( SceneWindow *owner );

            private:
                SceneWindow *m_owner = nullptr;
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

                SceneWindow *getOwner() const;

                void setOwner( SceneWindow *owner );

            private:
                SceneWindow *m_owner = nullptr;
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

                SceneWindow *getOwner() const;

                void setOwner( SceneWindow *owner );

            private:
                SceneWindow *m_owner = nullptr;
            };

            void handleWindowClicked();
            void handleTreeSelectionChanged( SmartPtr<ui::IUITreeNode> node );

            s32 getItemState( const String &itemName ) const;
            void saveItemState( SmartPtr<ui::IUITreeNode> parent, SmartPtr<ui::IUITreeNode> node );
            void restoreItemState( SmartPtr<ui::IUITreeNode> parent, SmartPtr<ui::IUITreeNode> node,
                                   bool parentWasNew );

            RecursiveMutex m_buildTreeMutex;
            RecursiveMutex m_treeStateMutex;

            SmartPtr<ui::IUIWindow> m_window;
            SmartPtr<ui::IUIWindow> m_sceneWindow;

            SmartPtr<IEventListener> m_applicationEventListener;

            SmartPtr<ui::IUITextEntry> m_inputText;
            SmartPtr<IEventListener> m_promptListener;

            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<IEventListener> m_treeListener;

            SmartPtr<ui::IUIMenu> m_applicationMenu;
            SmartPtr<ui::IUIMenu> m_applicationAddMenu;

            SmartPtr<IEventListener> m_menuListener;

            SmartPtr<ISharedObject> m_selectedObject;

            SmartPtr<ISharedObject> m_selectedEntity;

            SmartPtr<ICommand> m_dragDropActorCmd;

            time_interval m_nodeSelectTime = 0.0;

            AtomicSharedPtr<std::map<String, bool>> m_treeState;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // EntityWindow_h__
