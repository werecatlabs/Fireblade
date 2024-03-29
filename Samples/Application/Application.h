#ifndef _MAINAPP_H
#define _MAINAPP_H

#include <FBCore/FBCoreHeaders.h>
#include <FBCore/Application.h>

namespace fb
{

    class Application : public core::Application
    {
    public:
        enum class ElementId
        {
            Open,
            Exit,

            Count
        };

        Application();
        ~Application() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

    protected:
        void createPlugins() override;

        void createScene() override;

        void createUI() override;

        void createRenderWindow();

        SmartPtr<ui::IUIApplication> m_application;
        SmartPtr<ui::IUIRenderWindow> m_renderWindow;

        SmartPtr<render::IGraphicsObject> m_box;
        SmartPtr<render::ISceneNode> m_node;

        SmartPtr<IFrameStatistics> m_frameStatistics;
    };

}  // namespace fb

#endif
