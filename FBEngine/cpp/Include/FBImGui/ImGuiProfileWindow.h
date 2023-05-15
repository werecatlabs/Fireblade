#ifndef ImGuiProfileWindow_h__
#define ImGuiProfileWindow_h__

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIProfileWindow.h>

namespace fb
{
    namespace ui
    {

        class ImGuiProfileWindow : public CImGuiElement<IUIProfileWindow>
        {
        public:
            /** Virtual destructor. */
            ~ImGuiProfileWindow() override = default;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void update();

            SmartPtr<IProfile> getProfile() const;

            void setProfile( SmartPtr<IProfile> profile );

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_label;
            std::chrono::high_resolution_clock::time_point m_start;
            std::vector<float> m_data;

            SmartPtr<IProfile> m_profile;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiProfileWindow_h__
