#ifndef _CGUISPINNER_H
#define _CGUISPINNER_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUISpinner.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUISpinner : public CUIElement<IUISpinner>
        {
        public:
            CUISpinner();
            ~CUISpinner() override;

            void initialise( SmartPtr<IUIElement> &parent, const TiXmlNode *pNode );

            bool handleEvent( const SmartPtr<IInputEvent> &event ) override;

            void setText( const String &text ) override;
            String getText() const override;

            void incrementValue() override;
            void decrementValue() override;

            void setValue( const String &value ) override;
            String getValue() const override;

            void setMinValue( const String &value ) override;
            String getMinValue() const override;

            void setMaxValue( const String &value ) override;
            String getMaxValue() const override;

        private:
            // Callbacks
            void onToggleVisibility() override;
            void onToggleHighlight() override;

            String m_valueType;
            f64 m_increament;
            f64 m_value;
            f64 m_minValue;
            f64 m_maxValue;
            String m_text;
            SmartPtr<render::IOverlayElementText> m_overlayTxt;
            Array<String> m_values;
        };
    } // end namespace ui
}     // end namespace fb

#endif
