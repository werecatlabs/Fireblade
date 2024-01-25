#ifndef GraphicsSettingsDirector_h__
#define GraphicsSettingsDirector_h__

#include <FBCore/Scene/Director.h>

namespace fb
{
    namespace scene
    {

        /** Graphics settings director implementation. */
        class GraphicsSettingsDirector : public Director
        {
        public:
            /** Constructor. */
            GraphicsSettingsDirector();

            /** Destructor. */
            ~GraphicsSettingsDirector() override;

            /** @copydoc IDirector::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IDirector::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
        };
    }  // namespace scene
}  // namespace fb

#endif // GraphicsSettingsDirector_h__
