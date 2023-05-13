#ifndef Physics_h__
#define Physics_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBApplication/CApplicationClient.h>

namespace fb
{

    class Physics : public application::CApplicationClient
    {
    public:
        Physics();
        ~Physics() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

    protected:
        void createScene() override;

        void createPlugins() override;

        SmartPtr<scene::IActor> m_boxGround;

        Array<SmartPtr<scene::IActor>> m_boxes;
    };
}  // end namespace fb

#endif  // Physics_h__
