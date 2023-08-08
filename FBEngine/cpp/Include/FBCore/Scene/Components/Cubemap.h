#ifndef Cubemap_h__
#define Cubemap_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        class Cubemap : public Component
        {
        public:
            Cubemap();
            ~Cubemap() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void reload( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            SmartPtr<render::ICubemap> getCubemapGenerator() const;
            void setCubemapGenerator( SmartPtr<render::ICubemap> cubemapGenerator );

            f32 getCameraDistance() const;
            void setCameraDistance( f32 cameraDistance );

            f32 getEnableDistanceTheshold() const;
            void setEnableDistanceTheshold( f32 distanceTheshold );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<render::ICubemap> m_cubemapGenerator;
            f32 m_cameraDistance = 0.0f;
            f32 m_distanceTheshold = 0.0f;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // Cubemap_h__
