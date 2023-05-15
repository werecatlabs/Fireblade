#ifndef MeshComponent_h__
#define MeshComponent_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {
        /**
         * Component that represents a mesh.
         */
        class Mesh : public Component
        {
        public:
            /**
             * Constructor.
             */
            Mesh();

            /**
             * Destructor.
             */
            ~Mesh() override;

            /**
             * @copydoc BaseComponent::load
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * @copydoc BaseComponent::unload
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            /**
             * Gets the path to the mesh file.
             * @return The path to the mesh file.
             */
            String getMeshPath() const;

            /**
             * Sets the path to the mesh file.
             * @param meshPath The path to the mesh file.
             */
            void setMeshPath( const String &meshPath );

            /**
             * Gets the mesh object.
             * @return The mesh object.
             */
            SmartPtr<IMesh> getMesh() const;

            /**
             * Sets the mesh object.
             * @param mesh The mesh object.
             */
            void setMesh( SmartPtr<IMesh> mesh );

            /**
             * Gets the mesh resource object.
             * @return The mesh resource object.
             */
            SmartPtr<IMeshResource> getMeshResource() const;

            /**
             * Sets the mesh resource object.
             * @param meshResource The mesh resource object.
             */
            void setMeshResource( SmartPtr<IMeshResource> meshResource );

            /**
             * Gets the data as a properties object.
             * @return The data as a properties object.
             */
            SmartPtr<Properties> getProperties() const override;

            /**
             * Sets the data as a properties object.
             * @param properties The properties object.
             */
            void setProperties( SmartPtr<Properties> properties ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /**
             * Handles component events.
             * @param state The state of the component.
             * @param eventType The type of the event.
             * @return The return value of the event.
             */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /** The mesh object. */
            SmartPtr<IMesh> m_mesh;    

            /** The mesh resource. */
            SmartPtr<IMeshResource> m_meshResource;  

            /** The path to the mesh file. */
            String m_meshPath;       

            /** The component ID extension. */
            static u32 m_idExt;                      
        };
    }  // namespace scene
}  // end namespace fb

#endif  // MeshComponent_h__
