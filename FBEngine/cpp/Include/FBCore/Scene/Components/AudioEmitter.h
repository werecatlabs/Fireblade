#ifndef AudioEmitter_h__
#define AudioEmitter_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        class AudioEmitter : public Component
        {
        public:
            AudioEmitter();
            ~AudioEmitter() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            SmartPtr<ISound> getSound() const;

            void setSound( SmartPtr<ISound> sound );

            void play();

            void stop();

            void pause();

            void unpause();

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<ISound> m_sound;
        };

    }  // namespace scene
}  // namespace fb

#endif  // AudioEmitter_h__
