#ifndef _MAINAPP_H
#define _MAINAPP_H

#include <FBCore/FBCoreHeaders.h>
#include <FBCore/Application.h>

namespace fb
{

    class HelloWorld : public core::Application
    {
    public:
        HelloWorld();
        ~HelloWorld() override;

        void load( SmartPtr<ISharedObject> data ) override;

    protected:
        void createScene() override;

        void createPlugins() override;
    };

}  // namespace fb

#endif
