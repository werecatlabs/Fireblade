/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef __FB_MeshSerializerImpl_H__
#define __FB_MeshSerializerImpl_H__

#include <FBCore/FBCorePrerequisites.h>
#include "FBCore/Core/StringTypes.h"
#include "FBCore/Mesh/Serializer.h"
#include "FBCore/Mesh/VertexBoneAssignment.h"
#include "FBCore/Mesh/VertexElement.h"

namespace fb
{
    class MeshSerializerListener;
    class LodStrategy;

    /** \addtogroup Core
     *  @{
     */
    /** \addtogroup Resources
     *  @{
     */
    /** Internal implementation of Mesh reading / writing for the latest version of the
    .mesh format.
    @remarks
    In order to maintain compatibility with older versions of the .mesh format, there
    will be alternative subclasses of this class to load older versions, whilst this class
    will remain to load the latest version.

     @note
        This mesh format was used from Ogre v1.8.

    */
    class MeshSerializerImpl : public Serializer
    {
    public:
        MeshSerializerImpl();
        ~MeshSerializerImpl() override;

        /** Exports a mesh to the file specified.
        @remarks
        This method takes an externally created Mesh object, and exports both it
        and optionally the Materials it uses to a .mesh file.
        @param pMesh Pointer to the Mesh to export
        @param stream The destination stream
        @param endianMode The endian mode for the written file
        */
        void exportMesh( const Mesh *pMesh, SmartPtr<IStream> stream, u32 endianMode = ENDIAN_NATIVE );

        /** Imports Mesh and (optionally) Material data from a .mesh file DataStream.
        @remarks
        This method imports data from a DataStream opened from a .mesh file and places it's
        contents into the Mesh object which is passed in.
        @param stream The DataStream holding the .mesh data. Must be initialised (pos at the start of the
        buffer).
        @param pDest Pointer to the Mesh object which will receive the data. Should be blank already.
        */
        void importMesh( SmartPtr<IStream> &stream, Mesh *pDest, MeshSerializerListener *listener );

    protected:
        // Internal methods
        virtual void writeSubMeshNameTable( const Mesh *pMesh );
        virtual void writeMesh( const Mesh *pMesh );
        virtual void writeSubMesh( const SubMesh *s );
        virtual void writeSubMeshOperation( const SubMesh *s );
        virtual void writeSubMeshTextureAliases( const SubMesh *s );
        virtual void writeGeometry( const VertexBuffer *pGeom );
        virtual void writeSkeletonLink( const String &skelName );
        virtual void writeMeshBoneAssignment( const VertexBoneAssignment &assign );
        virtual void writeSubMeshBoneAssignment( const VertexBoneAssignment &assign );
        virtual void writeLodInfo( const Mesh *pMesh );
        virtual void writeLodSummary( unsigned short numLevels, bool manual,
                                      const LodStrategy *strategy );
        // virtual void writeLodUsageManual(const MeshLodUsage& usage);
        // virtual void writeLodUsageGenerated(const Mesh* pMesh, const MeshLodUsage& usage, unsigned
        // short lodNum);
        virtual void writeBoundsInfo( const Mesh *pMesh );
        virtual void writeEdgeList( const Mesh *pMesh );
        virtual void writeAnimations( const Mesh *pMesh );
        // virtual void writeAnimation(const Animation* anim);
        virtual void writePoses( const Mesh *pMesh );
        // virtual void writePose(const Pose* pose);
        // virtual void writeAnimationTrack(const VertexAnimationTrack* track);
        // virtual void writeMorphKeyframe(const VertexMorphKeyFrame* kf, u32 vertexCount);
        // virtual void writePoseKeyframe(const VertexPoseKeyFrame* kf);
        // virtual void writePoseKeyframePoseRef(const VertexPoseKeyFrame::PoseRef& poseRef);
        virtual void writeExtremes( const Mesh *pMesh );
        virtual void writeSubMeshExtremes( unsigned short idx, const SubMesh *s );

