#ifndef __ParticleSystemComponent_h__
#define __ParticleSystemComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Interface/Physics/IRigidBody2.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace scene
    {
        /** @brief Component for particle system */
        class ParticleSystem : public Component
        {
        public:
            /** @brief Constructor */
            ParticleSystem();

            /** @brief Destructor */
            ~ParticleSystem() override;

            /** @copydoc Component::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** Gets the graphics object */
            SmartPtr<render::IGraphicsObject> getGraphicsObject() const;

            /** Sets the graphics object */
            void setGraphicsObject( SmartPtr<render::IGraphicsObject> graphicsObject );

            /** Gets the graphics node */
            SmartPtr<render::ISceneNode> getGraphicsNode() const;

            /** Sets the graphics node */
            void setGraphicsNode( SmartPtr<render::ISceneNode> graphicsNode );

            /** Gets the particle system */
            SmartPtr<render::IParticleSystem> getParticleSystem() const;

            /** Sets the particle system */
            void setParticleSystem( SmartPtr<render::IParticleSystem> particleSystem );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<render::IParticleSystem> m_particleSystem;
            SmartPtr<render::IGraphicsObject> m_graphicsObject;
            SmartPtr<render::ISceneNode> m_graphicsNode;

            f32 m_lifetime = 5.0f;
        };
    } // namespace scene
}     // namespace fb

#endif  // ParticleSystemComponent_h__
