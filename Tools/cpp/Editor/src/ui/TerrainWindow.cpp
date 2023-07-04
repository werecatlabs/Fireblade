#include <GameEditorPCH.h>
#include "TerrainWindow.h"
#include "ApplicationFrame.h"
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <ui/ResourceDatabaseDialog.h>

namespace fb
{
    namespace editor
    {
        TerrainWindow::TerrainWindow()
        {
            auto className = String( "TerrainEditor" );
            setClassName( className );

            // auto parentWindow = new wxScrolledWindow(parent, id);
            // setWindow(parentWindow);

            // wxBoxSizer* m_pButtonBox = new wxBoxSizer( wxVERTICAL );
            // parentWindow->SetSizer(m_pButtonBox);

            // m_createTerrainBtn = new wxButton(parentWindow, TerrainWindow::ID_TerrainCreateTerrainBtn,
            // wxString( "Create Terrain" ) ); m_pButtonBox->Add(m_createTerrainBtn);

            // m_raiseBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainRaiseBtn, wxString(
            // "Raise" ) ); m_pButtonBox->Add(m_raiseBtn);

            // m_lowerBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainLowerBtn, wxString(
            // "Lower" ) ); m_pButtonBox->Add(m_lowerBtn);

            // m_minimumBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainMinimumBtn,
            // wxString( "Minimum" ) ); m_pButtonBox->Add(m_minimumBtn);

            // m_maximumBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainMaximumBtn,
            // wxString( "Maximum" ) ); m_pButtonBox->Add(m_maximumBtn);

            // m_setHeightBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainsetHeightBtn,
            // wxString( "set Height" ) ); m_pButtonBox->Add(m_setHeightBtn);

            // m_paintBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainPaintBtn, wxString(
            // "Paint" ) ); m_pButtonBox->Add(m_paintBtn);

            // m_eraseBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainEraseBtn, wxString(
            // "Erase" ) ); m_pButtonBox->Add(m_eraseBtn);

            // m_blendBtn = new wxToggleButton(parentWindow, TerrainWindow::ID_TerrainBlendBtn, wxString(
            // "Blend" ) ); m_pButtonBox->Add(m_blendBtn);

            // m_addTextureBtn = new wxButton(parentWindow, TerrainWindow::ID_AddTerrainSplatImage,
            // wxString( "Add Texture" ) ); m_pButtonBox->Add(m_addTextureBtn);

            // m_brushesCombobox = new wxBitmapComboBox();
            // m_brushesCombobox->Create(parentWindow, TerrainWindow::ID_TerrainBrushImage,
            // wxEmptyString, 	wxDefaultPosition, wxDefaultSize, 	0, NULL, 	wxCB_SORT);

            // m_splatMapsCombobox = new wxBitmapComboBox();
            // m_splatMapsCombobox->Create(parentWindow, TerrainWindow::ID_TerrainSplatImage,
            // wxEmptyString, 	wxDefaultPosition, wxDefaultSize, 	0, NULL, 	wxCB_SORT);

            // String brushesFolderName = "../media/ET/Brushes/";
            // String splatMapsFolderName = "../media/ET/SplatMaps/";

            // SmartPtr<IFileSystem>& fileSystem = IApplicationManager::instance()->getFileSystem();
            // fileSystem->getFileNamesInFolder(brushesFolderName, m_brushes);
            // fileSystem->getFileNamesInFolder(splatMapsFolderName, m_splatMaps);

            // loadImages(brushesFolderName, m_brushes, m_brushesCombobox);
            // loadImages(splatMapsFolderName, m_splatMaps, m_splatMapsCombobox);

            ////m_combobox->setPopupMaxHeight(64);
            ////m_combobox->setSize(wxDefaultCoord, 64);

            // m_pButtonBox->Add(m_brushesCombobox);
            // m_pButtonBox->Add(m_splatMapsCombobox);

            // wxPropertyGridManager* pgman = new wxPropertyGridManager(parentWindow,
            //	TerrainWindow::ID_TerrainFrameToolProperties,
            //	wxDefaultPosition,
            //	wxDefaultSize,
            //	style );
            // m_toolPropertyGrid = pgman->GetGrid();
            // m_pButtonBox->Add(pgman , 1, wxEXPAND);

            // TerrainManager::getSingletonPtr()->addTerrainManagerListener(m_terrainManagerListener);
        }