        virtual u32 calcMeshSize( const Mesh *pMesh );
        virtual u32 calcSubMeshSize( const SubMesh *pSub );
        virtual u32 calcGeometrySize( const VertexBuffer *pGeom );
        virtual u32 calcSkeletonLinkSize( const String &skelName );
        virtual u32 calcBoneAssignmentSize( void );
        virtual u32 calcSubMeshOperationSize( const SubMesh *pSub );
        virtual u32 calcSubMeshNameTableSize( const Mesh *pMesh );
        virtual u32 calcEdgeListSize( const Mesh *pMesh );
        // virtual u32 calcEdgeListLodSize(const EdgeData* data, bool isManual);
        // virtual u32 calcEdgeGroupSize(const EdgeData::EdgeGroup& group);
        virtual u32 calcPosesSize( const Mesh *pMesh );
        // virtual u32 calcPoseSize(const Pose* pose);
        virtual u32 calcAnimationsSize( const Mesh *pMesh );
        ////virtual u32 calcAnimationSize(const Animation* anim);
        /// virtual u32 calcAnimationTrackSize(const VertexAnimationTrack* track);
        // virtual u32 calcMorphKeyframeSize(const VertexMorphKeyFrame* kf, u32 vertexCount);
        // virtual u32 calcPoseKeyframeSize(const VertexPoseKeyFrame* kf);
        virtual u32 calcPoseKeyframePoseRefSize( void );
        // virtual u32 calcPoseVertexSize(const Pose* pose);
        virtual u32 calcSubMeshTextureAliasesSize( const SubMesh *pSub );

        virtual void readTextureLayer( SmartPtr<IStream> &stream, Mesh *pMesh,
                                       SmartPtr<render::IMaterial> pMat );
        virtual void readSubMeshNameTable( SmartPtr<IStream> &stream, Mesh *pMesh );
        virtual void readMesh( SmartPtr<IStream> &stream, Mesh *pMesh,
                               MeshSerializerListener *listener );
        virtual void readSubMesh( SmartPtr<IStream> &stream, Mesh *pMesh,
                                  MeshSerializerListener *listener );
        virtual void readSubMeshOperation( SmartPtr<IStream> &stream, Mesh *pMesh, SubMesh *sub );
        virtual void readSubMeshTextureAlias( SmartPtr<IStream> &stream, Mesh *pMesh, SubMesh *sub );
        virtual void readGeometry( SmartPtr<IStream> &stream, Mesh *pMesh, VertexBuffer *dest );
        virtual void readGeometryVertexDeclaration( SmartPtr<IStream> &stream, Mesh *pMesh,
                                                    VertexBuffer *dest );
        virtual void readGeometryVertexElement( SmartPtr<IStream> &stream, Mesh *pMesh,
                                                VertexBuffer *dest );
        virtual void readGeometryVertexBuffer( SmartPtr<IStream> &stream, Mesh *pMesh,
                                               VertexBuffer *dest );

        virtual void readSkeletonLink( SmartPtr<IStream> &stream, Mesh *pMesh,
                                       MeshSerializerListener *listener );
        virtual void readMeshBoneAssignment( SmartPtr<IStream> &stream, Mesh *pMesh );
        virtual void readSubMeshBoneAssignment( SmartPtr<IStream> &stream, Mesh *pMesh, SubMesh *sub );
        virtual void readMeshLodInfo( SmartPtr<IStream> &stream, Mesh *pMesh );
        // virtual void readMeshLodUsageManual(SmartPtr<IStream>& stream, Mesh* pMesh,
        //    unsigned short lodNum, MeshLodUsage& usage);
        // virtual void readMeshLodUsageGenerated(SmartPtr<IStream>& stream, Mesh* pMesh,
        //    unsigned short lodNum, MeshLodUsage& usage);
        virtual void readBoundsInfo( SmartPtr<IStream> &stream, Mesh *pMesh );
        virtual void readEdgeList( SmartPtr<IStream> &stream, Mesh *pMesh );
        // virtual void readEdgeListLodInfo(SmartPtr<IStream>& stream, EdgeData* edgeData);
        virtual void readPoses( SmartPtr<IStream> &stream, Mesh *pMesh );
        virtual void readPose( SmartPtr<IStream> &stream, Mesh *pMesh );
        virtual void readAnimations( SmartPtr<IStream> &stream, Mesh *pMesh );
        virtual void readAnimation( SmartPtr<IStream> &stream, Mesh *pMesh );
        virtual void readAnimationTrack( SmartPtr<IStream> &stream, SmartPtr<IAnimation> anim,
                                         Mesh *pMesh );
        virtual void readMorphKeyFrame( SmartPtr<IStream> &stream,
                                        SmartPtr<IAnimationVertexTrack> track );
        virtual void readPoseKeyFrame( SmartPtr<IStream> &stream,
                                       SmartPtr<IAnimationVertexTrack> track );
        virtual void readExtremes( SmartPtr<IStream> &stream, Mesh *pMesh );

        /// Flip an entire vertex buffer from little endian
        virtual void flipFromLittleEndian( void *pData, u32 vertexCount, u32 vertexSize,
                                           const Array<SmartPtr<IVertexElement>> &elems );
        /// Flip an entire vertex buffer to little endian
        virtual void flipToLittleEndian( void *pData, u32 vertexCount, u32 vertexSize,
                                         const Array<SmartPtr<IVertexElement>> &elems );
        /// Flip the endianness of an entire vertex buffer, passed in as a
        /// pointer to locked or temporary memory
        virtual void flipEndian( void *pData, u32 vertexCount, u32 vertexSize,
                                 const Array<SmartPtr<IVertexElement>> &elems );
    };

