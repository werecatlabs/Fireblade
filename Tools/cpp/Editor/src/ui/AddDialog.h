#ifndef AddEntityDialog_h__
#define AddEntityDialog_h__

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"

namespace fb
{
    namespace editor
    {
        
        class AddEntityDialog : public BaseWindow
        {
        public:
            enum
            {
                CANCEL_BTN_ID,
                USE_DEFAULTS_CHK,
                PropertiesId,
            };

            AddEntityDialog();
            ~AddEntityDialog();

            String getLabel() const;
            bool getUseDefaults() const;

            void setProperties( SmartPtr<Properties> properties );
            SmartPtr<Properties> getProperties() const;

        protected:
            void OnCancelBtn();
            void OnPropertyChange();

            // wxPropertyGrid*	m_properties;

            // wxCheckBox* m_useDefaultsChkBox;
            // wxButton* m_okBtn;
            // wxButton* m_cancelBtn;

            SmartPtr<Properties> m_properties;
        };

    }  // end namespace editor
}  // end namespace fb

#endif  // AddEntityDialog_h__
