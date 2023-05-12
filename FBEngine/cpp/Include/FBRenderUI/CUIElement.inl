namespace fb
{
    namespace ui
    {

        template <class T>
        CUIElement<T>::CUIElement() :
            m_parent( nullptr ),
            m_userData( nullptr ),
            m_isEnabled( false ),
            m_hasFocus( false ),
            m_isHighlighted( false ),
            m_isSelected( false )
        {
            m_name = String( "GUIElement" ) + StringUtil::toString( m_nextGeneratedNameExt++ );

            m_layout = nullptr;

            m_children = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>();
        }

        template <class T>
        CUIElement<T>::~CUIElement()
        {
            unload( nullptr );
        }

        template <class T>
        SmartPtr<ISharedObject> CUIElement<T>::getOwner() const
        {
            return m_owner;
        }

        template <class T>
        void CUIElement<T>::setOwner( SmartPtr<ISharedObject> owner )
        {
            m_owner = owner;
        }

        template <class T>
        bool CUIElement<T>::getSameLine() const
        {
            return m_sameLine;
        }

        template <class T>
        void CUIElement<T>::setSameLine( bool sameLine )
        {
            m_sameLine = sameLine;
        }

    }  // namespace ui
}  // namespace fb
