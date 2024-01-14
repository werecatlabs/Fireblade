#ifndef TextureResourceDirector_h__
#define TextureResourceDirector_h__

#include <FBCore/Scene/Directors/ResourceDirector.h>

namespace fb
{
    namespace scene
    {
        /** Texture resource director implementation. */
        class TextureResourceDirector : public ResourceDirector
        {
        public:
            /** Constructor. */
            TextureResourceDirector();

            /** Destructor. */
            ~TextureResourceDirector() override;

            /** @copydoc IDirector::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IDirector::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_textureType;
            s32 m_textureSize = 8192;
        };

    }  // namespace scene
}  // namespace fb

#endif  // TextureResourceDirector_h__
