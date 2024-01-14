#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiProfileWindow.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiProfileWindow, CImGuiElement<IUIProfileWindow> );

    void ImGuiProfileWindow::load( SmartPtr<ISharedObject> data )
    {
    }

    void ImGuiProfileWindow::unload( SmartPtr<ISharedObject> data )
    {
    }

    void ImGuiProfileWindow::update()
    {
        String label;

        auto maxSamples = 1000;
        if( auto p = getProfile() )
        {
            auto averageTimeTaken = static_cast<f32>( p->getAverageTimeTaken() );
            m_data.push_back( averageTimeTaken );

            if( m_data.size() > maxSamples )
            {
                auto num = m_data.size() - maxSamples;
                m_data.erase( m_data.begin(), m_data.begin() + num );
            }

            label = p->getLabel();
        }

        float minValue = std::numeric_limits<float>::max();
        float maxValue = std::numeric_limits<float>::min();

        // Calculate the min and max values of the data
        for( float value : m_data )
        {
            minValue = std::min( minValue, value );
            maxValue = std::max( maxValue, value );
        }

        // Add some padding to the max value (optional)
        maxValue += maxValue * 0.1f;

        auto dataPtr = m_data.data();
        auto dataSize = static_cast<int>( m_data.size() );
        ImGui::PlotHistogram( label.c_str(), dataPtr, dataSize, 0, nullptr, minValue, maxValue,
                              ImVec2( 0, 80 ) );
    }

    auto ImGuiProfileWindow::getProfile() const -> SmartPtr<IProfile>
    {
        return m_profile;
    }

    void ImGuiProfileWindow::setProfile( SmartPtr<IProfile> profile )
    {
        m_profile = profile;
    }

}  // namespace fb::ui
