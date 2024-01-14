#ifndef _FB_GUIElement_H
#define _FB_GUIElement_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>
#include <FBCore/Interface/UI/IUIDragSource.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/Parameter.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/State/Messages/StateMessageObject.h>
#include <FBCore/State/States/UIElementState.h>
#include <FBCore/System/Prototype.h>

namespace fb
{
    namespace ui
    {
        /** A base gui element.
         */
        template <class T>
        class CUIElement : public core::Prototype<T>
        {
        public:
            /** Default constructor. */
            CUIElement();

            /** Virtual destructor. */
            ~CUIElement() override;

            void unload( SmartPtr<ISharedObject> data ) override;

            /** Used to initialise the gui element with default values. */
            virtual void initialise( SmartPtr<IUIElement> &parent );

            /** Used to initialise the gui element. */
            virtual void initialise( SmartPtr<IUIElement> &parent, const Properties &properties );

            /** Called the update the gui item. */
            void update() override;

            /** Called when an input event has occurred. */
            virtual bool handleEvent( const SmartPtr<IInputEvent> &event );

            /** Sets the id of the gui item. */
            void setName( const String &name );

            /** Gets the id of the gui item. */
            String getName() const;

            virtual s32 getElementId() const;

            virtual void setElementId( s32 val );

            /** Gets the hash id of the gui element. */
            u32 getHashId() const;

            /** Sets the component id. */
            void setComponentID( const String &componentId );

            /** Gets the component id. */
            const String &getComponentID() const;

            /** Gets the type of the gui item. */
            const String &getType() const;

            /** Sets the position of the gui item. */
            virtual void setPosition( const Vector2F &position );

            /** Returns the position of the gui item. */
            Vector2F getPosition() const;

            /** Gets the absolute position of the gui item. */
            Vector2F getAbsolutePosition() const;

            /** Sets the size of the gui item. */
            virtual void setSize( const Vector2F &size );

            /** Gets the size of the gui item. */
            Vector2F getSize() const;

            /** Sets whether this gui item is enabled. */
            void setEnabled( bool enabled, bool cascade = true );

            /** Sets whether this gui item is enabled. */
            bool isEnabled() const;

            /** Sets whether this gui item is selected. */
            void setSelected( bool selected );

            /** Checks if this gui item is selected. */
            bool isSelected() const;

            void setHovered( bool hovered )
            {
                m_isHovered = hovered;
            }

            bool isHovered() const
            {
                return m_isHovered;
            }

            /** Sets whether the gui item is visible or not. */
            virtual void setVisible( bool isVisible, bool cascade = true );

            /** Returns a boolean value indicating whether or not the ui item is visible. */
            bool isVisible() const;

            /** Sets whether this gui item is in focus. */
            virtual void setFocus( bool hasFocus );

            /** Checks if the this gui item is in focus. */
            bool isInFocus() const;

            /** Sets this gui item as being highlighted. */
            void setHighlighted( bool isHighlighted, bool cascade = true );

            /** Check if this gui item is highlighted. */
            bool isHighlighted() const;

            SmartPtr<IUIElement> getParent() const;

            void setParent( SmartPtr<IUIElement> parent );

            /** Gets the children of the gui item. */
            SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> getChildren() const;

            void setChildren( SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p );

            /** Adds a child to this gui item. */
            void addChild( SmartPtr<IUIElement> child ) override;

            /** Removes a child of the gui item. */
            bool removeChild( SmartPtr<IUIElement> child ) override;

            /** Removes this element. */
            void remove();

            /** Removes a the children of this gui item. */
            void removeAllChildren();

            void destroyAllChildren();

            /** Check if the check item has a child. */
            virtual bool hasChildById( const String &id ) const;

            /** Find a child by id. */
            virtual SmartPtr<IUIElement> findChildById( const String &id ) const;

            /** Finds a component by id. */
            SmartPtr<IUIElement> findChildByComponentId( const String &componentId ) const;

            s32 getSiblingIndex() const
            {
                if( m_parent )
                {
                    auto pThis = CUIElement<T>::template getSharedFromThis<IUIElement>();
                    auto children = m_parent->getChildren();
                    if( children )
                    {
                        for( u32 i = 0; i < children->size(); ++i )
                        {
                            if( children->at( i ) == pThis )
                            {
                                return i;
                            }
                        }
                    }
                }

                return -1;
            }

