#ifndef FBOISInput_h__
#define FBOISInput_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    class OISInput : public SharedObject<ISharedObject>
    {
    public:
        OISInput();
        ~OISInput() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        static SmartPtr<IInputDeviceManager> createInputManager( SmartPtr<render::IWindow> window );
    };
}  // namespace fb

#endif  // FBOISInput_h__
