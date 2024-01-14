#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiProfilerWindow.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiProfileWindow.h"

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiProfilerWindow, CImGuiElement<IUIProfilerWindow> );

    void ImGuiProfilerWindow::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( auto profiler = applicationManager->getProfiler() )
        {
            auto profiles = profiler->getProfiles();
            for( auto profile : profiles )
            {
                auto p = fb::make_ptr<ImGuiProfileWindow>();
                p->setProfile( profile );
                m_profiles.emplace_back( p );
            }
        }
    }

    void ImGuiProfilerWindow::unload( SmartPtr<ISharedObject> data )
    {
    }

    void ImGuiProfilerWindow::update()
    {
        //ImGui::Begin( "Profiler" );

        for( auto profile : m_profiles )
        {
            profile->update();
        }

        //ImGui::End();
    }

    auto ImGuiProfilerWindow::addProfile() -> SmartPtr<IUIProfileWindow>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto profile = factoryManager->make_ptr<ImGuiProfileWindow>();
        m_profiles.emplace_back( profile );
        return profile;
    }

    void ImGuiProfilerWindow::removeProfile( SmartPtr<IUIProfileWindow> profile )
    {
        auto it = std::find( m_profiles.begin(), m_profiles.end(), profile );
        if( it != m_profiles.end() )
        {
            m_profiles.erase( it );
        }
    }

    auto ImGuiProfilerWindow::getProfiles() const -> Array<SmartPtr<IUIProfileWindow>>
    {
        return m_profiles;
    }

    void ImGuiProfilerWindow::setProfiles( const Array<SmartPtr<IUIProfileWindow>> &profiles )
    {
        m_profiles = profiles;
    }

}  // namespace fb::ui