            SmartPtr<IUIElement> getLayout() const;
            void setLayout( SmartPtr<IUIElement> layout );

            /** Sets the attached user data.*/
            void setUserData( void *userData ) override;

            /** Gets the user data attached. */
            void *getUserData() const override;

            /** Finds a gui item listener. */
            SmartPtr<IEventListener> findObjectListener( const String &id ) const;

            /** Adds an animator to the container. */
            void addAnimator( SmartPtr<IAnimator> &animator );

            /** Removes an animator from the container. */
            bool removeAnimator( SmartPtr<IAnimator> &animator );

            SmartPtr<IUIContainer> getContainer() const;
            void setContainer( SmartPtr<IUIContainer> container );

            /** @copydoc IComponent::getOwner */
            virtual SmartPtr<ISharedObject> getOwner() const;

            /** @copydoc IComponent::setOwner */
            virtual void setOwner( SmartPtr<ISharedObject> owner );

            void _getObject( void **ppObject ) const;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            bool isDragDropSource() const;

            void setDragDropSource( bool dragDropSource );

            SmartPtr<IUIDragSource> &getDragSource();
            const SmartPtr<IUIDragSource> &getDragSource() const;

            void setDragSource( SmartPtr<IUIDragSource> dragSource );

            SmartPtr<IUIDropTarget> &getDropTarget();
            const SmartPtr<IUIDropTarget> &getDropTarget() const;

            void setDropTarget( SmartPtr<IUIDropTarget> dropTarget );

            virtual u32 getOrder() const;

            virtual void setOrder( u32 order );

            /** Internal function. */
            virtual void _onInitialiseStart();

            /** Internal function. */
            virtual void _onInitialiseEnd();

            //
            // Events
            //

            virtual void onAddChild( IUIElement *child );
            virtual void onRemoveChild( IUIElement *child );
            virtual void onChangedState();
            virtual void onChildChangedState( IUIElement *child );
            virtual void onToggleEnabled();
            virtual void onToggleVisibility();
            virtual void onToggleHighlight();
            virtual void onActivate( SmartPtr<IUIElement> element );
            virtual void onDeactivate();
            virtual void onSelect();
            virtual void onDeselect();
            virtual void onGainFocus();
            virtual void onLostFocus();

            virtual void handleEvent( const SmartPtr<IEvent> &event );

            SmartPtr<render::IOverlayElement> getOverlayElement() const;

            void setOverlayElement( SmartPtr<render::IOverlayElement> overlayElement );

            bool getSameLine() const;

            void setSameLine( bool sameLine );

            ColourF getColour() const;

            void setColour( const ColourF &colour );

            SmartPtr<IStateContext> getStateContext() const;

            void setStateContext( SmartPtr<IStateContext> stateContext );

            SmartPtr<IStateListener> getStateListener() const;

            void setStateListener( SmartPtr<IStateListener> stateListener );

            void sortZOrder();

            void updateZOrder();

            virtual void handleStateChanged( SmartPtr<IState> &state );

            bool getHandleInputEvents() const;

            void setHandleInputEvents( bool handleInputEvents );

            bool isThreadSafe() const;

            void addMessage( SmartPtr<IStateMessage> message );

        protected:
            class ElementStateListener : public IStateListener
            {
            public:
                ElementStateListener() = default;
                ~ElementStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                void handleStateChanged( SmartPtr<IState> &state ) override;

                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<CUIElement<T>> getOwner() const;

                void setOwner( SmartPtr<CUIElement<T>> owner );

            protected:
                AtomicSmartPtr<CUIElement<T>> m_owner;
            };

            // general event
            virtual void onEvent( const String &eventType );

            void setType( const String &type );

            virtual void createStateContext();

            AtomicSmartPtr<IStateContext> m_stateContext;
            AtomicSmartPtr<IStateListener> m_stateListener;

            AtomicSmartPtr<ISharedObject> m_owner;

            SmartPtr<IUIDragSource> m_dragSource;
            SmartPtr<IUIDropTarget> m_dropTarget;

            SmartPtr<render::IOverlayElement> m_overlayElement;

            SmartPtr<IUIContainer> m_container;

            /// The parent gui element.
            SmartPtr<IUIElement> m_parent;

            /// The attached user data.
            void *m_userData;

            SmartPtr<IUIElement> m_layout;

