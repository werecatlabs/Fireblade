#include <GameEditorPCH.h>
#include "ui/ProjectWindow.h"
#include "ui/FileWindow.h"
#include "ui/ProjectAssetsWindow.h"
#include "editor/EditorManager.h"
#include "ui/ProjectTreeData.h"
#include "ui/UIManager.h"
#include "commands/DragDropActorCmd.h"
#include "commands/RemoveResourceCmd.h"
#include "commands/AddActorCmd.h"
#include "commands/RemoveSelectionCmd.h"
#include "commands/AddNewScriptCmd.h"
#include "GameEditorTypes.h"
#include <FBCore/FBCore.h>
#include "editor/Project.h"

#define TREE_ITEM_STATE_NOT_FOUND 0
#define TREE_ITEM_STATE_EXPANDED 1
#define TREE_ITEM_STATE_NOT_EXPANDED 2

namespace fb
{
    namespace editor
    {

        ProjectWindow::ProjectWindow()
        {
        }

        ProjectWindow::~ProjectWindow()
        {
        }

        void ProjectWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
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

                auto applicationManager = core::IApplicationManager::instance();
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

        int ProjectWindow::getItemState( String itemName )
        {
            std::map<String, bool>::iterator it;
            for( it = treeState.begin(); it != treeState.end(); it++ )
            {
                String name = ( *it ).first;
                if( name == itemName )
                    return ( *it ).second ? TREE_ITEM_STATE_EXPANDED : TREE_ITEM_STATE_NOT_EXPANDED;
            }

            return TREE_ITEM_STATE_NOT_FOUND;
        }

        bool ProjectWindow::isValid() const
        {
            if( m_projectAssetsWindow )
            {
                return m_projectAssetsWindow->isValid();
            }

            return false;
        }

    }  // end namespace editor
}  // end namespace fb
