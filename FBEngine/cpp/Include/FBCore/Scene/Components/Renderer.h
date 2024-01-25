#ifndef __Renderer_h__
#define __Renderer_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        /** @class Renderer
         *  @brief A component responsible for rendering objects in the scene
         */
        class Renderer : public Component
        {
        public:
            /** @enum CastShadows
             *  @brief Enum representing the different shadow casting modes
             */
            enum class CastShadows
            {
                Off,
                On,
                DoubleSided,
                ShadowsOnly
            };

            /** @enum RecieveShadows
             *  @brief Enum representing the different shadow receiving modes
             */
            enum class RecieveShadows
            {
                Off,
                On
            };

            /** @enum Reflections
             *  @brief Enum representing the different reflection modes
             */
            enum class Reflections
            {
                Off,
                Default,
                Blend,
                Simple
            };

            /** @enum Occulsion
             *  @brief Enum representing the different occlusion modes
             */
            enum class Occulsion
            {
                Off,
                On,
                Dynamic,
                Static
            };

            /** @brief Default constructor for Renderer */
            Renderer();

            /** @brief Destructor for Renderer */
            ~Renderer() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::updateDirty */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** @copydoc BaseComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @brief Returns the shared material of the Renderer */
            SmartPtr<render::IMaterial> getSharedMaterial() const;

            /** @brief Sets the shared material of the Renderer */
            void setSharedMaterial( SmartPtr<render::IMaterial> sharedMaterial );

            /** @brief Returns the material name of the Renderer */
            String getMaterialName() const;

            /** @brief Sets the material name of the Renderer */
            void setMaterialName( const String &materialName );

            /** @brief Updates the materials used by the Renderer */
            void updateMaterials() override;

            void updateVisibility() override;

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                   const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                   SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            /** @brief Returns the graphics object used by the Renderer */
            SmartPtr<render::IGraphicsObject> getGraphicsObject() const;

            /** @brief Sets the graphics object used by the Renderer */
            void setGraphicsObject( SmartPtr<render::IGraphicsObject> graphicsObject );

            /** @brief Returns the graphics node used by the Renderer */
            SmartPtr<render::ISceneNode> &getGraphicsNode();

            /** @brief Returns the graphics node used by the Renderer */
            const SmartPtr<render::ISceneNode> &getGraphicsNode() const;

            /** @brief Sets the graphics node used by the Renderer */
            void setGraphicsNode( SmartPtr<render::ISceneNode> graphicshNode );

            void updateTransform( const Transform3<real_Num> &transform ) override;

            void updateStatic();

            FB_CLASS_REGISTER_DECL;

        protected:
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            virtual void updateMesh();

            String m_materialName;

            SmartPtr<render::IGraphicsObject> m_graphicsObject;
            SmartPtr<render::ISceneNode> m_graphicsNode;

            SmartPtr<render::IMaterial> m_sharedMaterial;

            Array<SmartPtr<render::IMaterial>> m_materials;
            Array<SmartPtr<render::IMaterial>> m_sharedMaterials;

            CastShadows m_castShadows = CastShadows::On;
            RecieveShadows m_recieveShadows = RecieveShadows::On;
            Reflections m_reflections = Reflections::Default;
            Occulsion m_occulsion = Occulsion::On;

            static u32 m_idExt;
        };

        inline SmartPtr<render::ISceneNode> &Renderer::getGraphicsNode()
        {
            return m_graphicsNode;
        }

        inline const SmartPtr<render::ISceneNode> &Renderer::getGraphicsNode() const
        {
            return m_graphicsNode;
        }
    }  // namespace scene
}  // end namespace fb

#endif  // __Renderer_h__