            /// Used to store user properties.
            mutable SmartPtr<ISharedObject> m_userProperties;

            s32 m_itemId = -1;

            /// Used to know if the element is enabled.
            bool m_isEnabled = true;

            /// To know if the element has focus.
            bool m_hasFocus = false;

            /// To know if the element is highlighted.
            bool m_isHighlighted = false;

            /// To know if the element is selected.
            bool m_isSelected = false;

            bool m_isHovered = false;

            bool m_dragDropSource = false;

            bool m_sameLine = false;

            bool m_handleInputEvents = true;

            /// The id of the element.
            String m_name;

            /// The hashed id of the element.
            hash_type m_hashId;

            /// The id of the component.
            String m_componentId;

            /// The type of gui element.
            String m_type;

            /// The children of the gui element.
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> m_children;

            /// The number of the next name extension.
            static u32 m_nextGeneratedNameExt;
        };

        template <class T>
        u32 CUIElement<T>::m_nextGeneratedNameExt = 0;

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
        void CUIElement<T>::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto renderUI = applicationManager->getRenderUI();

                setContainer( nullptr );

                removeAllChildren();

                CUIElement<T>::setObjectListenersPtr( nullptr );

                m_parent = nullptr;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CUIElement<T>::update()
        {
            if( !isVisible() )
            {
                return;
            }

            if( !m_isEnabled )
            {
                return;
            }

            auto applicationManager = core::ApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            if( task == Thread::Task::Application )
            {
                if( auto p = CUIElement<T>::getObjectListenersPtr() )
                {
                    Parameters params;
                    params.resize( 4 );
                    params[0].setPtr( this );
                    params[1].setU32( static_cast<u32>( task ) );
                    params[2].setF64( t );
                    params[3].setF64( dt );

                    auto &listeners = *p;
                    for( auto &listener : listeners )
                    {
                        //listener->handleEvent( IEvent::Type::UI, IUIElementListener::UPDATE_HASH, params,
                        //                       this, nullptr );
                    }
                }

                if( auto p = getChildren() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        child->update();
                    }
                }
            }
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

        template <class T>
        void CUIElement<T>::addChild( SmartPtr<IUIElement> child )
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();

            auto task = Thread::getCurrentTask();

            const auto &loadingState = CUIElement<T>::getLoadingState();
            const auto &childLoadingState = child->getLoadingState();

            if( loadingState == LoadingState::Loaded && childLoadingState == LoadingState::Loaded &&
                task == renderTask )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto renderUI = applicationManager->getRenderUI();
                FB_ASSERT( renderUI );

                ISharedObject::ScopedLock lock( renderUI );

                if( child )
                {
                    child->remove();  // remove from old parent
                    child->setParent( this );

                    auto p = getChildren();
                    if( !p )
                    {
                        p = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>();
                        setChildren( p );
                    }

                    if( p )
                    {
                        auto &children = *p;
                        children.push_back( child );
                    }

                    if( !this->template isDerived<ui::IUILayout>() )
                    {
                        auto layout = getLayout();
                        child->setLayout( layout );
                    }

                    onAddChild( child.get() );

                    updateZOrder();
                }

                auto childElement = fb::static_pointer_cast<CUIElement>( child );

