#ifndef __ActorWindow_h__
#define __ActorWindow_h__

#include <GameEditorPrerequisites.h>
#include <ui/EditorWindow.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/IEventListener.h>

#include "GameEditorTypes.h"

namespace fb
{
    namespace editor
    {
        /**
         * @brief A window for managing actors and their components.
         */
        class ActorWindow : public EditorWindow
        {
        public:
            /**
             * @brief Enum class for widget ID.
             */
            enum class WidgetId
            {
                Label,
                Enabled,
                Visible,
                Static,
                AddComponent,
                RemoveComponent,
                Count
            };

            /**
             * @brief Constructor for ActorWindow class.
             * @param parent The parent window.
             */
            ActorWindow( SmartPtr<ui::IUIWindow> parent );

            /**
             * @brief Destructor for ActorWindow class.
             */
            ~ActorWindow() override;

            /**
             * @brief Load data for the window.
             * @param data The data to load.
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Unload data from the window.
             * @param data The data to unload.
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * @brief Set the state of the static checkbox.
             */
            void staticCheckboxState();

            /**
             * @brief Set the state of the visible checkbox.
             */
            void visibleCheckboxState();

            /**
             * @brief Add a component to the actor.
             */
            void addComponent();

            /**
             * @brief Remove a component from the actor.
             */
            void removeComponent();

            /**
             * @brief Update the window.
             * @param t The current time.
             * @param dt The elapsed time.
             */
            void update( time_interval t, time_interval dt );

            /**
             * @brief Build the actor tree.
             */
            void buildTree();

            /**
             * @brief Add a transform to the actor tree.
             * @param component The component to add.
             * @param label The label for the component.
             * @param node The tree node.
             */
            void addTransformToTree( SmartPtr<scene::ITransform> component, String label,
                                     SmartPtr<ui::IUITreeNode> node );

            /**
             * @brief Add a component to the actor tree.
             * @param component The component to add.
             * @param node The tree node.
             */
            void addComponentToTree( SmartPtr<scene::IComponent> component,
                                     SmartPtr<ui::IUITreeNode> node );

            /**
             * @brief Add an object to the actor tree.
             * @param object The object to add.
             * @param node The tree node.
             */
            void addObjectToTree( SmartPtr<ISharedObject> object, SmartPtr<ui::IUITreeNode> node );

            /**
             * @brief Add an actor to the actor tree.
             * @param actor The actor to add.
             * @param parentNode The parent tree node.
             */
            void addActorToTree( SmartPtr<scene::IActor> actor, SmartPtr<ui::IUITreeNode> parentNode );

            /**
             * @brief Add a resource to the actor tree.
             * @param resource The resource to add.
             * @param node The tree node.
             */
            void addResourceToTree( SmartPtr<IResource> resource, SmartPtr<ui::IUITreeNode> node );

            /**
             * @brief Add a material to the actor tree.
             * @param material The material to add.
             * @param node The tree node.
             */
            void addMaterialToTree( SmartPtr<render::IMaterial> material,
                                    SmartPtr<ui::IUITreeNode> node );

            /**
             * Saves the state of the tree.
             */
            void saveTreeState();

            /**
             * Restores the state of the tree.
             */
            void restoreTreeState();

            /**
             * Updates the selection.
             */
            void updateSelection() override;

            /**
             * Gets the transform window.
             * @return The transform window.
             */
            SmartPtr<TransformWindow> getTransformWindow() const;

            /**
             * Sets the transform window.
             * @param transformWindow The transform window.
             */
            void setTransformWindow( SmartPtr<TransformWindow> transformWindow );

            /**
             * Gets the properties window.
             * @return The properties window.
             */
            SmartPtr<PropertiesWindow> getPropertiesWindow() const;

            /**
             * Sets the properties window.
             * @param propertiesWindow The properties window.
             */
            void setPropertiesWindow( SmartPtr<PropertiesWindow> propertiesWindow );

        protected:
            class TerrainStateListener : public IStateListener
            {
            public:
                TerrainStateListener( ActorWindow *projectWindow );
                ~TerrainStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;

                void handleQuery( SmartPtr<IStateQuery> &query ) override;

            protected:
                ActorWindow *m_projectWindow = nullptr;
            };

            class UIElementListener : public IEventListener
            {
            public:
                UIElementListener();
                ~UIElementListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                ActorWindow *getOwner() const;
                void setOwner( ActorWindow *owner );

            private:
                ActorWindow *m_owner = nullptr;
            };

            void setActorName( const String &textStr );

            void updateObjectSelection( SmartPtr<ISharedObject> object );

            SmartPtr<ui::IUIWindow> m_actorWindow;
            SmartPtr<ui::IUIWindow> m_componentWindow;

            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<IEventListener> m_uiListener;

            SmartPtr<ui::IUILabelTogglePair> m_actorEnabled;
            SmartPtr<ui::IUILabelTogglePair> m_actorVisible;
            SmartPtr<ui::IUILabelTogglePair> m_actorStatic;

            SmartPtr<ui::IUIButton> m_addComponentButton;
            SmartPtr<ui::IUIButton> m_removeComponentButton;

            SmartPtr<ui::IUILabelTextInputPair> m_actorNamePair;

            SmartPtr<TransformWindow> m_transformWindow;
            SmartPtr<PropertiesWindow> m_propertiesWindow;
            SmartPtr<EventsWindow> m_eventsWindow;

            SmartPtr<MaterialWindow> m_materialWindow;
            SmartPtr<TerrainWindow> m_terrainWindow;

            SmartPtr<ISharedObject> m_selectedObject;

            SmartPtr<ISharedObject> m_selectedEntity;

            std::map<String, bool> treeState;

            ObjectType m_objectType = ObjectType::None;
            ObjectType m_resourceType = ObjectType::None;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // EntityWindow_h__
