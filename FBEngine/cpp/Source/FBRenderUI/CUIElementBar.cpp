#include <FBRenderUI/FBRenderUIPCH.h>
#include "FBRenderUI/CUIElementBar.h"
#include "FBRenderUI/CUIManager.h"
#include "FBRenderUI/CUIItemTemplate.h"
#include <FBRenderUI/CUIImage.h>
#include <FBCore/FBCore.h>
#include <tinyxml.h>

namespace fb
{
    namespace ui
    {

        CUIElementBar::CUIElementBar()
        {
            m_type = "Bar";
        }

        CUIElementBar::~CUIElementBar()
        {
            // m_barInner.setNull();

            removeAllChildren();

            SmartPtr<render::IOverlayManager> overlayManager =
                core::IApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
            overlayManager->removeElement( m_container );
        }

        void CUIElementBar::initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode )
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

            auto numChildren = (u32)parent->getChildren()->size();
            auto newZOrder = zorder + numChildren + 1;
            m_container->setZOrder( newZOrder );

            m_barOrientation = static_cast<u8>( BarOrientation::BO_HORIZONTAL );
            if( String( element->Attribute( "barOrientation" ) ) == ( "vertical" ) )
            {
                m_barOrientation = static_cast<u8>( BarOrientation::BO_VERTICAL );
            }
        }

        void CUIElementBar::onToggleVisibility()
        {
            m_container->setVisible( isVisible() );

            CUIElement::onToggleVisibility();
        }

        void CUIElementBar::setMaterialName( const String &materialName )
        {
            // m_barInner->setMaterialName(materialName);
        }

        void CUIElementBar::setPosition( const Vector2F &position )
        {
            CUIElement::setPosition( position );

            //m_container->setLeft( position.X() );
            //m_container->setTop( position.Y() );
        }

        void CUIElementBar::setPoints( f32 points )
        {
            m_targetPoints = points;
        }

        void CUIElementBar::setMaxPoints( f32 maxPoints )
        {
            m_maxPoints = maxPoints;
        }

        void CUIElementBar::update()
        {
            f32 ratio = m_targetPoints / m_maxPoints;

            if constexpr( false )
            {
                u32 numElements = static_cast<u32>( static_cast<f32>( m_images.size() ) * ratio );

                for( u32 i = 0; i < m_images.size(); ++i )
                {
                    m_images[i]->setVisible( false );
                }

                for( u32 i = 0; i < numElements; ++i )
                {
                    m_images[i]->setVisible( true );
                }
            }

            auto numElements =
                MathI::clamp( (s32)( static_cast<f32>( m_images.size() ) * ( 1.0f - ratio ) ), 0,
                              (s32)m_images.size() );

            for( size_t i = 0; i < m_images.size(); ++i )
            {
                if( i < numElements )
                {
                    m_images[i]->setVisible( false );
                }
                else
                {
                    m_images[i]->setVisible( true );
                }
            }
        }

        u8 CUIElementBar::getBarOrientation() const
        {
            return m_barOrientation;
        }

        void CUIElementBar::setBarOrientation( u8 val )
        {
            m_barOrientation = val;
        }

        void CUIElementBar::onAddChild( IUIElement *child )
        {
            // m_images.push_back(child);
        }
    }  // namespace ui
}  // end namespace fb
