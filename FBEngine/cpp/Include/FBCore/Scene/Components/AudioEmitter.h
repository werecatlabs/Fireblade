#ifndef AudioEmitter_h__
#define AudioEmitter_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        /** @brief Audio emitter component.
        *  This component is used to play sounds.
        */
        class AudioEmitter : public Component
        {
        public:
            /** @brief Constructor. */
            AudioEmitter();

            /** @brief Destructor. */
            ~AudioEmitter() override;

            /** @copydoc Component::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc Component::getChildObjects */
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
