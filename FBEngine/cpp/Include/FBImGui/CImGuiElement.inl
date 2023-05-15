namespace fb
{
    namespace ui
    {

        template <class T>
        s32 CImGuiElement<T>::getElementId() const
        {
            return m_itemId;
        }

        template <class T>
        void CImGuiElement<T>::setElementId( s32 itemId )
        {
            m_itemId = itemId;
        }

        template <class T>
        SmartPtr<IUIElement> CImGuiElement<T>::getParent() const
        {
            return m_parent;
        }

        template <class T>
        void CImGuiElement<T>::setParent( SmartPtr<IUIElement> parent )
        {
            m_parent = parent;
        }

        template <class T>
        SmartPtr<ISharedObject> CImGuiElement<T>::getOwner() const
        {
            return m_owner;
        }

        template <class T>
        void CImGuiElement<T>::setOwner( SmartPtr<ISharedObject> owner )
        {
            m_owner = owner;
        }

        template <class T>
        SmartPtr<Properties> CImGuiElement<T>::getProperties() const
        {
            return nullptr;
        }

        template <class T>
        void CImGuiElement<T>::setProperties( SmartPtr<Properties> properties )
        {
        }

        template <class T>
        Array<SmartPtr<ISharedObject>> CImGuiElement<T>::getChildObjects() const
        {
            return Array<SmartPtr<ISharedObject>>();
        }

        template <class T>
        bool CImGuiElement<T>::isDragDropSource() const
        {
            return m_dragDropSource;
        }

        template <class T>
        void CImGuiElement<T>::setDragDropSource( bool dragDropSource )
        {
            m_dragDropSource = dragDropSource;
        }

        template <class T>
        SmartPtr<IUIDragSource> CImGuiElement<T>::getDragSource() const
        {
            return m_dragSource;
        }

        template <class T>
        void CImGuiElement<T>::setDragSource( SmartPtr<IUIDragSource> dragSource )
        {
            m_dragSource = dragSource;
        }

        template <class T>
        SmartPtr<IUIDropTarget> CImGuiElement<T>::getDropTarget() const
        {
            return m_dropTarget;
        }

        template <class T>
        void CImGuiElement<T>::setDropTarget( SmartPtr<IUIDropTarget> dropTarget )
        {
            m_dropTarget = dropTarget;
        }

        template <class T>
        u32 CImGuiElement<T>::getOrder() const
        {
            return 0;
        }

        template <class T>
        void CImGuiElement<T>::setOrder( u32 order )
        {
        }

        template <class T>
        SmartPtr<IStateContext> CImGuiElement<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <class T>
        void CImGuiElement<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <class T>
        SmartPtr<IStateListener> CImGuiElement<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <class T>
        void CImGuiElement<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        template <class T>
        bool CImGuiElement<T>::getSameLine() const
        {
            return m_sameLine;
        }

        template <class T>
        void CImGuiElement<T>::setSameLine( bool sameLine )
        {
            m_sameLine = sameLine;
        }

        template <class T>
        ColourF CImGuiElement<T>::getColour() const
        {
            return ColourF::White;
        }

        template <class T>
        void CImGuiElement<T>::setColour( const ColourF &colour )
        {
        }

        template <class T>
        bool CImGuiElement<T>::getHandleInputEvents() const
        {
            return m_handleInputEvents;
        }

        template <class T>
        void CImGuiElement<T>::setHandleInputEvents( bool handleInputEvents )
        {
            m_handleInputEvents = handleInputEvents;
        }

        template <class T>
        void CImGuiElement<T>::updateZOrder()
        {
        }

        template <class T>
        void CImGuiElement<T>::setChildren( SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p )
        {
            m_children = p;
        }

    }  // end namespace ui
}  // end namespace fb
