#include <GameEditorPCH.h>
#include "ui/AddDialog.h"
#include "editor/Project.h"
#include "editor/EditorManager.h"

namespace fb
{
    namespace editor
    {

        AddEntityDialog::AddEntityDialog()
        {
            // wxBoxSizer *baseSizer = new wxBoxSizer( wxVERTICAL );
            // SetSizer(baseSizer);

            // wxBoxSizer *compDataBox = new wxBoxSizer( wxVERTICAL );
            // baseSizer->Add(compDataBox, 1, wxEXPAND);

            // wxPropertyGridManager* pgman = new wxPropertyGridManager(this,
            //	PropertiesId,
            //	wxDefaultPosition,
            //	wxDefaultSize,
            //	wxPG_SPLITTER_AUTO_CENTER | wxPG_BOLD_MODIFIED );
            // m_properties = pgman->GetGrid();
            // baseSizer->Add( pgman, 1, wxEXPAND );
            //
            // wxBoxSizer *optionsBox = new wxBoxSizer( wxVERTICAL );
            // baseSizer->Add(optionsBox, 1, wxEXPAND);

            // m_useDefaultsChkBox = new wxCheckBox(this, USE_DEFAULTS_CHK, "Use Defaults");
            // m_useDefaultsChkBox->SetValue(false);
            // optionsBox->Add(m_useDefaultsChkBox, 0, wxALIGN_CENTER_HORIZONTAL);

            // wxBoxSizer *btnBox = new wxBoxSizer( wxHORIZONTAL );
            // baseSizer->Add(btnBox, 0, wxALIGN_CENTER_HORIZONTAL);

            // m_okBtn = new wxButton(this, wxID_OK, "Ok");
            // btnBox->Add(m_okBtn, 1, wxEXPAND);

            // m_cancelBtn = new wxButton(this, CANCEL_BTN_ID, "Cancel");
            // btnBox->Add(m_cancelBtn, 1, wxEXPAND);
        }

        AddEntityDialog::~AddEntityDialog()
        {
        }

        String AddEntityDialog::getLabel() const
        {
            //const Array<Property> &properties = m_propGrp.getPropertiesAsArray();
            //for( u32 i = 0; i < properties.size(); ++i )
            //{
            //    const Property &prop = properties[i];

            //    String label = prop.getLabel();
            //    if( label.length() == 0 )
            //        label = prop.getName();

            //    if( label == "Name" )
            //        return prop.getValue();
            //}

            return "";
        }

        void AddEntityDialog::OnCancelBtn()
        {
            // this->Close();
        }

        bool AddEntityDialog::getUseDefaults() const
        {
            // return m_useDefaultsChkBox->IsChecked();
            return false;
        }

        void AddEntityDialog::setProperties( SmartPtr<Properties> properties )
        {
            m_properties = properties;
        }

        SmartPtr<Properties> AddEntityDialog::getProperties() const
        {
            return m_properties;
        }

        void AddEntityDialog::OnPropertyChange()
        {
            // wxPGProperty* p = event.GetProperty();
            // String propName = String(p->GetName().c_str());

            // if(propName==("path"))
            //{
            //	SmartPtr<Project> project = EditorManager::getSingletonPtr()->getProject();
            //	String relPath = String(p->GetValueAsString().c_str());
            //	String value = Path::getRelativePath(project->getProjectDirectory(), relPath).c_str();
            //	m_propGrp.setProperty(propName, value);
            //	p->SetValueFromString(value.c_str());
            //	return;
            // }

            // ui::wxGUIUtil::setPropertyValue(m_propGrp, m_properties, p);
        }

    }  // namespace editor
}  // namespace fb
