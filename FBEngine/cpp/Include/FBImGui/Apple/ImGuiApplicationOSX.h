#ifndef _FB_ImGuiApplicationOSX_H
#define _FB_ImGuiApplicationOSX_H


#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Interface/Graphics/IWindowListener.h>

namespace fb
{
    namespace ui
    {

        class ImGuiApplicationOSX
        {
        public:
            void load();
            void update();
            void postUpdate();

            void handleWindowEvent(SmartPtr<render::IWindowEvent> event);

            void draw(SmartPtr<IUIRenderWindow> renderWindow);

        protected:
            class WindowListener : public render::IWindowListener
            {
            public:
                WindowListener() = default;
                ~WindowListener() = default;

                virtual void handleEvent(SmartPtr<render::IWindowEvent> event);

                void setOwner( ImGuiApplicationOSX *owner )
                {
                    m_owner = owner;
                }

                ImGuiApplicationOSX *getOwner() const
                {
                    return m_owner;
                }

            protected:
                ImGuiApplicationOSX *m_owner = nullptr;
            };

            Ogre::MetalDevice* m_device = nullptr;
            SmartPtr<render::IWindow> m_window;
        };
    }
}

#endif