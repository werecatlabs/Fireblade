#ifndef ParticleTextureAtlas_h__
#define ParticleTextureAtlas_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>

#include <Ogre.h>

namespace fb
{
    namespace render
    {

        class ParticleTextureAtlas
        {
        public:
            ParticleTextureAtlas();

            ParticleTextureAtlas( s32 particleSize, s32 atlasSize );

            ~ParticleTextureAtlas();

            // Get the texture atlas texture
            Texture3D *getTexture() const;

            // Update the texture atlas with the given particle positions
            void update( const std::vector<Ogre::Vector3> &positions );

        private:
            s32 particleSize;
            s32 atlasSize;
            Texture3D *texture;
            std::vector<float> atlasData;
        };

    }  // namespace render
}  // namespace fb

#endif  // ParticleTextureAtlas_h__
