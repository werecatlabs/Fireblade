#ifndef ISubMesh_h__
#define ISubMesh_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    /**
     * @brief Interface for a submesh, representing a part of a larger mesh.
     */
    class ISubMesh : public ISharedObject
    {
    public:
        /**
         * @brief Destructor.
         */
        ~ISubMesh() override = default;

        /**
         * @brief Sets the material name.
         *
         * @param materialName The name of the material to use for this submesh.
         */
        virtual void setMaterialName( const String &materialName ) = 0;

        /**
         * @brief Gets the material name.
         *
         * @return The name of the material used for this submesh.
         */
        virtual String getMaterialName() const = 0;

        /**
         * @brief Sets the vertex buffer.
         *
         * @param vertexBuffer The vertex buffer to use for this submesh.
         */
        virtual void setVertexBuffer( SmartPtr<IVertexBuffer> vertexBuffer ) = 0;

        /**
         * @brief Gets the vertex buffer.
         *
         * @return The vertex buffer used for this submesh.
         */
        virtual SmartPtr<IVertexBuffer> getVertexBuffer() const = 0;

        /**
         * @brief Sets the index buffer.
         *
         * @param indexBuffer The index buffer to use for this submesh.
         */
        virtual void setIndexBuffer( SmartPtr<IIndexBuffer> indexBuffer ) = 0;

        /**
         * @brief Gets the index buffer.
         *
         * @return The index buffer used for this submesh.
         */
        virtual SmartPtr<IIndexBuffer> getIndexBuffer() const = 0;

        /**
         * @brief Updates the axis-aligned bounding box (AABB) for this submesh.
         */
        virtual void updateAABB() = 0;

        /**
         * @brief Gets the AABB for this submesh.
         *
         * @return The AABB of the submesh.
         */
        virtual AABB3<real_Num> getAABB() const = 0;

        /**
         * @brief Sets the AABB for this submesh.
         *
         * @param aabb The new AABB for the submesh.
         */
        virtual void setAABB( const AABB3<real_Num> &aabb ) = 0;

        /**
         * @brief Clones this submesh.
         *
         * @return A new submesh that is a copy of this one.
         */
        virtual SmartPtr<ISubMesh> clone() const = 0;

        /**
         * @brief Gets a value indicating whether this submesh uses shared vertices.
         *
         * @return True if this submesh uses shared vertices, false otherwise.
         */
        virtual bool getUseSharedVertices() const = 0;

        /**
         * @brief Sets a value indicating whether this submesh uses shared vertices.
         *
         * @param useSharedVertices True to use shared vertices, false to use unique vertices.
         */
        virtual void setUseSharedVertices( bool useSharedVertices ) = 0;

        /**
         * @brief Compares this submesh with another.
         *
         * @param other The submesh to compare with. */
        virtual bool compare( SmartPtr<ISubMesh> other ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // ISubMesh