                if( auto stateContext = childElement->getStateContext() )
                {
                    stateContext->setDirty( true );
                }
            }
            else
            {
                if( childLoadingState != LoadingState::Loaded )
                {
                    graphicsSystem->loadObject( child );
                }

                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( IUIElement::STATE_MESSAGE_ADD_CHILD );
                message->setObject( child );

                if( auto stateContext = getStateContext() )
                {
                    stateContext->addMessage( renderTask, message );
                }
            }
        }

        template <class T>
        bool CUIElement<T>::removeChild( SmartPtr<IUIElement> child )
        {
            if( child )
            {
                if( auto p = getChildren() )
                {
                    auto &children = *p;

                    auto childrenArray = Array<SmartPtr<IUIElement>>( children.begin(), children.end() );

                    auto it = std::find( childrenArray.begin(), childrenArray.end(), child );
                    if( it != childrenArray.end() )
                    {
                        child->setParent( nullptr );

                        childrenArray.erase( it );

                        onRemoveChild( child.get() );

                        *p = ConcurrentArray<SmartPtr<IUIElement>>( childrenArray.begin(),
                                                                    childrenArray.end() );
                        return true;
                    }
                }
            }

            return false;
        }

        template <class T>
        void CUIElement<T>::remove()
        {
            if( m_parent )
            {
                m_parent->removeChild( this );
            }
        }

        template <class T>
        void CUIElement<T>::removeAllChildren()
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        child->setParent( nullptr );
                    }
                }

                children.clear();
            }
        }

        template <class T>
        void CUIElement<T>::destroyAllChildren()
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto ui = applicationManager->getUI();

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        child->setParent( nullptr );

                        ui->removeElement( child );
                    }
                }

                setChildren( nullptr );
            }
        }

        template <class T>
        bool CUIElement<T>::hasChildById( const String &id ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                        if( id == childElement->getName() )
                        {
                            return true;
                        }
                    }
                }

                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                        if( childElement->hasChildById( id ) )
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        template <class T>
        SmartPtr<IUIElement> CUIElement<T>::findChildById( const String &id ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                        if( id == childElement->getName() )
                        {
                            return child;
                        }
                    }
                }

                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                        if( childElement->hasChildById( id ) )
                        {
                            return child;
                        }
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        SmartPtr<IUIElement> CUIElement<T>::findChildByComponentId( const String &componentId ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                    if( componentId == childElement->getComponentID() )
                    {
                        return childElement;
                    }
                }

                SmartPtr<IUIElement> pCompGUIItem;
                for( auto child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                    pCompGUIItem = childElement->findChildByComponentId( componentId );
                    if( pCompGUIItem )
                    {
                        return pCompGUIItem;
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        void CUIElement<T>::setUserData( void *pUserData )
        {
            m_userData = pUserData;
        }

        template <class T>
        void *CUIElement<T>::getUserData() const
        {
            return m_userData;
        }

        template <class T>
        SmartPtr<IEventListener> CUIElement<T>::findObjectListener( const String &id ) const
        {
            if( auto p = core::Prototype<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto listener : listeners )
                {
                    auto handle = listener->getHandle();
                    if( handle->getName() == id )
                    {
                        return listener;
                    }
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    auto listener = child->findObjectListener( id );
                    if( listener )
                    {
                        return listener;
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        void CUIElement<T>::addAnimator( SmartPtr<IAnimator> &animator )
        {
        }

        template <class T>
        bool CUIElement<T>::removeAnimator( SmartPtr<IAnimator> &animator )
        {
            return false;
        }

        template <class T>
        SmartPtr<IUIContainer> CUIElement<T>::getContainer() const
        {
            return m_container;
        }

        template <class T>
        void CUIElement<T>::setContainer( SmartPtr<IUIContainer> container )
        {
            m_container = container;
        }

        template <class T>
        void CUIElement<T>::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

        //
        // Callbacks
        //

        template <class T>
        void CUIElement<T>::_onInitialiseStart()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( this );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::INITIALISE_START_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::_onInitialiseEnd()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( this );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::INITIALISE_END_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onAddChild( IUIElement *child )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( child );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::ADD_CHILD_HASH, params, this, this,
                                           nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onRemoveChild( IUIElement *child )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( child );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::REMOVE_CHILD_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onChangedState()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::CHANGED_STATE_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onChildChangedState( IUIElement *child )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( child );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::CHILD_CHANGED_STATE_HASH, params,
                                           this, this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onToggleEnabled()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( this );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::TOGGLE_ENABLED_HASH, params, this,
                                           this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                parent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onToggleVisibility()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setBool( isVisible() );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::VISIBLE_HASH, params, this, this,
                                           nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                auto parentElement = fb::static_pointer_cast<CUIElement<T>>( parent );
                parentElement->onChildChangedState( this );
            }

            if( isVisible() )
            {
                if( auto p = CUIElement<T>::getObjectListenersPtr() )
                {
                    auto &listeners = *p;
                    if( !listeners.empty() )
                    {
                        Parameters params;
                        params.resize( 1 );
                        params[0].setBool( isVisible() );

                        for( auto &listener : listeners )
                        {
                            listener->handleEvent( IEvent::Type::UI, IEvent::SHOW_HASH, params, this,
                                                   this, nullptr );
                        }
                    }
                }
            }
            else
            {
                if( auto p = CUIElement<T>::getObjectListenersPtr() )
                {
                    auto &listeners = *p;
                    if( !listeners.empty() )
                    {
                        Parameters params;
                        params.resize( 1 );
                        params[0].setBool( isVisible() );

                        for( auto &listener : listeners )
                        {
                            listener->handleEvent( IEvent::Type::UI, IEvent::HIDE_HASH, params, this,
                                                   this, nullptr );
                        }
                    }
                }
            }
        }

        template <class T>
        void CUIElement<T>::onToggleHighlight()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    Parameters params;

                    listener->handleEvent( IEvent::Type::UI, IEvent::TOGGLE_HIGHLIGHT_HASH, params, this,
                                           this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                auto parentElement = fb::static_pointer_cast<CUIElement<T>>( parent );
                parentElement->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onActivate( SmartPtr<IUIElement> element )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                if( !listeners.empty() )
                {
                    for( auto &listener : listeners )
                    {
                        listener->handleEvent( IEvent::Type::UI, IEvent::ACTIVATE_HASH,
                                               Array<Parameter>(), this, element, nullptr );
                    }
                }
            }

            if( auto p = getChildren() )
            {
                auto children = *p;
                for( auto &child : children )
                {
                    if( child )
                    {
                        child->onActivate( element );
                    }
                }
            }

            if( auto parent = getParent() )
            {
                parent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onDeactivate()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::DEACTIVATE_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->onDeactivate();
                }
            }

            if( auto parent = getParent() )
            {
                parent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onSelect()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::SELECT_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->onSelect();
                }
            }

            if( auto parent = getParent() )
            {
                parent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onDeselect()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::DESELECT_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    child->onDeselect();
                }
            }

            if( auto parent = getParent() )
            {
                parent->onChildChangedState( this );
            }
        }

        template <class T>
        bool CUIElement<T>::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            auto enabled = isEnabled();
            if( !enabled )
            {
                return false;
            }

            //for( u32 i = 0; i < m_inputListeners.size(); ++i )
            //{
            //    auto inputListener = m_inputListeners[i];
            //    if( inputListener->onEvent( event ) )
            //        return true;
            //}

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    if( child->handleEvent( event ) )
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        template <class T>
        Vector2F CUIElement<T>::getAbsolutePosition() const
        {
            if( m_parent )
            {
                return m_parent->getAbsolutePosition() + getPosition();
            }

            return getPosition();
        }

        template <class T>
        void CUIElement<T>::setEnabled( bool enabled, bool cascade )
        {
            if( m_isEnabled != enabled )
            {
                m_isEnabled = enabled;

                if( cascade )
                {
                    if( auto p = getChildren() )
                    {
                        auto &children = *p;
                        for( u32 i = 0; i < children.size(); ++i )
                        {
                            children[i]->setEnabled( m_isEnabled );
                        }
                    }
                }

                if( !m_isEnabled && m_hasFocus )
                {
                    setFocus( false );
                }

                onToggleEnabled();
            }
        }

        template <class T>
        void CUIElement<T>::setSelected( bool selected )
        {
            bool wasSelected = m_isSelected;
            m_isSelected = selected;

            if( wasSelected && !m_isSelected )
            {
                onDeselect();
            }
            else if( !wasSelected && m_isSelected )
            {
                onSelect();
            }
        }

        template <class T>
        bool CUIElement<T>::isSelected() const
        {
            return m_isSelected;
        }

        template <class T>
        void CUIElement<T>::setVisible( bool visible, bool cascade )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setVisible( visible );
                }
            }

            if( cascade )
            {
                if( auto p = getChildren() )
                {
                    for( auto &child : *p )
                    {
                        child->setVisible( visible, cascade );
                    }
                }
            }

            //if( !visible && element->hasFocus() )
            //{
            //    setFocus( false );
            //}

            onToggleVisibility();
        }

        template <class T>
        void CUIElement<T>::setFocus( bool hasFocus )
        {
            if( m_hasFocus && !hasFocus )
            {
                onLostFocus();
            }

            m_hasFocus = hasFocus;

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );

                    childElement->setFocus( m_hasFocus );
                }

                if( m_hasFocus )
                {
                    onGainFocus();
                }
            }
        }

        template <class T>
        bool CUIElement<T>::isThreadSafe() const
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto renderTask = graphicsSystem->getRenderTask();

            auto task = Thread::getCurrentTask();

            const auto &loadingState = T::getLoadingState();

            return loadingState == LoadingState::Loaded && task == renderTask;
        }

        template <class T>
        void CUIElement<T>::addMessage( SmartPtr<IStateMessage> message )
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            if( auto stateContext = CUIElement<T>::getStateContext() )
            {
                const auto stateTask = graphicsSystem->getStateTask();
                stateContext->addMessage( stateTask, message );
            }
        }

        template <class T>
        SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> CUIElement<T>::getChildren() const
        {
            return m_children;
        }

        template <class T>
        void CUIElement<T>::setChildren( SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p )
        {
            m_children = p;
        }

        template <class T>
        void CUIElement<T>::onGainFocus()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::GAIN_FOCUS_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                parent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onLostFocus()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::LOST_FOCUS_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                parent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onEvent( const String &eventType )
        {
            // for (u32 i = 0; i < m_children.size(); ++i)
            //	((CUIElement*)m_children[i])->onEvent(eventType);

            // for (u32 i = 0; i < m_listeners.size(); ++i)
            //	m_listeners[i]->onEvent(eventType);
        }

        template <class T>
        void CUIElement<T>::setPosition( const Vector2F &position )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setPosition( position );
                }
            }
        }

        template <class T>
        SmartPtr<Properties> CUIElement<T>::getProperties() const
        {
            auto properties = fb::make_ptr<Properties>();
            properties->setProperty( "size", getSize() );
            properties->setProperty( "position", getPosition() );
            properties->setProperty( "enabled", m_isEnabled );
            properties->setProperty( "visible", isVisible() );
            return properties;
        }

        template <class T>
        void CUIElement<T>::setProperties( SmartPtr<Properties> properties )
        {
            auto enabled = isEnabled();
            auto visible = isVisible();
            //properties->getPropertyValue( "size", m_size );
            //properties->getPropertyValue( "position", m_position );
            properties->getPropertyValue( "enabled", enabled );
            properties->getPropertyValue( "visible", visible );

            setEnabled( enabled );
            setVisible( visible );
        }

        template <class T>
        Array<SmartPtr<ISharedObject>> CUIElement<T>::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( 32 );

            //if( auto stateContext = m_stateContext.load() )
            //{
            //    objects.push_back( stateContext );
            //}

            //if( auto stateListener = m_stateListener.load() )
            //{
            //    objects.push_back( stateListener );
            //}

            //if( auto owner = m_owner.load() )
            //{
            //    objects.push_back( owner );
            //}

            if( m_dragSource )
            {
                objects.push_back( m_dragSource );
            }

            if( m_dropTarget )
            {
                objects.push_back( m_dropTarget );
            }

            //if( auto element = getOverlayElement() )
            //{
            //    objects.push_back( element );
            //}

            //if( m_container )
            //{
            //    objects.push_back( m_container );
            //}

            //if( m_parent )
            //{
            //    objects.push_back( m_parent );
            //}

            //if( m_layout )
            //{
            //    objects.push_back( m_layout );
            //}

            return objects;
        }

        template <class T>
        bool CUIElement<T>::isDragDropSource() const
        {
            return m_dragDropSource;
        }

        template <class T>
        void CUIElement<T>::setDragDropSource( bool dragDropSource )
        {
            m_dragDropSource = dragDropSource;
        }

        template <class T>
        SmartPtr<IUIDragSource> &CUIElement<T>::getDragSource()
        {
            return m_dragSource;
        }

        template <class T>
        const SmartPtr<IUIDragSource> &CUIElement<T>::getDragSource() const
        {
            return m_dragSource;
        }

        template <class T>
        void CUIElement<T>::setDragSource( SmartPtr<IUIDragSource> dragSource )
        {
            m_dragSource = dragSource;
        }

        template <class T>
        SmartPtr<IUIDropTarget> &CUIElement<T>::getDropTarget()
        {
            return m_dropTarget;
        }

        template <class T>
        const SmartPtr<IUIDropTarget> &CUIElement<T>::getDropTarget() const
        {
            return m_dropTarget;
        }

        template <class T>
        void CUIElement<T>::setDropTarget( SmartPtr<IUIDropTarget> dropTarget )
        {
            m_dropTarget = dropTarget;
        }

        template <class T>
        u32 CUIElement<T>::getOrder() const
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getZOrder();
                }
            }

            if( auto element = getOverlayElement() )
            {
                return element->getZOrder();
            }

            return 0;
        }

        template <class T>
        void CUIElement<T>::setOrder( u32 order )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setZOrder( order );
                }
            }

            if( auto element = getOverlayElement() )
            {
                element->setZOrder( order );
            }
        }

        template <class T>
        void CUIElement<T>::initialise( SmartPtr<IUIElement> &parent )
        {
        }

        template <class T>
        void CUIElement<T>::initialise( SmartPtr<IUIElement> &parent, const Properties &properties )
        {
        }

        template <class T>
        void CUIElement<T>::setName( const String &name )
        {
            m_name = name;
            m_hashId = StringUtil::getHash( m_name );
        }

        template <class T>
        String CUIElement<T>::getName() const
        {
            return m_name;
        }

        template <class T>
        s32 CUIElement<T>::getElementId() const
        {
            return m_itemId;
        }

        template <class T>
        void CUIElement<T>::setElementId( s32 val )
        {
            m_itemId = val;
        }

        template <class T>
        void CUIElement<T>::setComponentID( const String &componentId )
        {
            m_componentId = componentId;
        }

        template <class T>
        const String &CUIElement<T>::getComponentID() const
        {
            return m_componentId;
        }

        template <class T>
        void CUIElement<T>::setType( const String &type )
        {
            m_type = type;
        }

        template <class T>
        const String &CUIElement<T>::getType() const
        {
            return m_type;
        }

        template <class T>
        Vector2F CUIElement<T>::getPosition() const
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getPosition();
                }
            }

            return Vector2F::zero();
        }

        template <class T>
        void CUIElement<T>::setSize( const Vector2F &size )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setSize( size );
                }
            }
        }

        template <class T>
        Vector2F CUIElement<T>::getSize() const
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getSize();
                }
            }

            return Vector2F::zero();
        }

        template <class T>
        bool CUIElement<T>::isEnabled() const
        {
            return m_isEnabled;
        }

        template <class T>
        bool CUIElement<T>::isVisible() const
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->isVisible();
                }
            }

            return false;
        }

        template <class T>
        bool CUIElement<T>::isInFocus() const
        {
            return m_hasFocus;
        }

        template <class T>
        bool CUIElement<T>::isHighlighted() const
        {
            return m_isHighlighted;
        }

        template <class T>
        SmartPtr<IUIElement> CUIElement<T>::getParent() const
        {
            return m_parent;
        }

        template <class T>
        void CUIElement<T>::setParent( SmartPtr<IUIElement> parent )
        {
            m_parent = parent;
        }

        template <class T>
        u32 CUIElement<T>::getHashId() const
        {
            return (u32)m_hashId;
        }

        template <class T>
        SmartPtr<IUIElement> CUIElement<T>::getLayout() const
        {
            return m_layout;
        }

        template <class T>
        void CUIElement<T>::setLayout( SmartPtr<IUIElement> layout )
        {
            m_layout = layout;
        }

        template <class T>
        void CUIElement<T>::handleEvent( const SmartPtr<IEvent> &event )
        {
        }

        template <class T>
        SmartPtr<render::IOverlayElement> CUIElement<T>::getOverlayElement() const
        {
            return m_overlayElement;
        }

        template <class T>
        void CUIElement<T>::setOverlayElement( SmartPtr<render::IOverlayElement> overlayElement )
        {
            m_overlayElement = overlayElement;
        }

        template <class T>
        void CUIElement<T>::createStateContext()
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateContext = stateManager->addStateObject();

            auto listener = factoryManager->make_ptr<ElementStateListener>();
            listener->setOwner( this );
            stateContext->addStateListener( listener );

            auto state = factoryManager->make_ptr<UIElementState>();
            stateContext->addState( state );

            stateContext->setOwner( this );

            setStateContext( stateContext );
            setStateListener( listener );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );
        }

        template <class T>
        ColourF CUIElement<T>::getColour() const
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getColour();
                }
            }

            return ColourF::White;
        }

        template <class T>
        void CUIElement<T>::setColour( const ColourF &colour )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->setColour( colour );
                }
            }
        }

        template <class T>
        SmartPtr<IStateContext> CUIElement<T>::getStateContext() const
        {
            return m_stateContext;
        }

        template <class T>
        void CUIElement<T>::setStateContext( SmartPtr<IStateContext> stateContext )
        {
            m_stateContext = stateContext;
        }

        template <class T>
        SmartPtr<IStateListener> CUIElement<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <class T>
        void CUIElement<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        template <class T>
        void CUIElement<T>::sortZOrder()
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;

                std::sort( children.begin(), children.end(),
                           []( auto a, auto b ) { return a->getOrder() < b->getOrder(); } );

                for( auto child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement>( child );
                    childElement->sortZOrder();
                }
            }
        }

        template <class T>
        void CUIElement<T>::updateZOrder()
        {
            sortZOrder();

            if( auto p = getChildren() )
            {
                for( auto &child : *p )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement>( child );
                    childElement->updateZOrder();
                }
            }
        }

        template <class T>
        void CUIElement<T>::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        template <class T>
        bool CUIElement<T>::getHandleInputEvents() const
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getHandleInputEvents();
                }
            }

            return false;
        }

        template <class T>
        void CUIElement<T>::setHandleInputEvents( bool handleInputEvents )
        {
            if( auto stateContext = getStateContext() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->setHandleInputEvents( handleInputEvents );
                }
            }
        }

        template <class T>
        void CUIElement<T>::setHighlighted( bool isHighlighted, bool cascade )
        {
            if( m_isHighlighted != isHighlighted )
            {
                m_isHighlighted = isHighlighted;

                if( cascade )
                {
                    if( auto p = getChildren() )
                    {
                        auto &children = *p;
                        for( auto &child : children )
                        {
                            auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                            childElement->setHighlighted( m_isHighlighted );
                        }
                    }
                }

                onToggleHighlight();
            }
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = getOwner() )
            {
                if( message->isExactly<StateMessageObject>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                    auto messageType = objectMessage->getType();
                    auto object = objectMessage->getObject();

                    if( messageType == IUIElement::STATE_MESSAGE_ADD_CHILD )
                    {
                        owner->addChild( object );
                    }
                    else if( messageType == IUIElement::STATE_MESSAGE_REMOVE_CHILD )
                    {
                        owner->removeChild( object );
                    }
                }
            }
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto renderUI = applicationManager->getRenderUI();
            FB_ASSERT( renderUI );

            IUIManager::ScopedLock lock( renderUI );

            if( auto owner = getOwner() )
            {
                owner->handleStateChanged( state );
            }

            auto elementState = fb::static_pointer_cast<UIElementState>( state );

            auto position = elementState->getPosition();
            auto size = elementState->getSize();
            auto zOrder = elementState->getZOrder();
            auto inheritsPick = elementState->getHandleInputEvents();

            auto iPosition = Vector2I::zero();
            auto iSize = Vector2I::zero();

            auto visible = elementState->isVisible();

            if( auto uiWindow = applicationManager->getSceneRenderWindow() )
            {
                auto sceneWindowPosition = uiWindow->getPosition();
                auto sceneWindowSize = uiWindow->getSize();

                sceneWindowSize = Vector2F( 1920, 1080 );

                auto pos = position * sceneWindowSize;
                auto sz = size * sceneWindowSize;

                iPosition = Vector2I( (s32)pos.X(), (s32)pos.Y() );
                iSize = Vector2I( (s32)sz.X(), (s32)sz.Y() );
            }
            else
            {
                if( auto mainWindow = applicationManager->getWindow() )
                {
                    auto mainWindowSize = mainWindow->getSize();
                    auto mainWindowSizeF = Vector2F( (f32)mainWindowSize.x, (f32)mainWindowSize.y );

                    auto pos = position * mainWindowSizeF;
                    auto sz = size * mainWindowSizeF;

                    iPosition = Vector2I( (s32)pos.X(), (s32)pos.Y() );
                    iSize = Vector2I( (s32)sz.X(), (s32)sz.Y() );
                }
            }

            if( auto owner = getOwner() )
            {
                if( auto element = owner->getOverlayElement() )
                {
                    if( element->isLoaded() )
                    {
                        element->setZOrder( zOrder );
                        element->setPosition( position );
                        element->setSize( size );
                        element->setVisible( visible );
                        //element->setInheritsPick(inheritsPick);

                        state->setDirty( false );
                    }
                }
            }
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        template <class T>
        SmartPtr<CUIElement<T>> CUIElement<T>::ElementStateListener::getOwner() const
        {
            return m_owner;
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::setOwner( SmartPtr<CUIElement<T>> owner )
        {
            m_owner = owner;
        }

    }  // end namespace ui
}  // end namespace fb

#endif
