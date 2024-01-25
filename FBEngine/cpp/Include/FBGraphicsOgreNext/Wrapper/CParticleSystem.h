#ifndef _CParticleSystem_H
#define _CParticleSystem_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/ParticleSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsObjectOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CParticleNode.h>
#include <FBCore/Core/HashMap.h>

namespace fb
{
    namespace render
    {
        class CParticleSystem : public ParticleSystem
        {
        public:
            CParticleSystem();
            ~CParticleSystem() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            SmartPtr<IGraphicsObject> clone(
                const String &name = StringUtil::EmptyString ) const;

            void _getObject( void **ppObject ) const;

        protected:
            Ogre::ParticleSystem *m_particleSystem = nullptr;
        };
    } // namespace render
}     // end namespace fb

#endif
