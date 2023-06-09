#ifndef LightComponent_h__
#define LightComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/Graphics/ILight.h>

namespace fb
{
    namespace scene
    {

        /**
         * A component that represents a light in the scene.
         */
        class Light : public Component
        {
        public:
            Light();
            ~Light() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            /**
             * Gets the type of the light.
             */
            render::ILight::LightTypes getLightType() const;

            /**
             * Sets the type of the light.
             *
             * @param lightType The new light type.
             */
            void setLightType( render::ILight::LightTypes lightType );

            /**
             * Gets the diffuse color of the light.
             */
            ColourF getDiffuseColour() const;

            /**
             * Sets the diffuse color of the light.
             *
             * @param diffuseColour The new diffuse color.
             */
            void setDiffuseColour( const ColourF &diffuseColour );

            /**
             * Gets the specular color of the light.
             */
            ColourF getSpecularColour() const;

            /**
             * Sets the specular color of the light.
             *
             * @param specularColour The new specular color.
             */
            void setSpecularColour( const ColourF &specularColour );

            FB_CLASS_REGISTER_DECL;

        protected:
            /**
             * Gets the light object used by this component.
             */
            SmartPtr<render::ILight> getLight() const;

            /**
             * Sets the light object used by this component.
             *
             * @param light The new light object.
             */
            void setLight( SmartPtr<render::ILight> light );

            /**
             * Gets the scene node used by this component.
             */
            SmartPtr<render::ISceneNode> getSceneNode() const;

            /**
             * Sets the scene node used by this component.
             *
             * @param sceneNode The new scene node.
             */
            void setSceneNode( SmartPtr<render::ISceneNode> sceneNode );

            /** The diffuse color of the light. */
            ColourF m_diffuseColour = ColourF::White;

            /** The specular color of the light. */
            ColourF m_specularColour = ColourF::White;

            /** The type of the light. */
            render::ILight::LightTypes m_lightType = render::ILight::LightTypes::LT_DIRECTIONAL;

            /** The light object used by this component. */
            SmartPtr<render::ILight> m_light;

            /** The scene node used by this component. */
            SmartPtr<render::ISceneNode> m_sceneNode;

            /** The ID extension for the component. */
            static u32 m_nameExt;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // LightComponent_h__
