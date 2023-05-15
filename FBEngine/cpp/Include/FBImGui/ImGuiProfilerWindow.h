#ifndef ImGuiProfilerWindow_h__
#define ImGuiProfilerWindow_h__

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIProfilerWindow.h>

namespace fb
{
    namespace ui
    {
        class ImGuiProfilerWindow : public CImGuiElement<IUIProfilerWindow>
        {
        public:
            /** Virtual destructor. */
            ~ImGuiProfilerWindow() override = default;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            SmartPtr<IUIProfileWindow> addProfile();

            void removeProfile( SmartPtr<IUIProfileWindow> profile );

            Array<SmartPtr<IUIProfileWindow>> getProfiles() const;

            void setProfiles( const Array<SmartPtr<IUIProfileWindow>> &profiles );

            FB_CLASS_REGISTER_DECL;

        protected:
            Array<SmartPtr<IUIProfileWindow>> m_profiles;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiProfilerWindow_h__
