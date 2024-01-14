#ifndef _MeshUtil_H
#define _MeshUtil_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Mesh/IVertexElement.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Matrix4.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Mesh/MeshTransformData.h>

namespace fb
{

    /** Mesh utility class. */
    class MeshUtil
    {
    public:
        /** Cleans a mesh. */
        static SmartPtr<IMesh> clean( SmartPtr<IMesh> mesh, real_Num weldTolerance );

        /** Cleans a submesh. */
        static SmartPtr<ISubMesh> clean( SmartPtr<ISubMesh> submesh, real_Num weldTolerance );

        /** Gets points from a mesh. */
        static Array<Vector3F> getPoints( SmartPtr<IMesh> mesh );

        /** Gets points from a submesh. */
        static Array<Vector3F> getPoints( SmartPtr<ISubMesh> submesh );

        /** Gets indices from a mesh. */
        static Array<u32> getIndices( SmartPtr<IMesh> mesh );

        /** Gets indices from a submesh. */
        static Array<u32> getIndices( SmartPtr<ISubMesh> submesh );

        /** Create a height map from a mesh. */
        static void meshToHeightMap( const String &meshName, const String &textureName,
                                     const Vector2I &textureSize );

        /** Builds a mesh from an array of points and uvs. */
        static SmartPtr<IMesh> buildMesh( const Array<Vector3F> &positions, const Array<Vector2F> &uvs,
                                          const Array<Vector2F> &uvs2 );

        /** Builds a sub mesh from an array of points and uvs. */
        static SmartPtr<ISubMesh> buildSubMesh( const Array<Vector3F> &positions,
                                                const Array<Vector2F> &uvs,
                                                const Array<Vector2F> &uvs2 );

        /** Creates a merged mesh from transform data. */
        static SmartPtr<IMesh> mergeMeshes( const Array<MeshTransformData> &meshTransformData );

        /** Creates a merged mesh from an array of meshes. */
        static SmartPtr<IMesh> mergeMeshes( const Array<SmartPtr<IMesh>> &meshes );

        /** Creates a merged mesh from an array of meshes and transform matrices. */
        static SmartPtr<IMesh> mergeMeshes( const Array<SmartPtr<IMesh>> &meshes,
                                            const Array<Matrix4F> &transformations );

        /** Merges sub meshes by material. */
        static SmartPtr<IMesh> mergeSubMeshesByMaterial( SmartPtr<IMesh> mesh );

        /** Returns a boolean indicating whether or not the mesh is valid. */
        static bool isMeshValid( SmartPtr<IMesh> mesh );

        /** Gets a mesh from height data. */
        static SmartPtr<IMesh> getMesh( const Array<f32> &heightData, f32 worldScale, f32 heightScale,
                                        u32 tileSize );

        /** Gets an index in the height map. */
        static u32 getIndex( u32 tileSize, u32 x, u32 z );

        /** Unshares vertices in a mesh. */
        static void unshareVertices( SmartPtr<IMesh> mesh );

        /// Utility method which returns the count of values in a given type (result for colors may be
        /// counter-intuitive)
        static u16 getTypeCount( IVertexElement::VertexElementType etype );

        /** Gets the multiplyTypeCount from the base type. */
        static IVertexElement::VertexElementType multiplyTypeCount(
            IVertexElement::VertexElementType baseType, u16 count );

        /** Simple converter function which will turn a type into it's single-value (or lowest
           multiple-value) equivalent - makes switches on type easier.  May give counter-intuitive
           results with bytes or shorts.
        */
        static IVertexElement::VertexElementType getBaseType(
            IVertexElement::VertexElementType multiType );

        /** Utility method for converting colour from
            one packed 32-bit colour type to another.
        @param srcType The source type
        @param dstType The destination type
        @param ptr Read / write value to change
        */
        static void convertColourValue( IVertexElement::VertexElementType srcType,
                                        IVertexElement::VertexElementType dstType, u32 *ptr );

        /** Utility method for converting colour to
            a packed 32-bit colour type.
        @param src source colour
        @param dst The destination type
        */
        static u32 convertColourValue( const ColourF &src, IVertexElement::VertexElementType dst );

        /** Utility method to get the most appropriate packed colour vertex element format. */
        static IVertexElement::VertexElementType getBestColourVertexElementType( void );
    };
}  // end namespace fb

#endif
