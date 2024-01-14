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

            /** @copydoc Renderer::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Renderer::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Renderer::updateFlags */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** @copydoc Renderer::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc Renderer::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Renderer::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            void updateMaterials() override;

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

            void updateMesh() override;

            ///< The mesh object.
            SmartPtr<render::IGraphicsMesh> m_meshObject;
        };
    } // namespace scene
}     // end namespace fb

#endif  // __MeshRenderer_h__
