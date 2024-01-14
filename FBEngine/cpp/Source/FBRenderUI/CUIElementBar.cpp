#include <FBRenderUI/FBRenderUIPCH.h>
#include "FBRenderUI/CUIElementBar.h"
#include "FBRenderUI/CUIManager.h"
#include "FBRenderUI/CUIItemTemplate.h"
#include <FBRenderUI/CUIImage.h>
#include <FBCore/FBCore.h>

namespace fb::ui
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
            core::ApplicationManager::instance()->getGraphicsSystem()->getOverlayManager();
        overlayManager->removeElement( m_container );
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

            for( auto &m_image : m_images )
            {
                m_image->setVisible( false );
            }

            for( u32 i = 0; i < numElements; ++i )
            {
                m_images[i]->setVisible( true );
            }
        }

        auto numElements =
            MathI::clamp( static_cast<s32>( static_cast<f32>( m_images.size() ) * ( 1.0f - ratio ) ), 0,
                          static_cast<s32>( m_images.size() ) );

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

    auto CUIElementBar::getBarOrientation() const -> u8
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
}  // namespace fb::ui
