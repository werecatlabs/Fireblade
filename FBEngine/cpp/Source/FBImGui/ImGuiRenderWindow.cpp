#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiRenderWindow.h>
#include <FBImGui/ImGuiApplication.h>
#include <FBImGui/ImGuiManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {

        ImGuiRenderWindow::ImGuiRenderWindow()
        {
        }

        ImGuiRenderWindow::~ImGuiRenderWindow()
        {
            unload( nullptr );
        }

        void *ImGuiRenderWindow::getHWND() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto uiManager = fb::static_pointer_cast<ImGuiManager>( applicationManager->getUI() );
            FB_ASSERT( uiManager );

            if( uiManager )
            {
                auto application = uiManager->getApplication();
                if( application )
                {
                    auto pApplication = fb::static_pointer_cast<ImGuiApplication>( application );
                    return pApplication->getHWND();
                }
            }

            return nullptr;
        }

        SmartPtr<render::IWindow> ImGuiRenderWindow::getWindow() const
        {
            return m_window.lock();
        }

        void ImGuiRenderWindow::setWindow( SmartPtr<render::IWindow> window )
        {
            m_window = window;
        }

        SmartPtr<render::ITexture> ImGuiRenderWindow::getRenderTexture() const
        {
            return m_renderTexture.lock();
        }

        void ImGuiRenderWindow::setRenderTexture( SmartPtr<render::ITexture> renderTexture )
        {
            m_renderTexture = renderTexture;
        }

        void ImGuiRenderWindow::load( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Loading );

            // auto applicationManager = core::IApplicationManager::instance();
            // FB_ASSERT(applicationManager);

            // auto graphicsSystem = applicationManager->getGraphicsSystem();
            // FB_ASSERT(graphicsSystem);

            // if (graphicsSystem)
            //{
            //	auto width = 400;
            //	auto height = 400;
            //	auto windowName = String("RenderWindow");

            //	auto properties = fb::make_ptr<Properties>();

            //	static u32 nameExt = 0;
            //	String name = String("Window") + StringUtil::toString(nameExt++);

            //	auto pHandle = getHWND();
            //	auto uiHandle = (size_t)(pHandle);
            //	auto handle = std::to_string(uiHandle);

            //	properties->setProperty("WindowHandle", handle);

            //	m_window = graphicsSystem->createRenderWindow(windowName, width, height, false,
            // properties);
            //}

            setLoadingState( LoadingState::Loaded );
        }

        void ImGuiRenderWindow::unload( SmartPtr<ISharedObject> data )
        {
            setLoadingState( LoadingState::Unloading );

            m_window = nullptr;
            m_renderTexture = nullptr;
            CImGuiElement<IUIRenderWindow>::unload( nullptr );

            setLoadingState( LoadingState::Unloaded );
        }
    }  // end namespace ui
}  // end namespace fb
