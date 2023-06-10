#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Toggle.h>

namespace fb
{
    namespace scene
    {

        Toggle::Toggle()
        {
        }

        Toggle::~Toggle()
        {
            unload( nullptr );
        }

        void Toggle::load( SmartPtr<ISharedObject> data )
        {
        }

        void Toggle::unload( SmartPtr<ISharedObject> data )
        {
        }

    }  // namespace scene
}  // end namespace fb
