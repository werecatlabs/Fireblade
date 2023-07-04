#ifndef ObjectWindow_h__
#define ObjectWindow_h__

#include "ui/BaseWindow.h"

namespace fb
{
    namespace editor
    {
        class ObjectWindow : public BaseWindow
        {
        public:
            enum class ObjectType
            {
                None,
                Actor,
                Resource,
                Mesh,
                Material,
                MaterialNode,
                Terrain,
                FileUnknown,

                Count
            };

            enum
            {
                CANCEL_BTN_ID,
            };

            ObjectWindow();
            ObjectWindow( SmartPtr<ui::IUIWindow> parent );
            ~ObjectWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<ui::IUIWindow> getWindow() const;
            void setWindow( SmartPtr<ui::IUIWindow> parent );

            void updateSelection() override;

        protected:
            SmartPtr<ui::IUIWindow> m_window = nullptr;

            SmartPtr<ActorWindow> m_actorWindow;
            SmartPtr<MaterialWindow> m_materialWindow;
            SmartPtr<PropertiesWindow> m_propertiesWindow;
            SmartPtr<FileViewWindow> m_fileViewWindow;
            SmartPtr<ResourceWindow> m_resourceWindow;

            ObjectType m_objectType = ObjectType::None;
            ObjectType m_resourceType = ObjectType::None;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // ObjectWindow_h__
