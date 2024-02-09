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

            ColourF getAmbientColour() const;
            void setAmbientColour( const ColourF &ambientColour );

            ColourF getUpperHemisphere() const;
            void setUpperHemisphere( const ColourF &upperHemisphere );

            ColourF getLowerHemisphere() const;
            void setLowerHemisphere( const ColourF &lowerHemisphere );

            Vector3<real_Num> getHemisphereDir() const;
            void setHemisphereDir( Vector3<real_Num> hemisphereDir );

            f32 getEnvmapScale() const;
            void setEnvmapScale( f32 envmapScale );

            FB_CLASS_REGISTER_DECL;

        protected:
            ColourF m_ambientColour = ColourF::White;

            ColourF m_upperHemisphere = ColourF::White;
            ColourF m_lowerHemisphere = ColourF::White;
            Vector3<real_Num> m_hemisphereDir = Vector3<real_Num>::unitY();
            f32 m_envmapScale = 1.0f;
        };
    } // namespace scene
}     // namespace fb

#endif  // SceneLightingDirector_h__
