#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/UI/Slider.h>

namespace fb
{
    namespace scene
    {
        Slider::Slider()
        {
        }

        Slider::~Slider()
        {
            unload( nullptr );
        }

        void Slider::load( SmartPtr<ISharedObject> data )
        {
        }

        void Slider::unload( SmartPtr<ISharedObject> data )
        {
        }
    }  // namespace scene
}  // end namespace fb
