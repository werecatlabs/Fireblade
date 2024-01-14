#ifndef __Component_CollisionMesh_h__
#define __Component_CollisionMesh_h__

#include <FBCore/Scene/Components/Collision.h>

namespace fb
{
    namespace scene
    {

        /** Collision mesh component. */
        class CollisionMesh : public Collision
        {
        public:
            static const String meshStr;

            /** Constructor. */
            CollisionMesh();

            /** Destructor. */
            ~CollisionMesh() override;

            /** @copydoc Collision::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Collision::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Collision::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Collision::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** Gets the path of the mesh. */
            String getMeshPath() const;

            /** Sets the path of the mesh. */
            void setMeshPath( const String &meshPath );

            /** Gets the mesh resource. */
            SmartPtr<IMeshResource> getMeshResource() const;

            /** Sets the mesh resource. */
            void setMeshResource( SmartPtr<IMeshResource> meshResource );

            bool isConvex() const;

            void setConvex( bool convex );

            /** @copydoc Collision::isValid */
            bool isValid() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            /** @copydoc Collision::handleComponentEvent */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /** @copydoc Collision::createPhysicsShape */
            void createPhysicsShape() override;

            /** Sets up the physics mesh. */
            void setupMesh();

            // Used to determine if the mesh is convex or not.
            bool m_isConvex = false;

            // The path of the mesh.
            String m_meshPath;

            // The mesh resource.
            SmartPtr<IMeshResource> m_meshResource;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // CollisionMesh_h__
