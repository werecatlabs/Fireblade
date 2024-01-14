#include <FBOISInput/FBOISInput.h>
#include <FBOISInput/FBOISInputManager.h>
#include <FBOISInput/FBInputEvent.h>
#include <FBOISInput/FBMouseState.h>
#include <FBCore/FBCore.h>

namespace fb
{
    OISInput::OISInput()
    {
    }

    OISInput::~OISInput()
    {
    }

    void OISInput::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        FactoryUtil::addFactory<OISInputManager>();
        FactoryUtil::addFactory<InputEvent>();
        FactoryUtil::addFactory<MouseState>();

        factoryManager->setPoolSizeByType<InputEvent>( 8 );
        factoryManager->setPoolSizeByType<MouseState>( 8 );
    }

    void OISInput::unload( SmartPtr<ISharedObject> data )
    {
    }

    SmartPtr<IInputDeviceManager> OISInput::createInputManager( SmartPtr<render::IWindow> window )
    {
        return fb::make_ptr<OISInputManager>( window );
    }
}  // namespace fb
