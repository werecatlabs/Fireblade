#ifndef __Component_CollisionMesh_h__
#define __Component_CollisionMesh_h__

#include <FBCore/Scene/Components/Collision.h>

namespace fb
{
    namespace scene
    {
        class CollisionMesh : public Collision
        {
        public:
            CollisionMesh();
            ~CollisionMesh() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** Gets the path of the mesh. */
            String getMeshPath() const;

            /** Sets the path of the mesh. */
            void setMeshPath( const String &meshPath );

            /** Gets the mesh resource. */
            SmartPtr<IMeshResource> getMeshResource() const;

            /** Sets the mesh resource. */
            void setMeshResource( SmartPtr<IMeshResource> meshResource );

            bool isValid() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            void createPhysicsShape();

            void setupMesh();

            bool m_isConvex = false;
            String m_meshPath;
            SmartPtr<IMeshResource> m_meshResource;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionMesh_h__
