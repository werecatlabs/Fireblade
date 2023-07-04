#include <GameEditorPCH.h>
#include <ui/MeshImportWindow.h>
#include "FBWxWidgets/wxGUIUtil.h"
#include "editor/EditorManager.h"
#include <FBCore/Interface/Scene/IActor.h>
#include <FBWxWidgets/wxLabelCheckboxPair.h>
#include <FBApplication/ApplicationUtil.h>
#include <FBGraphics/GraphicsUtil.h>
#include <ui/PropertiesWindow.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        void MeshImportWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto parent = getParent();

                auto parentWindow = ui->addElementByType<ui::IUIWindow>();
                setParentWindow( parentWindow );

                if( parent )
                {
                    parent->addChild( parentWindow );
                }

                m_dropdown = ui->addElementByType<ui::IUIDropdown>();
                FB_ASSERT( m_dropdown );

                auto dropdownListener = fb::make_ptr<ElementListener>();
                dropdownListener->setOwner( this );
                m_dropdown->addObjectListener( dropdownListener );

                auto materialTypes = render::GraphicsUtil::getMaterialTypes();
                m_dropdown->setOptions( materialTypes );

                parentWindow->addChild( m_dropdown );

                auto treeCtrl = ui->addElementByType<ui::IUITreeCtrl>();
                parentWindow->addChild( treeCtrl );
                m_tree = treeCtrl;

                auto treeListener = fb::make_ptr<ElementListener>();
                treeListener->setOwner( this );
                m_treeListener = treeListener;

                m_tree->addObjectListener( treeListener );

                m_propertiesWindow = fb::make_ptr<PropertiesWindow>();
                m_propertiesWindow->setParent( parentWindow );
                m_propertiesWindow->load( data );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void MeshImportWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                if( m_tree )
                {
                    ui->removeElement( m_tree );
                    m_tree = nullptr;
                }

                m_treeListener = nullptr;

                if( m_propertiesWindow )
                {
                    m_propertiesWindow->unload( nullptr );
                    m_propertiesWindow = nullptr;
                }

                if( auto parentWindow = getParentWindow() )
                {
                    ui->removeElement( parentWindow );
                    setParentWindow( nullptr );
                }

                BaseWindow::unload( nullptr );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        MeshImportWindow::MeshImportWindow()
        {
        }

        MeshImportWindow::~MeshImportWindow()
        {
            unload( nullptr );
        }

        void MeshImportWindow::setCreateRoadBtnToggled( bool status )
        {
            m_isCreateRoadBtnToggled = status;
        }

        bool MeshImportWindow::isCreateRoadBtnToggled()
        {
            return m_isCreateRoadBtnToggled;
        }

        void MeshImportWindow::setLastRoadNodeCreated( SmartPtr<scene::IActor> riverNode )
        {
            m_lastCreatedEntity = riverNode;
        }

        SmartPtr<scene::IActor> MeshImportWindow::getLastRoadNodeCreated() const
        {
            return m_lastCreatedEntity;
        }

        void MeshImportWindow::OnCreateRoad()
        {
            // ApplicationManager* applicationManager = IApplicationManager::instance();
            // RoadManagerPtr roadManager = applicationManager->getRoadManager();
            // if ( roadManager )
            //{
            //	roadManager->createRoad();
            // }
        }

        void MeshImportWindow::OnAddRoadNodeToggled()
        {
            auto applicationManager = EditorManager::getSingletonPtr();
            // RoadManagerPtr roadManager = applicationManager->getRoadManager();

            // if ( event.GetInt() )
            //{
            //	m_isCreateRoadBtnToggled = true;

            //	if ( roadManager )
            //	{
            //		roadManager->setCreateRoadNode(m_isCreateRoadBtnToggled);
            //	}
            //}
            // else
            //{
            //	m_isCreateRoadBtnToggled = false;

            //	if ( roadManager )
            //	{
            //		roadManager->setCreateRoadNode(m_isCreateRoadBtnToggled);
            //	}
            //}
        }

        void MeshImportWindow::OnPropertyGridChange()
        {
            // wxPGProperty* p = event.GetProperty();

            // SmartPtr<IActor> road = RoadManager::getSingletonPtr()->getSelectedRoad();
            // if ( !road.isNull() )
            //{
            //	String name = p->GetName().c_str();
            //	String value = m_roadPropertyGrid->GetPropertyValueAsString(p).c_str();
            //	road->setPropertyValue(name, value);
            // }
        }

        //
        // void MeshImportWindow::OnCreateRoad()
        //{
        //	populateLayerProperties();
        //	populateFoliageLayers();
        // }

        void MeshImportWindow::populateFoliageLayers()
        {
            // m_riversList->ClearAll();

            // wxListItem itemCol;
            // itemCol.setText(_T("Name"));
            // itemCol.setWidth(500);
            // m_riversList->InsertColumn(0, itemCol);

            // Array<SmartPtr<IActor>> layers = RoadManager::getSingletonPtr()->getRoads();
            // for ( u32 i = 0; i < layers.size(); ++i )
            //{
            //	SmartPtr<IActor> layer = layers[i];
            //	String name = layer->GetName();
            //	wxString buf(name.c_str());
            //	long tmp = m_riversList->InsertItem(i, buf, 0);
            // }
        }

        void MeshImportWindow::populateLayerProperties()
        {
            // m_roadPropertyGrid->Clear();

            // EditorSmartPtr<IActor> layer = RoadManager::getSingletonPtr()->getSelectedRoad();
            // if ( !layer.isNull() )
            //{
            //	Properties propertyGroup;
            //	layer->getProperties(propertyGroup);
            //	wxGUIUtil::populateProperties(propertyGroup, m_roadPropertyGrid);
            // }
        }

        void MeshImportWindow::OnDestroyRoad()
        {
            populateLayerProperties();
            populateFoliageLayers();
        }

        void MeshImportWindow::OnSelectRoad()
        {
            populateLayerProperties();
        }

        fb::Parameter MeshImportWindow::ElementListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            return Parameter();
        }

        MeshImportWindow *MeshImportWindow::ElementListener::getOwner() const
        {
            return m_owner;
        }

        void MeshImportWindow::ElementListener::setOwner( MeshImportWindow *owner )
        {
            m_owner = owner;
        }

    }  // end namespace editor
}  // end namespace fb
