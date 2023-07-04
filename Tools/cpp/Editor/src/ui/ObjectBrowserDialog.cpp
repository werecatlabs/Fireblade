#include <GameEditorPCH.h>
#include <ui/ObjectBrowserDialog.h>
#include <ui/ProjectTreeData.h>
#include <editor/EditorManager.h>
#include <FBApplication/ApplicationUtil.h>
#include <FBCore/FBCore.h>
#include "ActorWindow.h"
#include "UIManager.h"

namespace fb
{
    namespace editor
    {
        ObjectBrowserDialog::ObjectBrowserDialog()
        {
            // wxBoxSizer* baseSizer = new wxBoxSizer(wxVERTICAL);
            // SetSizer(baseSizer);

            // m_tree = new wxTreeCtrl(this, -1);
            // baseSizer->Add(m_tree, 1, wxEXPAND);

            // m_tree->Bind(wxEVT_TREE_SEL_CHANGED, &ObjectBrowserDialog::handleTreeSelectionChanged,
            // this, -1); m_tree->Bind(wxEVT_TREE_ITEM_ACTIVATED,
            // &ObjectBrowserDialog::handleTreeSelectionActivated, this, -1);

            // populate();
        }

        ObjectBrowserDialog::~ObjectBrowserDialog()
        {
            unload( nullptr );
        }

        void ObjectBrowserDialog::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto parent = getParent();

                auto parentWindow = ui->addElementByType<ui::IUIWindow>();
                FB_ASSERT( parentWindow );

                setParentWindow( parentWindow );
                parentWindow->setLabel( "ObjectBrowserDialogChild" );

                if( parent )
                {
                    parent->addChild( parentWindow );
                }

                auto uiListener = fb::make_ptr<UIElementListener>();
                uiListener->setOwner( this );
                m_uiListener = uiListener;

                auto addComponentButton = ui->addElementByType<ui::IUIButton>();
                FB_ASSERT( addComponentButton );

                addComponentButton->setLabel( "Add Component" );
                parentWindow->addChild( addComponentButton );
                addComponentButton->addObjectListener( uiListener );
                addComponentButton->setElementId( AddComponent );
                m_addComponentButton = addComponentButton;

                auto tree = ui->addElementByType<ui::IUITreeCtrl>();
                parentWindow->addChild( tree );
                tree->setMultiSelect( false );
                tree->addObjectListener( uiListener );
                tree->setElementId( Tree );
                m_tree = tree;

                populate();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ObjectBrowserDialog::unload( SmartPtr<ISharedObject> data )
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
                    m_tree->removeObjectListener( m_uiListener );

                    ui->removeElement( m_tree );
                    m_tree = nullptr;
                }

                if( m_addComponentButton )
                {
                    ui->removeElement( m_addComponentButton );
                    m_addComponentButton = nullptr;
                }

                if( auto parentWindow = getParentWindow() )
                {
                    ui->removeElement( parentWindow );
                    setParentWindow( nullptr );
                }

                if( m_uiListener )
                {
                    m_uiListener->unload( nullptr );
                    m_uiListener = nullptr;
                }

                for( auto data : m_dataArray )
                {
                    data->unload( nullptr );
                }

                m_dataArray.clear();

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ObjectBrowserDialog::populate()
        {
            if( !m_tree )
            {
                return;
            }

            if( m_tree )
            {
                m_tree->clear();
            }

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            auto factories = factoryManager->getFactories();

            std::sort( factories.begin(), factories.end(),
                       []( SmartPtr<IFactory> a, SmartPtr<IFactory> b ) {
                           return a->getObjectType() < b->getObjectType();
                       } );

            auto rootNode = m_tree->addRoot();
            FB_ASSERT( rootNode );
            rootNode->setExpanded( true );

            for( auto factory : factories )
            {
                if( factory->isObjectDerivedFrom<scene::IComponent>() )
                {
                    auto name = factory->getObjectType();

                    auto treeNode = m_tree->addNode();

                    FB_ASSERT( treeNode );
                    ApplicationUtil::setText( treeNode, name );

                    auto data = factoryManager->make_ptr<ProjectTreeData>( "factory", "factory", factory,
                                                                           factory );
                    treeNode->setNodeUserData( data );

                    rootNode->addChild( treeNode );
                }
            }

            // m_tree->ExpandAll();
        }

