#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiWindow.h>
#include <FBImGui/ImGuiApplication.h>
#include "FBCore/Interface/UI/IUIManager.h"
#include "FBCore/Core/LogManager.h"
#include <imgui.h>

namespace fb::ui
{

    FB_CLASS_REGISTER_DERIVED( fb::ui, ImGuiWindow, ImGuiWindowT<IUIWindow> );

    ImGuiWindow::ImGuiWindow() = default;

    ImGuiWindow::~ImGuiWindow() = default;

    void ImGuiWindow::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto ui = applicationManager->getUI();
        auto uiApplication = fb::static_pointer_cast<ImGuiApplication>( ui->getApplication() );

        auto window = this;
        if( window->isVisible() )
        {
            if( !window->isDocked() )
            {
                ImGui::SetNextWindowDockID( uiApplication->dockLeft_id_right, ImGuiCond_FirstUseEver );
                window->setDocked( true );
            }

            auto windowParent = window->getParent();
            if( windowParent == nullptr )
            {
                auto label = window->getLabel();
                if( StringUtil::isNullOrEmpty( label ) )
                {
                    label = "Untitled";
                }

                auto windowFlags = static_cast<s32>( ImGuiWindowFlags_None );
                windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

                auto visible = window->isVisible();
                if( ImGui::Begin( label.c_str(), &visible, windowFlags ) )
                {
                    try
                    {
                        if( auto p = window->getChildren() )
                        {
                            auto &children = *p;
                            for( auto child : children )
                            {
                                uiApplication->createElement( child );
                            }
                        }

                        auto dropTarget = window->getDropTarget();
                        if( dropTarget )
                        {
                            if( ImGui::BeginDragDropTarget() )
                            {
                                auto payload = ImGui::AcceptDragDropPayload( "_TREENODE" );
                                if( payload )
                                {
                                    auto pData = static_cast<const char *>( payload->Data );
                                    auto dataSize = payload->DataSize;
                                    auto data = String( pData, dataSize );

                                    auto args = Array<Parameter>();
                                    args.reserve( 1 );

                                    args.emplace_back( data );

                                    dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop, args,
                                                             window, window, nullptr );
                                }

                                ImGui::EndDragDropTarget();
                            }
                        }

                        if( auto contextMenu = window->getContextMenu() )
                        {
                            if( ImGui::BeginPopupContextWindow( label.c_str(),
                                                                ImGuiPopupFlags_MouseButtonRight ) )
                            {
                                try
                                {
                                    auto label = contextMenu->getLabel();
                                    if( StringUtil::isNullOrEmpty( label ) )
                                    {
                                        label = "Untitled";
                                    }

                                    //if( ImGui::BeginMenu( label.c_str() ) )
                                    {
                                        try
                                        {
                                            auto menuItems = contextMenu->getMenuItems();
                                            for( auto menuItemElement : menuItems )
                                            {
                                                uiApplication->createMenuItem( contextMenu,
                                                                               menuItemElement );
                                            }
                                        }
                                        catch( std::exception &e )
                                        {
                                            FB_LOG_EXCEPTION( e );
                                        }

                                        //ImGui::EndMenu();
                                    }
                                }
                                catch( std::exception &e )
                                {
                                    FB_LOG_EXCEPTION( e );
                                }

                                ImGui::EndPopup();
                            }
                        }

                        const bool is_hovered = ImGui::IsItemHovered();  // Hovered
                        const bool is_active = ImGui::IsItemActive();    // Held

                        if( is_hovered && ImGui::IsMouseClicked( 0 ) )
                        {
                            auto listeners = window->getObjectListeners();
                            for( auto listener : listeners )
                            {
                                auto args = Array<Parameter>();
                                args.resize( 1 );

                                args[0].object = window;

                                listener->handleEvent( IEvent::Type::UI, IEvent::handleMouseClicked,
                                                       args, window, this, nullptr );
                            }
                        }

                        window->setVisible( visible, false );
                    }
                    catch( std::exception &e )
                    {
                        FB_LOG_EXCEPTION( e );
                    }
                }

                ImGui::End();
            }
        }
    }

}  // namespace fb::ui
