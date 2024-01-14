#ifndef MeshResourceDirector_h__
#define MeshResourceDirector_h__

#include <FBCore/Scene/Directors/ResourceDirector.h>

namespace fb
{
    namespace scene
    {
        /** Mesh resource director implementation. */
        class MeshResourceDirector : public ResourceDirector
        {
        public:
            /** Constructor. */
            MeshResourceDirector();

            /** Destructor. */
            ~MeshResourceDirector() override;

            /** @copydoc IDirector::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IDirector::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            f32 getScale() const;

            void setScale( f32 scale );

            bool getConstraints() const;

            void setConstraints( bool constraints );

            bool getVisibility() const;

            void setVisibility( bool visibility );

            bool getCameras() const;

            void setCameras( bool cameras );

            bool getSwapZY() const;

            void setSwapZY( bool swapZY );

            bool getRotate90DegreesX() const;

            void setRotate90DegreesX( bool rotate90DegreesX );

            bool getLights() const;

            void setLights( bool lights );

            bool getLightmapUVs() const;

            void setLightmapUVs( bool lightmapUVs );

            FB_CLASS_REGISTER_DECL;

        protected:
            f32 m_scale = 1.0f;

            bool m_constraints = true;
            bool m_animation = true;
            bool m_visibility = true;
            bool m_cameras = true;
            bool m_swapZY = false;
            bool m_rotate90DegreesX = false;
            bool m_lights = true;
            bool m_lightmapUVs = false;

            bool m_hasSharedVertexData = false;
        };
    }  // namespace scene
}  // namespace fb

#endif  // MeshResourceDirector_h__