        String ObjectBrowserDialog::getSelectedObject() const
        {
            return m_selectedObject;
        }

        void ObjectBrowserDialog::setSelectedObject( const String &val )
        {
            m_selectedObject = val;
        }

        SmartPtr<ui::IUITreeCtrl> ObjectBrowserDialog::getTree() const
        {
            return m_tree;
        }

        void ObjectBrowserDialog::setTree( SmartPtr<ui::IUITreeCtrl> tree )
        {
            m_tree = tree;
        }

        void ObjectBrowserDialog::handleTreeSelectionChanged()
        {
            // auto selectedId = event.GetItem();
            // auto data = (ProjectTreeData*)m_tree->GetItemData(selectedId);
            // if (data)
            //{
            //	auto factory = fb::static_pointer_cast<IFactory>(data->getOwnerData());
            //	auto selectedObject = factory->getObjectType();
            //	setSelectedObject(selectedObject);
            // }
        }

        void ObjectBrowserDialog::handleTreeSelectionActivated()
        {
            // auto selectedId = event.GetItem();
            // auto data = (ProjectTreeData*)m_tree->GetItemData(selectedId);
            // if (data)
            //{
            //	auto factory = fb::static_pointer_cast<IFactory>(data->getOwnerData());
            //	auto selectedObject = factory->getObjectType();
            //	setSelectedObject(selectedObject);
            // }

            // EndModal(wxID_OK);
        }

        fb::Parameter ObjectBrowserDialog::UIElementListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( eventValue == IEvent::handleSelection )
            {
                auto owner = getOwner();
                auto tree = owner->getTree();
                auto selectedNode = tree->getSelectTreeNode();

                auto element = fb::dynamic_pointer_cast<ui::IUIElement>( sender );
                auto elementId = static_cast<ObjectBrowserDialog::WidgetId>( element->getElementId() );
                switch( elementId )
                {
                case AddComponent:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto selectionManager = applicationManager->getSelectionManager();
                    FB_ASSERT( selectionManager );

                    auto selection = selectionManager->getSelection();
                    for( auto selected : selection )
                    {
                        if( selected )
                        {
                            if( selected->isDerived<scene::IActor>() )
                            {
                                auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                                if( actor )
                                {
                                    if( selectedNode )
                                    {
                                        auto userData = selectedNode->getNodeUserData();
                                        auto projectData =
                                            fb::static_pointer_cast<ProjectTreeData>( userData );

                                        auto objectData = projectData->getObjectData();
                                        auto factory = fb::static_pointer_cast<IFactory>( objectData );

                                        auto component = static_cast<scene::IComponent *>(
                                            factory->createObjectFromPool() );
                                        actor->addComponentInstance( component );

                                        if( applicationManager->isEditor() )
                                        {
                                            component->setState( scene::IComponent::State::Edit );
                                        }
                                        else
                                        {
                                            component->setState( scene::IComponent::State::Play );
                                        }
                                    }
                                }
                            }
                        }
                    }

                    auto editorManager = EditorManager::getSingletonPtr();
                    auto ui = editorManager->getUI();
                    if( auto actorWindow = ui->getActorWindow() )
                    {
                        actorWindow->buildTree();
                    }
                }
                break;
                }
            }

            return Parameter();
        }

        ObjectBrowserDialog *ObjectBrowserDialog::UIElementListener::getOwner() const
        {
            return m_owner;
        }

        void ObjectBrowserDialog::UIElementListener::setOwner( ObjectBrowserDialog *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
