#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CParticleRenderer.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {

        PUParticleRenderer::PUParticleRenderer( ParticleUniverse::ParticleRenderer *renderer ) :
            m_renderer( renderer )
        {
        }

        PUParticleRenderer::PUParticleRenderer()
        {
        }

        PUParticleRenderer::~PUParticleRenderer()
        {
        }

    }  // namespace render
}  // namespace fb
