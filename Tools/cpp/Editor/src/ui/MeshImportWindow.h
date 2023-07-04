#ifndef MeshImportWindow_h__
#define MeshImportWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {

        class MeshImportWindow : public BaseWindow
        {
        public:
            enum
            {
                ID_RoadFrameCreateRoad,
                ID_RoadFrameAddRoadNode,
                ID_RoadFrameRoadList,
                ID_RoadFrameProperties,
            };

            MeshImportWindow();
            ~MeshImportWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setCreateRoadBtnToggled( bool status );
            bool isCreateRoadBtnToggled();

            void setLastRoadNodeCreated( SmartPtr<scene::IActor> riverNode );
            SmartPtr<scene::IActor> getLastRoadNodeCreated() const;

        private:
            class ElementListener : public SharedObject<IEventListener>
            {
            public:
                ElementListener() = default;
                ~ElementListener() = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                MeshImportWindow *getOwner() const;

                void setOwner( MeshImportWindow *owner );

            private:
                MeshImportWindow *m_owner = nullptr;
            };

            void OnAddRoadNodeToggled();
            void OnPropertyGridChange();

            void OnCreateRoad();
            void OnDestroyRoad();
            void OnSelectRoad();

            void populateFoliageLayers();
            void populateLayerProperties();

            SmartPtr<scene::IActor> m_lastCreatedEntity;

            bool m_isCreateRoadBtnToggled;

            SmartPtr<ui::IUIDropdown> m_dropdown;

            SmartPtr<ui::IUITreeCtrl> m_tree;
            SmartPtr<IEventListener> m_treeListener;
            SmartPtr<render::IMaterial> m_material;

            SmartPtr<PropertiesWindow> m_propertiesWindow;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // MeshImportWindow_h__
