#ifndef IPhysicsMesh_h__
#define IPhysicsMesh_h__

#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb
{
    namespace physics
    {
        
        /** Interface for a mesh shape. */
        class IMeshShape : public IPhysicsShape3
        {
        public:
            /** Destructor. */
            ~IMeshShape() override = default;

            /** Gets the mesh object. */
            virtual SmartPtr<IMesh> getMesh() const = 0;

            /** Gets the mesh object. */
            virtual SmartPtr<IMeshResource> getMeshResource() const = 0;

            /** Sets the mesh object. */
            virtual void setMeshResource( SmartPtr<IMeshResource> meshResource ) = 0;

            /** Gets the processed mesh object. */
            virtual SmartPtr<IMesh> getCleanMesh() const = 0;

            /** Sets the processed mesh object. */
            virtual void setCleanMesh( SmartPtr<IMesh> cleanMesh ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsMesh_h__
