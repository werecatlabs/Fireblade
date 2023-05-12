#ifndef _CGUIBar_H
#define _CGUIBar_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIBar.h>
#include <FBRenderUI/CUIElement.h>
#include "CUIImage.h"

namespace fb
{
    namespace ui
    {
        
        class CUIBar : public CUIElement<IUIBar>
        {
        public:
            CUIBar();
            ~CUIBar() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            void onToggleVisibility() override;

            void setMaterialName( const String &materialName );

            void setPosition( const Vector2F &position ) override;

            //! the amount of points the bar has
            //! this represented as a whole number
            void setPoints( f32 points ) override;

            //! set the maximum amount of points to the bar can contain
            //! e.g. a player might have 250 health points
            void setMaxPoints( f32 maxPoints ) override;

            //! Update the energy the bar to display changes in energy
            void update() override;

            u8 getBarOrientation() const;

            void setBarOrientation( u8 val );

        private:
            void onAddChild( IUIElement *child ) override;

            SmartPtr<render::IOverlayElementContainer> m_container;

            Vector2F m_originalSize;

            f32 m_targetPoints;
            f32 m_prevPoints;
            f32 m_curPoints;
            f32 m_maxPoints;

            u8 m_barOrientation;

            SmartPtr<CUIImage> m_barInner;
        };
    } // end namespace ui
}     // end namespace fb

#endif
