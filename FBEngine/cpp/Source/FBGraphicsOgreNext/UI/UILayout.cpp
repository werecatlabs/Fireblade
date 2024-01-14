#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/UI/UILayout.h>
#include "FBGraphicsOgreNext/UI/UIManager.h"
#include "ColibriGui/ColibriWindow.h"
#include "ColibriGui/ColibriManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, UILayout, UIElement<IUILayout> );

        UILayout::UILayout()
        {
            createStateContext();
        }

        UILayout::~UILayout()
        {
            unload( nullptr );
        }

        void UILayout::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::ApplicationManager::instance();
                auto ui = fb::static_pointer_cast<UIManager>( applicationManager->getUI() );

                //auto colibriManager = ui->getColibriManager();
                //m_window = colibriManager->createWindow( 0 );

                //setWidget( m_window );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void UILayout::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                m_window = nullptr;
                UIElement<ui::IUILayout>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IFSM> UILayout::getFSM()
        {
            return nullptr;
        }

        SmartPtr<ui::IUIWindow> UILayout::getUiWindow() const
        {
            return nullptr;
        }

        void UILayout::setUiWindow( SmartPtr<ui::IUIWindow> uiWindow )
        {
        }
    }  // namespace ui
}  // namespace fb
