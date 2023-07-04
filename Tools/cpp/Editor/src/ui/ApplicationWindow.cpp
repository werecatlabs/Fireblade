#include <GameEditorPCH.h>
#include "ui/ApplicationWindow.h"
#include "editor/EditorManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        ApplicationWindow::ApplicationWindow()
        {
            // wxBoxSizer *baseSizer = new wxBoxSizer( wxVERTICAL );
            // SetSizer(baseSizer);

            // m_tree = new wxTreeCtrl(this, -1);
            // baseSizer->Add(m_tree, 1, wxEXPAND);

            // wxBoxSizer *btnBox = new wxBoxSizer( wxHORIZONTAL );
            // baseSizer->Add(btnBox);

            // m_newBtn = new wxButton(this, NEW_COMP_BTN_ID, "New");
            // btnBox->Add(m_newBtn, 1, wxEXPAND);

            // m_deleteBtn = new wxButton(this, DELETE_COMP_BTN_ID, "Delete");
            // btnBox->Add(m_deleteBtn, 1, wxEXPAND);

            // populateComponentTree();
        }

        ApplicationWindow::~ApplicationWindow()
        {
        }

        void ApplicationWindow::populateComponentTree()
        {
            // m_tree->DeleteAllItems();

            // = m_tree->AddRoot("Components");
            //
            // EditorManager* appRoot = EditorManager::getSingletonPtr();
            // Array<SmartPtr<ComponentTemplate>> componentTempates =
            // appRoot->getComponentTemplateMgr()->getComponents(); for(u32 i=0;
            // i<componentTempates.size(); ++i)
            //{
            //	SmartPtr<ComponentTemplate> componentTemplate = componentTempates[i];
            //	m_tree->AppendItem(rootId, componentTemplate->getLabel().c_str());
            //}

            // m_tree->SortChildren(rootId);
        }

        void ApplicationWindow::OnNewComponent()
        {
            // EditComponentDialog dlg(this);
            // if(dlg.ShowModal() == wxID_OK)
            //{
            //	SmartPtr<ComponentTemplate> componentTemplate(new ComponentTemplate);
            //	componentTemplate->setLabel(dlg.getLabel());
            //	componentTemplate->setType(dlg.getType());

            //	auto editorManager = EditorManager::getSingletonPtr();
            //	auto componentTemplateMgr = editorManager->getComponentTemplateMgr();
            //	componentTemplateMgr->addComponent(componentTemplate);

            //	populateComponentTree();
            //}
        }

        void ApplicationWindow::OnDeleteComponent()
        {
        }

    }  // end namespace editor
}  // namespace fb
