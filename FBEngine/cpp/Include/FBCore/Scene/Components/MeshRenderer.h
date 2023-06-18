#ifndef __MeshRenderer_h__
#define __MeshRenderer_h__

#include <FBCore/Scene/Components/Renderer.h>

namespace fb
{
    namespace scene
    {

        /** @class MeshRenderer
         *  @brief A component responsible for rendering mesh objects in the scene, derived from the
         * Renderer class
         */
        class MeshRenderer : public Renderer
        {
        public:
            ///< Default constructor.
            MeshRenderer();

            ///< Destructor.
            ~MeshRenderer() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc BaseComponent::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            ///< Gets the shared material.
            SmartPtr<render::IMaterial> getSharedMaterial() const;

            ///< Sets the shared material.
            void setSharedMaterial( SmartPtr<render::IMaterial> sharedMaterial );

            ///< Gets the name of the material.
            String getMaterialName() const;

            ///< Sets the name of the material.
            void setMaterialName( const String &materialName );

            void updateMaterials();

            /** @copydoc IComponent::visibilityChanged */
            void visibilityChanged() override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            ///< Gets the mesh object.
            SmartPtr<render::IGraphicsMesh> getMeshObject() const;

            ///< Sets the mesh object.
            void setMeshObject( SmartPtr<render::IGraphicsMesh> meshObject );

            ///< Gets the mesh scene node.
            SmartPtr<render::ISceneNode> getMeshNode() const;

            ///< Sets the mesh scene node.
            void setMeshNode( SmartPtr<render::ISceneNode> meshNode );

            FB_CLASS_REGISTER_DECL;

        protected:
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            void updateMesh();

            ///< The mesh object.
            SmartPtr<render::IGraphicsMesh> m_meshObject;

            ///< The mesh scene node.
            SmartPtr<render::ISceneNode> m_meshNode;
        };

    }  // namespace scene
}  // end namespace fb

#endif  // __MeshRenderer_h__
