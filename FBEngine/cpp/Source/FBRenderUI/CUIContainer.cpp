#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIContainer.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb::ui
{

    CUIContainer::CUIContainer()
    {
        m_type = "Container";
    }

    CUIContainer::~CUIContainer()
    {
        unload( nullptr );
    }

    void CUIContainer::load( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Loading );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto overlayManager = graphicsSystem->getOverlayManager();
        FB_ASSERT( overlayManager );

        auto name = String( "Container_" ) + StringUtil::toString( m_nextGeneratedNameExt++ );
        setName( name );

        f32 left = 0.0;
        f32 top = 0.0;
        f32 width = 1.0;
        f32 height = 1.0;

        m_overlayContainer = overlayManager->addElement( String( "Panel" ), getName() );
        m_overlayContainer->setMetricsMode( render::IOverlayElement::GMM_RELATIVE );

        //m_overlayContainer->setLeft( left );
        //m_overlayContainer->setTop( top );
        //m_overlayContainer->setWidth( width );
        //m_overlayContainer->setHeight( height );
        setContainer( this );

        setPosition( Vector2F( left, top ) );
        setSize( Vector2F( width, height ) );

        setLoadingState( LoadingState::Loaded );
    }

    void CUIContainer::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                removeAllChildren();

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto overlayManager = graphicsSystem->getOverlayManager();
                FB_ASSERT( overlayManager );

                if( m_overlayContainer )
                {
                    overlayManager->removeElement( m_overlayContainer );
                    m_overlayContainer->unload( nullptr );
                    m_overlayContainer = nullptr;
                }

                CUIElement<IUIContainer>::unload( nullptr );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CUIContainer::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
    {
        SmartPtr<render::IOverlayManager> overlayManager =
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
        if( !overlayManager )
        {
            return;
        }

        const TiXmlElement *element = pNode->ToElement();
        FB_ASSERT_TRUE( !element );

        {
            const String id = XmlUtil::getString( element->Attribute( "id" ) );
            if( id.length() > 0 )
            {
                setName( id );
            }
        }

        String value;
        f32 left;
        f32 top;
        f32 width;
        f32 height;

        value = XmlUtil::getString( element->Attribute( "posX" ) );
        sscanf( value.c_str(), "%f", &left );
        value = XmlUtil::getString( element->Attribute( "posY" ) );
        sscanf( value.c_str(), "%f", &top );

        value = XmlUtil::getString( element->Attribute( "width" ) );
        sscanf( value.c_str(), "%f", &width );
        value = XmlUtil::getString( element->Attribute( "height" ) );
        sscanf( value.c_str(), "%f", &height );

        // m_overlayContainer = overlayManager->addElement(String("Panel"), getName());
        m_overlayContainer->setMetricsMode( render::IOverlayElement::GMM_RELATIVE );

        //m_overlayContainer->setLeft( left );
        //m_overlayContainer->setTop( top );
        //m_overlayContainer->setWidth( width );
        //m_overlayContainer->setHeight( height );

        setPosition( Vector2F( left, top ) );
        setSize( Vector2F( width, height ) );

        // auto overlay = overlayManager->findOverlay(parent->getName());
        // if(overlay)
        //{
        //	overlay->addElement(m_overlayContainer);
        // }

        auto zorder = m_overlayContainer->getZOrder();
        auto newZOrder = zorder + static_cast<u32>( parent->getChildren()->size() ) + 1;
        m_overlayContainer->setZOrder( newZOrder );
    }

    void CUIContainer::onToggleVisibility()
    {
        if( m_overlayContainer )
        {
            m_overlayContainer->setVisible( isVisible() );
        }

        CUIElement::onToggleVisibility();
    }

    void CUIContainer::setPosition( const Vector2F &position )
    {
        //if(m_overlayContainer)
        //{
        //    m_overlayContainer->setLeft( position.X() );
        //    m_overlayContainer->setTop( position.Y() );
        //}

        CUIElement::setPosition( position );
    }

    auto CUIContainer::getOverlayContainer() const -> SmartPtr<render::IOverlayElementContainer>
    {
        return m_overlayContainer;
    }

    void CUIContainer::setOverlayContainer( SmartPtr<render::IOverlayElementContainer> val )
    {
        m_overlayContainer = val;
    }

    void CUIContainer::onAddChild( IUIElement *child )
    {
        CUIElement<IUIContainer>::onAddChild( child );

        render::IOverlayElement *overlayChild = nullptr;
        child->_getObject( reinterpret_cast<void **>( &overlayChild ) );

        m_overlayContainer->addChild( overlayChild );
    }

    void CUIContainer::onRemoveChild( IUIElement *child )
    {
    }

    void CUIContainer::_getObject( void **ppObject ) const
    {
        *ppObject = m_overlayContainer.get();
    }
}  // namespace fb::ui
