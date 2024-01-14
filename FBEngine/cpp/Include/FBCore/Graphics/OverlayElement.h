#ifndef _COverlayElement_H
#define _COverlayElement_H

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/Graphics/IOverlay.h>
#include <FBCore/Interface/Graphics/IOverlayElement.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/State/States/OverlayElementState.h>

namespace fb
{
    namespace render
    {
        template <class T>
        class OverlayElement : public SharedGraphicsObject<T>
        {
        public:
            OverlayElement()
            {
            }

            ~OverlayElement() override
            {
            }

            void setName( const String &name ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        state->setName( name );
                    }
                }
            }

            String getName() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getName();
                    }
                }

                return String( "" );
            }

            void setMaterial( SmartPtr<IMaterial> material ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        state->setMaterial( material );
                    }
                }
            }

            SmartPtr<IMaterial> getMaterial() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getMaterial();
                    }
                }

                return nullptr;
            }

            void setCaption( const String &text ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->setCaption( text );
                    }
                }
            }

            String getCaption() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getCaption();
                    }
                }

                return String( "" );
            }

            void setVisible( bool visible ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->setVisible( visible );
                    }
                }
            }

            bool isVisible() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->isVisible();
                    }
                }

                return false;
            }

            Vector2F getPosition() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getPosition();
                    }
                }

                return Vector2F::zero();
            }

            void setPosition( const Vector2F &position ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        state->setPosition( position );
                    }
                }
            }

            Vector2F getSize() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getSize();
                    }
                }

                return Vector2F::zero();
            }

            void setSize( const Vector2F &size ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        state->setSize( size );
                    }
                }
            }

            u32 getZOrder() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getZOrder();
                    }
                }

                return 0;
            }

            void setZOrder( [[maybe_unused]] u32 zOrder ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        state->setZOrder( zOrder );
                    }
                }
            }

            void setColour( [[maybe_unused]] const ColourF &colour ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        state->setColour( colour );
                    }
                }
            }

            ColourF getColour() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getColour();
                    }
                }

                return ColourF::White;
            }

            void setMetricsMode( [[maybe_unused]] u8 metricsMode ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->setMetricsMode( metricsMode );
                    }
                }
            }

            u8 getMetricsMode() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getMetricsMode();
                    }
                }

                return 0;
            }

            void setHorizontalAlignment( u8 gha ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->setHorizontalAlignment( gha );
                    }
                }
            }

            u8 getHorizontalAlignment() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getHorizontalAlignment();
                    }
                }

                return 0;
            }

            void setVerticalAlignment( u8 gva ) override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->setVerticalAlignment( gva );
                    }
                }
            }

            u8 getVerticalAlignment() const override
            {
                if( auto stateContext = getStateContext() )
                {
                    if( auto state = stateContext->template getStateByType<OverlayElementState>() )
                    {
                        return state->getVerticalAlignment();
                    }
                }

                return 0;
            }

            bool isContainer() const override
            {
                return false;
            }

            void _getObject( void **ppObject ) const override
            {
                *ppObject = nullptr;
            }

            SmartPtr<IOverlay> getOverlay() const override
            {
                return m_overlay;
            }

            void setOverlay( SmartPtr<IOverlay> overlay ) override
            {
                m_overlay = overlay;
            }

            SmartPtr<IOverlayElement> getParent() const override
            {
                return m_parent;
            }

            void setParent( SmartPtr<IOverlayElement> parent ) override
            {
                m_parent = parent;
            }

            void addChild( SmartPtr<IOverlayElement> element ) override
            {
                if( element )
                {
                    if( auto p = getChildrenPtr() )
                    {
                        auto &children = *p;
                        children.push_back( element );
                    }
                }
            }

            void removeChild( SmartPtr<IOverlayElement> element ) override
            {
                if( element )
                {
                    if( auto p = getChildrenPtr() )
                    {
                        auto children = *p;
                        auto it = std::find( children.begin(), children.end(), element );
                        if( it != children.end() )
                        {
                            children.erase( it );
                        }

                        auto newChildren = fb::make_shared<Array<SmartPtr<IOverlayElement>>>();
                        *newChildren =
                            Array<SmartPtr<IOverlayElement>>( children.begin(), children.end() );
                        setChildrenPtr( newChildren );
                    }
                }
            }

            Array<SmartPtr<IOverlayElement>> getChildren() const override
            {
                if( auto p = getChildrenPtr() )
                {
                    auto &children = *p;
                    return Array<SmartPtr<IOverlayElement>>( children.begin(), children.end() );
                }

                return Array<SmartPtr<IOverlayElement>>();
            }

            /** @copydoc IObject::isValid */
            bool isValid() const override
            {
                if( auto parent = getParent() )
                {
                    auto children = getChildren();
                    for( auto child : children )
                    {
                        if( !child->isValid() )
                        {
                            return false;
                        }
                    }

                    return true;
                }

                return false;
            }

            SmartPtr<Properties> getProperties() const override
            {
                auto properties = fb::make_ptr<Properties>();
                properties->setProperty( "materialName", m_materialName );

                const auto colour = getColour();
                properties->setProperty( "colour", colour );

                auto position = getPosition();
                properties->setProperty( "position", position );

                auto size = getSize();
                properties->setProperty( "size", size );

                const auto zorder = getZOrder();
                properties->setProperty( "zorder", zorder );

                const auto visible = isVisible();
                properties->setProperty( "visible", visible );

                return properties;
            }

            void setProperties( SmartPtr<Properties> properties ) override
            {
                properties->getPropertyValue( "materialName", m_materialName );
            }

            Array<SmartPtr<ISharedObject>> getChildObjects() const override
            {
                Array<SmartPtr<ISharedObject>> objects;
                objects.reserve( 2 );

                objects.push_back( m_overlay );
                objects.push_back( m_parent );

                return objects;
            }

            SmartPtr<IStateContext> getStateContext() const
            {
                return m_stateContext;
            }

            void setStateContext( SmartPtr<IStateContext> stateContext )
            {
                m_stateContext = stateContext;
            }

            SmartPtr<IStateListener> getStateListener() const
            {
                return m_stateListener;
            }

            void setStateListener( SmartPtr<IStateListener> stateListener )
            {
                m_stateListener = stateListener;
            }

            FB_CLASS_REGISTER_TEMPLATE_DECL( OverlayElement, T );

        protected:
            class ElementStateListener : public IStateListener
            {
            public:
                ElementStateListener() = default;
                ~ElementStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override
                {
                }

                void handleStateChanged( SmartPtr<IState> &state ) override
                {
                }

                void handleQuery( SmartPtr<IStateQuery> &query ) override
                {
                }

                SmartPtr<IOverlayElement> getOwner() const
                {
                    return m_owner;
                }

                void setOwner( SmartPtr<IOverlayElement> owner )
                {
                    m_owner = owner;
                }

            protected:
                AtomicSmartPtr<IOverlayElement> m_owner;
            };

            virtual void createStateContext()
            {
            }

            SharedPtr<Array<SmartPtr<IOverlayElement>>> getChildrenPtr() const
            {
                return m_children;
            }

            void setChildrenPtr( SharedPtr<Array<SmartPtr<IOverlayElement>>> p )
            {
                m_children = p;
            }

            String m_materialName;
            SmartPtr<IOverlay> m_overlay;
            SmartPtr<IOverlayElement> m_parent;
            SharedPtr<Array<SmartPtr<IOverlayElement>>> m_children;

            SmartPtr<IStateContext> m_stateContext;
            SmartPtr<IStateListener> m_stateListener;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, OverlayElement, T, T );
    }  // end namespace render
}  // end namespace fb

#endif