        TerrainWindow::~TerrainWindow()
        {
            // bool result =
            // TerrainManager::getSingletonPtr()->removeTerrainManagerListener(m_terrainManagerListener);
        }

        void TerrainWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                BaseWindow::load( data );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto parent = getParent();

                auto parentWindow = ui->addElementByType<ui::IUIWindow>();
                FB_ASSERT( parentWindow );

                setParentWindow( parentWindow );
                parentWindow->setLabel( "TerrainWindowChild" );

                if( parent )
                {
                    parent->addChild( parentWindow );
                }

                auto terrainEditor = ui->addElementByType<ui::IUITerrainEditor>();
                parentWindow->addChild( terrainEditor );
                m_terrainEditor = terrainEditor;

                auto elementListener = fb::make_ptr<UIElementListener>();
                elementListener->setOwner( this );
                m_terrainEditorListener = elementListener;

                terrainEditor->addObjectListener( elementListener );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void TerrainWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<ui::IUITerrainEditor> TerrainWindow::getTerrainEditor() const
        {
            return m_terrainEditor;
        }

        void TerrainWindow::setTerrainEditor( SmartPtr<ui::IUITerrainEditor> terrainEditor )
        {
            m_terrainEditor = terrainEditor;
        }

        void TerrainWindow::updateSelection()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto selectionManager = applicationManager->getSelectionManager();

                auto selection = selectionManager->getSelection();
                for( auto object : selection )
                {
                    if( object )
                    {
                        if( object->isDerived<scene::TerrainSystem>() )
                        {
                            auto terrain = fb::dynamic_pointer_cast<scene::TerrainSystem>( object );

                            if( auto terrainEditor = getTerrainEditor() )
                            {
                                terrainEditor->setTerrain( terrain );
                            }
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void TerrainWindow::loadImages()
        {
            // for(u32 i=0; i<images.size(); ++i)
            //{
            //	String filePath = folderName + images[i];
            //	addComboBoxBitmap(filePath, comboBox);

            //}
        }

        void TerrainWindow::addComboBoxBitmap()
        {
            // String ext = StringUtil::getFileNameExtension(filePath);
            // wxBitmap* baseMap = NULL;

            // if(ext==(".jpg"))
            //{
            //	baseMap = new wxBitmap(filePath.c_str(), wxBITMAP_TYPE_JPEG);
            // }
            // else if(ext==(".png"))
            //{
            //	baseMap = new wxBitmap(filePath.c_str(), wxBITMAP_TYPE_PNG);
            // }
            // else if(ext == (".dds"))
            //{
            //	baseMap = createTexture(filePath);
            // }
            // else
            //{
            //	return;
            // }

            // baseMap->SetSize(64,64);

            // String fileName = Path::getFileName(filePath);
            // comboBox->Append(fileName.c_str(), *baseMap);
            // m_splatMaps.push_back(fileName);
        }

        // wxBitmap* TerrainWindow::createTexture(const String& texturePath)
        //{
        //	//auto applicationManager = core::IApplicationManager::instance();
        //	//SmartPtr<IFileSystem>& fileSystem = engine->getFileSystem();
        //	//SmartPtr<IStream> stream = fileSystem->open(texturePath);

        //	//ILuint handle, w, h;

        //	//ILenum	Error;

        //	//// Generate the main image name to use.
        //	//ilGenImages(1, &handle);

        //	//// Bind this image name.
        //	//ilBindImage(handle);

        //	////ILboolean loaded = ilLoadImage(filePath.c_str());
        //	//ILboolean loaded = ilLoadL(IL_DDS, stream->getData(), stream->getSize());
        //	//if (loaded == IL_FALSE)
        //	//	return 0; /* error encountered during loading */
        //	///* Let�s spy on it a little bit */
        //	//w = ilGetInteger(IL_IMAGE_WIDTH); // getting image width
        //	//h = ilGetInteger(IL_IMAGE_HEIGHT); // and height
        //	//printf("Our image resolution: %dx%d\n", w, h);
        //	///* how much memory will we need? */
        //	//int memory_needed = w * h * 3 * sizeof(unsigned char);
        //	///* We multiply by 3 here because we want 3 components per pixel */
        //	//ILubyte * data = (ILubyte *)malloc(memory_needed);
        //	///* finally get the image data */
        //	//ilCopyPixels(0, 0, 0, w, h, 1, IL_RGB, IL_UNSIGNED_BYTE, data);

        //	///* Finally, clean the mess! */
        //	//ilDeleteImages(1, & handle);
        //	////free(data); data = NULL;

        //	//wxImage image(w, h, data);
        //	//return new wxBitmap(image, 24);

        //	return nullptr;
        //}

        void TerrainWindow::OnRaiseBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("Raise");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("Raise");
            // }
        }

        void TerrainWindow::OnLowerBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("Lower");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("Lower");
            // }
        }

        void TerrainWindow::OnMinimumBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("Minimum");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("Minimum");
            // }
        }

