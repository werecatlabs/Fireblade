namespace fb
{
    namespace render
    {

        template <class T>
        CGraphicsObjectOgre<T>::CGraphicsObjectOgre()
        {
        }

        template <class T>
        CGraphicsObjectOgre<T>::~CGraphicsObjectOgre()
        {
            CGraphicsObject<T>::unload( nullptr );
        }

        template <class T>
        void CGraphicsObjectOgre<T>::setMaterialName( [[maybe_unused]] const String &materialName,
                                                      [[maybe_unused]] s32 index )
        {
        }

        template <class T>
        String CGraphicsObjectOgre<T>::getMaterialName( [[maybe_unused]] s32 index ) const
        {
            return StringUtil::EmptyString;
        }

        template <class T>
        SmartPtr<IGraphicsObject> CGraphicsObjectOgre<T>::clone( const String &name ) const
        {
            return nullptr;
        }

        template <class T>
        void CGraphicsObjectOgre<T>::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

        template <class T>
        void CGraphicsObjectOgre<T>::attachToParent( SmartPtr<ISceneNode> parent )
        {
        }

        template <class T>
        void CGraphicsObjectOgre<T>::detachFromParent( SmartPtr<ISceneNode> parent )
        {
        }

        template <class T>
        Ogre::MovableObject *CGraphicsObjectOgre<T>::getMovable() const
        {
            return m_movable;
        }

        template <class T>
        void CGraphicsObjectOgre<T>::setMovable( Ogre::MovableObject *movable )
        {
            m_movable = movable;
            m_movable->setUserAny( this );
        }

        template <class T>
        void CGraphicsObjectOgre<T>::StateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        template <class T>
        void CGraphicsObjectOgre<T>::StateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            auto graphicsObjectState = fb::static_pointer_cast<GraphicsObjectState>( state );
            auto visible = graphicsObjectState->isVisible();

            if( auto owner = getOwner() )
            {
                if( auto movable = owner->getMovable() )
                {
                    auto mask = graphicsObjectState->getVisibilityMask();
                    movable->setVisibilityFlags( mask );
                    movable->setVisible( visible );
                }
            }
        }

        template <class T>
        void CGraphicsObjectOgre<T>::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        template <class T>
        CGraphicsObjectOgre<T> *CGraphicsObjectOgre<T>::StateListener::getOwner() const
        {
            return m_owner;
        }

        template <class T>
        SmartPtr<Properties> CGraphicsObjectOgre<T>::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();

            auto name = String( "null" );
            if( CGraphicsObject<T>::m_owner )
            {
                auto handle = CGraphicsObject<T>::m_owner->getHandle();
                if( handle )
                {
                    name = handle->getName();
                }
            }

            properties->setProperty( "owner", name );

            auto visible = CGraphicsObject<T>::isVisible();
            properties->setProperty( "visible", visible );
            return properties;
        }

        template <class T>
        void CGraphicsObjectOgre<T>::setProperties( [[maybe_unused]] SmartPtr<Properties> properties )
        {
            auto name = String();
            properties->getPropertyValue( "owner", name );

            auto visible = CGraphicsObject<T>::isVisible();
            properties->getPropertyValue( "visible", visible );
        }

        template <class T>
        SmartPtr<IStateContext> CGraphicsObjectOgre<T>::getStateObject() const
        {
            return CGraphicsObject<T>::m_stateObject;
        }

        template <class T>
        void CGraphicsObjectOgre<T>::setStateObject( SmartPtr<IStateContext> subject )
        {
            CGraphicsObject<T>::m_stateObject = subject;
        }

        template <class T>
        SmartPtr<IGraphicsScene> CGraphicsObjectOgre<T>::getCreator() const
        {
            return CGraphicsObject<T>::m_creator.lock();
        }

        template <class T>
        void CGraphicsObjectOgre<T>::setCreator( SmartPtr<IGraphicsScene> creator )
        {
            CGraphicsObject<T>::m_creator = creator;
        }

        template <class T>
        void CGraphicsObjectOgre<T>::StateListener::setOwner( CGraphicsObjectOgre<T> *owner )
        {
            m_owner = owner;
        }

    }  // end namespace render
}  // end namespace fb
