#ifndef ParticleSystemComponent_h__
#define ParticleSystemComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Interface/Physics/IRigidBody2.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace scene
    {
        class ParticleSystem : public Component
        {
        public:
            ParticleSystem();
            ~ParticleSystem() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<render::IGraphicsObject> getGraphicsObject() const;
            void setGraphicsObject( SmartPtr<render::IGraphicsObject> graphicsObject );

            SmartPtr<render::ISceneNode> getGraphicshNode() const;
            void setGraphicshNode( SmartPtr<render::ISceneNode> graphicshNode );

            FB_CLASS_REGISTER_DECL;

        protected:
            SmartPtr<render::IGraphicsObject> m_graphicsObject;
            SmartPtr<render::ISceneNode> m_graphicshNode;

            f32 m_lifetime = 5.0f;
        };
    }  // namespace scene
}  // namespace fb

#endif  // ParticleSystemComponent_h__
