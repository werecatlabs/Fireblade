#ifndef ImGuiRenderWindow_h__
#define ImGuiRenderWindow_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/ImGuiWindowT.h>
#include <FBCore/Interface/UI/IUIRenderWindow.h>

namespace fb
{
    namespace ui
    {

        class ImGuiRenderWindow : public ImGuiWindowT<IUIRenderWindow>
        {
        public:
            ImGuiRenderWindow();
            ~ImGuiRenderWindow() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void *getHWND() const;

            SmartPtr<render::IWindow> getWindow() const override;
            void setWindow( SmartPtr<render::IWindow> window ) override;

            SmartPtr<render::ITexture> getRenderTexture() const override;
            void setRenderTexture( SmartPtr<render::ITexture> renderTexture ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            WeakPtr<render::IWindow> m_window;
            WeakPtr<render::ITexture> m_renderTexture;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiWindow_h__
