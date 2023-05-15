#ifndef __ImGuiWindow_h__
#define __ImGuiWindow_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIMenu.h>

namespace fb
{
    namespace ui
    {

        template <class T>
        class ImGuiWindow : public CImGuiElement<T>
        {
        public:
            ImGuiWindow();
            ~ImGuiWindow() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IUIWindow::setLabel */
            void setLabel( const String &label ) override;

            /** @copydoc IUIWindow::getLabel */
            String getLabel() const override;

            /** @copydoc IUIWindow::setContextMenu */
            void setContextMenu( SmartPtr<IUIMenu> menu ) override;

            /** @copydoc IUIWindow::getContextMenu */
            SmartPtr<IUIMenu> getContextMenu() const override;

            bool hasBorder() const override;
            void setHasBorder( bool border ) override;

            bool isDocked() const override;
            void setDocked( bool docked ) override;

        protected:
            String m_label;
            SmartPtr<IUIMenu> m_menu;
            bool m_hasBorder = false;
            bool m_isDocked = false;
        };
        
    }  // end namespace ui
}  // end namespace fb

#include <FBImGui/ImGuiWindow.inl>

#endif  // ImGuiWindow_h__