        void TerrainWindow::OnMaximumBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("Maximum");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("Maximum");
            // }
        }

        void TerrainWindow::OnsetHeightBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("set Height");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("set Height");
            // }
        }

        void TerrainWindow::OnPaintBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("Paint");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("Paint");
            // }
        }

        void TerrainWindow::OnEraseBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("Erase");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("Erase");
            // }
        }

        void TerrainWindow::OnBlendBtnToggled()
        {
            // if( event.GetInt() )
            //{
            //	TerrainManager::getSingletonPtr()->selectTool("Blend");
            // }
            // else
            //{
            //	TerrainManager::getSingletonPtr()->deselectTool("Blend");
            // }
        }

        void TerrainWindow::OnAddTextureBtn()
        {
            // wxFileDialog dialog(this,
            //	_T("Open"),
            //	wxEmptyString,
            //	wxEmptyString,
            //	_T("DDS (*.dds)|*.dds|Png (*.png)|*.png|Jpg (*.jpg)|*.jpg"),
            //	wxFD_OPEN | wxFD_MULTIPLE
            //	);

            // dialog.SetFilterIndex(0);

            // if (dialog.ShowModal() == wxID_OK)
            //{
            //	wxArrayString paths;
            //	dialog.GetPaths(paths);

            //	for(u32 i=0; i<paths.size(); ++i)
            //	{
            //		String filePath = paths[i].c_str();
            //		addComboBoxBitmap(filePath, m_splatMapsCombobox);
            //	}
            //}
        }

        void TerrainWindow::OnComboBox()
        {
            // long sel = event.GetInt();
            // String imageName = m_splatMaps[sel];
            // TerrainManager::getSingletonPtr()->setTexture(imageName.c_str());
        }

        void TerrainWindow::OnPropertyGridChangeTool()
        {
            // wxPGProperty* p = event.GetProperty();

            // ITerrainTool* curTerrainTool = TerrainManager::getSingletonPtr()->getSelectedTool();
            // if(!curTerrainTool)
            //	return;

            // SmartPtr<Properties> propertyGroup = curTerrainTool->getPropertyGroup();
            // propertyGroup->setProperty(String(p->GetName().c_str()),
            // String(m_toolPropertyGrid->GetPropertyValueAsString(p).c_str()));
        }

        //
        // void TerrainWindow::OnSelectTool(TerrainToolPtr tool)
        //{
        //	populateToolProperties();
        // }

        void TerrainWindow::populateToolProperties()
        {
            // m_toolPropertyGrid->Clear();

            // ITerrainTool* curTerrainTool = TerrainManager::getSingletonPtr()->getSelectedTool();
            // if(!curTerrainTool)
            //	return;

            // const SmartPtr<Properties> properties = curTerrainTool->getPropertyGroup();
            // wxGUIUtil::populateProperties(*properties, m_toolPropertyGrid);
        }

        fb::Parameter TerrainWindow::UIElementListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto ui = editorManager->getUI();
            FB_ASSERT( ui );

            if( eventValue == ui::IUITerrainEditor::selectTerrainTextureHash )
            {
                auto resourceType = IResource::ResourceType::Texture;

                if( auto resourceDatabaseDialog = ui->getResourceDatabaseDialog() )
                {
                    resourceDatabaseDialog->setResourceType( resourceType );

                    //if( auto selected = getSelected() )
                    //{
                    if( arguments.size() >= 1 )
                    {
                        auto selected = arguments[0].object;
                        resourceDatabaseDialog->setCurrentObject( selected );
                    }
                    //}

                    resourceDatabaseDialog->setPropertyName( "baseTexture" );
                    resourceDatabaseDialog->setWindowVisible( true );
                    resourceDatabaseDialog->populate();
                }
            }

            return Parameter();
        }

        TerrainWindow *TerrainWindow::UIElementListener::getOwner() const
        {
            return m_owner;
        }

        void TerrainWindow::UIElementListener::setOwner( TerrainWindow *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
