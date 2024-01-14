#ifndef SoundResourceDirector_h__
#define SoundResourceDirector_h__

#include <FBCore/Scene/Directors/ResourceDirector.h>

namespace fb
{
    namespace scene
    {

        /** Sound resource director implementation. */
        class SoundResourceDirector : public ResourceDirector
        {
        public:
            /** Constructor. */
            SoundResourceDirector();

            /** Destructor. */
            ~SoundResourceDirector() override;

            /** @copydoc IDirector::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IDirector::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            void play();
            void stop();

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_textureType;
            s32 m_textureSize = 8192;
        };

    }  // namespace scene
}  // namespace fb

#endif  // SoundResourceDirector_h__
