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
        class ImGuiWindowT : public CImGuiElement<T>
        {
        public:
            ImGuiWindowT();
            ~ImGuiWindowT() override;

            void update() override;

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

            FB_CLASS_REGISTER_TEMPLATE_DECL( ImGuiWindowT, T );

        protected:
            String m_label;
            SmartPtr<IUIMenu> m_menu;
            bool m_hasBorder = false;
            bool m_isDocked = false;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::ui, ImGuiWindowT, T, CImGuiElement<T> );

        template <class T>
        void ImGuiWindowT<T>::update()
        {
        }

        template <class T>
        ImGuiWindowT<T>::ImGuiWindowT()
        {
            static auto ext = 0;
            auto label = "Window" + StringUtil::toString( ext++ );
            setLabel( label );
        }

        template <class T>
        ImGuiWindowT<T>::~ImGuiWindowT()
        {
            unload( nullptr );
        }

        template <class T>
        void ImGuiWindowT<T>::load( SmartPtr<ISharedObject> data )
        {
            CImGuiElement<T>::load( data );
        }

        template <class T>
        void ImGuiWindowT<T>::unload( SmartPtr<ISharedObject> data )
        {
            CImGuiElement<T>::unload( data );
        }

        template <class T>
        void ImGuiWindowT<T>::setLabel( const String &label )
        {
            m_label = label;
        }

        template <class T>
        String ImGuiWindowT<T>::getLabel() const
        {
            return m_label;
        }

        template <class T>
        void ImGuiWindowT<T>::setContextMenu( SmartPtr<IUIMenu> menu )
        {
            m_menu = menu;
        }

        template <class T>
        SmartPtr<IUIMenu> ImGuiWindowT<T>::getContextMenu() const
        {
            return m_menu;
        }

        template <class T>
        bool ImGuiWindowT<T>::hasBorder() const
        {
            return m_hasBorder;
        }

        template <class T>
        void ImGuiWindowT<T>::setHasBorder( bool border )
        {
            m_hasBorder = border;
        }

        template <class T>
        bool ImGuiWindowT<T>::isDocked() const
        {
            return m_isDocked;
        }

        template <class T>
        void ImGuiWindowT<T>::setDocked( bool docked )
        {
            m_isDocked = docked;
        }

    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiWindow_h__
