namespace fb
{
    namespace ui
    {

        template <class T>
        ImGuiWindow<T>::ImGuiWindow()
        {
            static auto ext = 0;
            auto label = "Window" + StringUtil::toString( ext++ );
            setLabel( label );
        }

        template <class T>
        ImGuiWindow<T>::~ImGuiWindow()
        {
            unload( nullptr );
        }

        template <class T>
        void ImGuiWindow<T>::load( SmartPtr<ISharedObject> data )
        {
            CImGuiElement<T>::load( data );
        }

        template <class T>
        void ImGuiWindow<T>::unload( SmartPtr<ISharedObject> data )
        {
            CImGuiElement<T>::unload( data );
        }

        template <class T>
        void ImGuiWindow<T>::setLabel( const String &label )
        {
            m_label = label;
        }

        template <class T>
        String ImGuiWindow<T>::getLabel() const
        {
            return m_label;
        }

        template <class T>
        void ImGuiWindow<T>::setContextMenu( SmartPtr<IUIMenu> menu )
        {
            m_menu = menu;
        }

        template <class T>
        SmartPtr<IUIMenu> ImGuiWindow<T>::getContextMenu() const
        {
            return m_menu;
        }

        template <class T>
        bool ImGuiWindow<T>::hasBorder() const
        {
            return m_hasBorder;
        }

        template <class T>
        void ImGuiWindow<T>::setHasBorder( bool border )
        {
            m_hasBorder = border;
        }

        template <class T>
        bool ImGuiWindow<T>::isDocked() const
        {
            return m_isDocked;
        }

        template <class T>
        void ImGuiWindow<T>::setDocked( bool docked )
        {
            m_isDocked = docked;
        }

    }  // end namespace ui
}  // end namespace fb