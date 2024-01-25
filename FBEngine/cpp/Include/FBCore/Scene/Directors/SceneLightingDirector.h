#ifndef SceneLightingDirector_h__
#define SceneLightingDirector_h__

#include <FBCore/Scene/Directors/ResourceDirector.h>

namespace fb
{
    namespace scene
    {

        /** Scene light resource director implementation. */
        class SceneLightingDirector : public ResourceDirector
        {
        public:
            /** Constructor. */
            SceneLightingDirector();

            /** Destructor. */
            ~SceneLightingDirector() override;

            /** @copydoc ResourceDirector::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc ResourceDirector::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            ColourF m_ambientColour = ColourF::White;

            ColourF m_upperHemisphere = ColourF::White;
            ColourF m_lowerHemisphere = ColourF::White;
            Vector3F m_hemisphereDir = Vector3F::unitY();
            f32 m_envmapScale = 1.0f;
        };

    }  // namespace scene
}  // namespace fb

#endif  // SceneLightingDirector_h__
