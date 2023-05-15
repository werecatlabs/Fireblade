#ifndef TextureComponent_h__
#define TextureComponent_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        class Texture : public Component
        {
        public:
            Texture();

            /** Copy constructor is deleted. */
            Texture( const Texture &other ) = delete;

            /** Destructor. */
            ~Texture() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // TextureComponent_h__
