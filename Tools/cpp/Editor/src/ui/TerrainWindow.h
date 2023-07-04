#ifndef _TerrainFrame_H
#define _TerrainFrame_H

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {
        class TerrainWindow : public BaseWindow
        {
        public:
            enum class WidgetIds
            {
                ID_TerrainCreateTerrainBtn,
                ID_TerrainRaiseBtn,
                ID_TerrainLowerBtn,
                ID_TerrainMinimumBtn,
                ID_TerrainMaximumBtn,
                ID_TerrainsetHeightBtn,
                ID_TerrainPaintBtn,
                ID_TerrainEraseBtn,
                ID_TerrainBlendBtn,
                ID_TerrainFrameToolProperties,
                ID_TerrainBrushImage,
                ID_TerrainSplatImage,
                ID_AddTerrainSplatImage,
            };

            TerrainWindow();
            ~TerrainWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<ui::IUITerrainEditor> getTerrainEditor() const;

            void setTerrainEditor( SmartPtr<ui::IUITerrainEditor> terrainEditor );

            void updateSelection() override;

        private:
            class UIElementListener : public SharedObject<IEventListener>
            {
            public:
                UIElementListener() = default;
                ~UIElementListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                TerrainWindow *getOwner() const;

                void setOwner( TerrainWindow *owner );

            private:
                TerrainWindow *m_owner = nullptr;
            };

            // events
            void OnRaiseBtnToggled();
            void OnLowerBtnToggled();
            void OnMinimumBtnToggled();
            void OnMaximumBtnToggled();
            void OnsetHeightBtnToggled();

            void OnPaintBtnToggled();
            void OnEraseBtnToggled();
            void OnBlendBtnToggled();

            void OnAddTextureBtn();

            void OnComboBox();

            void OnPropertyGridChangeTool();

            // void OnSelectTool(TerrainToolPtr tool);

            void populateToolProperties();

            void loadImages();

            void addComboBoxBitmap();

            SmartPtr<IEventListener> m_terrainEditorListener;
            SmartPtr<ui::IUITerrainEditor> m_terrainEditor;

            ITerrainManagerListener *m_terrainManagerListener = nullptr;

            // wxPropertyGrid* m_toolPropertyGrid = nullptr;

            // wxButton*		m_createTerrainBtn = nullptr;
            // wxToggleButton* m_raiseBtn = nullptr;
            // wxToggleButton* m_lowerBtn = nullptr;
            // wxToggleButton* m_minimumBtn = nullptr;
            // wxToggleButton* m_maximumBtn = nullptr;
            // wxToggleButton* m_setHeightBtn = nullptr;
            // wxToggleButton* m_paintBtn = nullptr;
            // wxToggleButton* m_eraseBtn = nullptr;
            // wxToggleButton* m_blendBtn = nullptr;
            // wxButton*		m_addTextureBtn = nullptr;
            // wxToggleButton* m_pButton = nullptr;

            // wxBitmapComboBox *m_brushesCombobox = nullptr;
            // wxBitmapComboBox *m_splatMapsCombobox = nullptr;

            bool m_bIsPortalAddToggled = false;  // value to know if a button is toggled

            Array<String> m_brushes;
            Array<String> m_splatMaps;
        };
    }  // end namespace editor
}  // end namespace fb

#endif
