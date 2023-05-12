#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/Texture.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, Texture, IComponent );

        Texture::Texture()
        {
        }

        Texture::~Texture()
        {
        }

        void Texture::load( SmartPtr<ISharedObject> data )
        {
        }

        void Texture::unload( SmartPtr<ISharedObject> data )
        {
        }

    }  // namespace scene
}  // end namespace fb