    /** Class for providing backwards-compatibility for loading version 1.41 of the .mesh format.
     This mesh format was used from Ogre v1.7.
     */
    class MeshSerializerImpl_v1_41 : public MeshSerializerImpl
    {
    public:
        MeshSerializerImpl_v1_41();
        ~MeshSerializerImpl_v1_41() override;

    protected:
        // void writeMorphKeyframe(const VertexMorphKeyFrame* kf, u32 vertexCount);
        // void readMorphKeyFrame(SmartPtr<IStream>& stream, VertexAnimationTrack* track);
        // void writePose(const Pose* pose);
        // void readPose(SmartPtr<IStream>& stream, Mesh* pMesh);
        // u32 calcMorphKeyframeSize(const VertexMorphKeyFrame* kf, u32 vertexCount);
        // u32 calcPoseSize(const Pose* pose);
        u32 calcPoseVertexSize( void );
    };

    /** Class for providing backwards-compatibility for loading version 1.4 of the .mesh format.
     This mesh format was used from Ogre v1.4.
     */
    class MeshSerializerImpl_v1_4 : public MeshSerializerImpl_v1_41
    {
    public:
        MeshSerializerImpl_v1_4();
        ~MeshSerializerImpl_v1_4() override;

    protected:
        // virtual void writeLodSummary(unsigned short numLevels, bool manual, const LodStrategy
        // *strategy); virtual void writeLodUsageManual(const MeshLodUsage& usage); virtual void
        // writeLodUsageGenerated(const Mesh* pMesh, const MeshLodUsage& usage, 							unsigned short lodNum);

        void readMeshLodInfo( SmartPtr<IStream> &stream, Mesh *pMesh ) override;
    };

    /** Class for providing backwards-compatibility for loading version 1.3 of the .mesh format.
     This mesh format was used from Ogre v1.0 (and some pre-releases)
     */
    class MeshSerializerImpl_v1_3 : public MeshSerializerImpl_v1_4
    {
    public:
        MeshSerializerImpl_v1_3();
        ~MeshSerializerImpl_v1_3() override;

    protected:
        // virtual void readEdgeListLodInfo(SmartPtr<IStream>& stream, EdgeData* edgeData);

        /// Reorganise triangles of the edge list to group by vertex set
        // virtual void reorganiseTriangles(EdgeData* edgeData);

        // virtual void writeEdgeList(const Mesh* pMesh);
    };

    /** Class for providing backwards-compatibility for loading version 1.2 of the .mesh format.
     This is a LEGACY FORMAT that pre-dates version Ogre 1.0
     */
    class MeshSerializerImpl_v1_2 : public MeshSerializerImpl_v1_3
    {
    public:
        MeshSerializerImpl_v1_2();
        ~MeshSerializerImpl_v1_2() override;

    protected:
        void readMesh( SmartPtr<IStream> &stream, Mesh *pMesh,
                       MeshSerializerListener *listener ) override;
        void readGeometry( SmartPtr<IStream> &stream, Mesh *pMesh, VertexBuffer *dest ) override;
        virtual void readGeometryPositions( unsigned short bindIdx, SmartPtr<IStream> &stream,
                                            Mesh *pMesh, VertexBuffer *dest );
        virtual void readGeometryNormals( unsigned short bindIdx, SmartPtr<IStream> &stream, Mesh *pMesh,
                                          VertexBuffer *dest );
        virtual void readGeometryColours( unsigned short bindIdx, SmartPtr<IStream> &stream, Mesh *pMesh,
                                          VertexBuffer *dest );
        virtual void readGeometryTexCoords( unsigned short bindIdx, SmartPtr<IStream> &stream,
                                            Mesh *pMesh, VertexBuffer *dest, unsigned short set );
    };

    /** Class for providing backwards-compatibility for loading version 1.1 of the .mesh format.
     This is a LEGACY FORMAT that pre-dates version Ogre 1.0
     */
    class MeshSerializerImpl_v1_1 : public MeshSerializerImpl_v1_2
    {
    public:
        MeshSerializerImpl_v1_1();
        ~MeshSerializerImpl_v1_1() override;

    protected:
        void readGeometryTexCoords( unsigned short bindIdx, SmartPtr<IStream> &stream, Mesh *pMesh,
                                    VertexBuffer *dest, unsigned short set ) override;
    };

    /** @} */
    /** @} */
}  // namespace fb

#endif
