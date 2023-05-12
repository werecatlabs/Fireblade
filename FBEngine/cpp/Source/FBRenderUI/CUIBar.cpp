#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIBar.h>
#include <FBRenderUI/CUIManager.h>
#include <FBRenderUI/CUIItemTemplate.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIBar::CUIBar()
        {
            m_type = "Bar";
        }

        CUIBar::~CUIBar()
        {
            // m_barInner.setNull();

            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_container );
        }

        void CUIBar::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
        {
            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();

            const TiXmlElement *element = pNode->ToElement();
            FB_ASSERT_TRUE( !element );
            {
                String id( element->Attribute( "id" ) );
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

            value = element->Attribute( "posX" );
            sscanf( value.c_str(), "%f", &left );
            value = element->Attribute( "posY" );
            sscanf( value.c_str(), "%f", &top );

            value = element->Attribute( "width" );
            sscanf( value.c_str(), "%f", &width );
            value = element->Attribute( "height" );
            sscanf( value.c_str(), "%f", &height );

            // m_container = overlayManager->addElement(String("Panel"), getName());
            m_container->setMetricsMode( render::IOverlayElement::GMM_RELATIVE );

            //m_container->setLeft( left );
            //m_container->setTop( top );
            //m_container->setWidth( width );
            //m_container->setHeight( height );

            setPosition( Vector2F( left, top ) );
            setSize( Vector2F( width, height ) );

            SmartPtr<render::IOverlay> overlay = overlayManager->findOverlay( parent->getName() );
            if( overlay )
            {
                overlay->addElement( m_container );
            }

            auto zorder = m_container->getZOrder();
            auto newZOrder = zorder + (u32)parent->getChildren()->size() + 1;

            m_container->setZOrder( newZOrder );

            m_barOrientation = static_cast<u8>( BarOrientation::BO_HORIZONTAL );
            if( String( element->Attribute( "barOrientation" ) ) == ( "vertical" ) )
            {
                m_barOrientation = static_cast<u8>( BarOrientation::BO_VERTICAL );
            }
        }

        void CUIBar::onToggleVisibility()
        {
            m_container->setVisible( isVisible() );

            CUIElement::onToggleVisibility();
        }

        void CUIBar::setMaterialName( const String &materialName )
        {
            m_barInner->setMaterialName( materialName );
        }

        void CUIBar::setPosition( const Vector2F &position )
        {
            CUIElement::setPosition( position );

            //m_container->setLeft( position.X() );
            //m_container->setTop( position.Y() );
        }

        void CUIBar::setPoints( f32 points )
        {
            m_targetPoints = points;
        }

        void CUIBar::setMaxPoints( f32 maxPoints )
        {
            m_maxPoints = maxPoints;
        }

        void CUIBar::update()
        {
            f32 ratio = m_targetPoints / m_maxPoints;

            if( m_barOrientation == static_cast<u8>( BarOrientation::BO_HORIZONTAL ) )
                m_barInner->setSize( Vector2F( m_originalSize.X() * ratio, m_originalSize.Y() ) );
            else
                m_barInner->setSize( Vector2F( m_originalSize.X(), m_originalSize.Y() * ratio ) );
        }

        u8 CUIBar::getBarOrientation() const
        {
            return m_barOrientation;
        }

        void CUIBar::setBarOrientation( u8 val )
        {
            m_barOrientation = val;
        }

        void CUIBar::onAddChild( IUIElement *child )
        {
            // for(u32 i=0; i<m_children.size(); ++i)
            //{
            //	IGUIElement* curChild = (IGUIElement*)m_children[i];
            //	if(curChild->getType()==(String("Image")))
            //	{
            //		m_barInner = curChild;
            //		m_originalSize = m_barInner->getSize();
            //		return;
            //	}
            // }
        }
    }  // namespace ui
}  // end namespace fb
