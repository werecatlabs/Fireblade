#include <GameEditorPCH.h>
#include <ui/ProjectWindow.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/FileWindow.h>
#include <ui/ProjectAssetsWindow.h>
#include <ui/ProjectTreeData.h>
#include <ui/UIManager.h>
#include <commands/DragDropActorCmd.h>
#include <commands/RemoveResourceCmd.h>
#include <commands/AddActorCmd.h>
#include <commands/RemoveSelectionCmd.h>
#include <commands/AddNewScriptCmd.h>
#include <FBCore/FBCore.h>

enum
{
    TREE_ITEM_STATE_NOT_FOUND = 0,
    TREE_ITEM_STATE_EXPANDED = 1,
    TREE_ITEM_STATE_NOT_EXPANDED = 2
};

namespace fb::editor
{

    ProjectWindow::ProjectWindow() = default;

    ProjectWindow::~ProjectWindow()
    {
        unload( nullptr );
    }

    void ProjectWindow::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto parentWindow = ui->addElementByType<ui::IUIWindow>();
            parentWindow->setLabel( "Project" );
            setParentWindow( parentWindow );

            m_projectAssetsWindow = fb::make_ptr<ProjectAssetsWindow>();
            m_projectAssetsWindow->setParent( parentWindow );
            m_projectAssetsWindow->load( nullptr );

            m_fileWindow = fb::make_ptr<FileWindow>();
            m_fileWindow->setParent( parentWindow );
            m_fileWindow->load( nullptr );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ProjectWindow::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            if( m_projectAssetsWindow )
            {
                m_projectAssetsWindow->unload( nullptr );
                m_projectAssetsWindow = nullptr;
            }

            if( m_fileWindow )
            {
                m_fileWindow->unload( nullptr );
                m_fileWindow = nullptr;
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ProjectWindow::buildTree()
    {
        try
        {
            if( m_projectAssetsWindow )
            {
                m_projectAssetsWindow->build();
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto ProjectWindow::getItemState( String itemName ) -> int
    {
        std::map<String, bool>::iterator it;
        for( it = treeState.begin(); it != treeState.end(); it++ )
        {
            String name = ( *it ).first;
            if( name == itemName )
            {
                return ( *it ).second ? TREE_ITEM_STATE_EXPANDED : TREE_ITEM_STATE_NOT_EXPANDED;
            }
        }

        return TREE_ITEM_STATE_NOT_FOUND;
    }

    auto ProjectWindow::isValid() const -> bool
    {
        if( m_projectAssetsWindow )
        {
            return m_projectAssetsWindow->isValid();
        }

        return false;
    }

}  // namespace fb::editor
