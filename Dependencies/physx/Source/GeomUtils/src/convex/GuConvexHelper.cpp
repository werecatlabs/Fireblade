/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#include "GuConvexHelper.h"
#include "GuGeometryUnion.h"
#include "GuGeomUtilsInternal.h"
#include "PsUtilities.h"

using namespace physx;
using namespace Gu;

// PT: we can't call alloca in a function and we want to avoid defines or duplicating the code. This makes it a bit tricky to write.
void Gu::getScaledConvex(	PxVec3*& scaledVertices, PxU8*& scaledIndices, PxVec3* dstVertices, PxU8* dstIndices,
							bool idtConvexScale, const PxVec3* srcVerts, const PxU8* srcIndices, PxU32 nbVerts, const Cm::FastVertex2ShapeScaling& convexScaling)
{
	//pretransform convex polygon if we have scaling!
	if(idtConvexScale)	// PT: the scale is always 1 for boxes so no need to test the type
	{
		scaledVertices = const_cast<PxVec3*>(srcVerts);
		scaledIndices = const_cast<PxU8*>(srcIndices);
	}
	else
	{
		scaledIndices = dstIndices;
		scaledVertices = dstVertices;
		for(PxU32 i=0; i<nbVerts; i++)
		{
			scaledIndices[i] = Ps::to8(i);	//generate trivial indexing.
			scaledVertices[i] = convexScaling * srcVerts[srcIndices[i]];
		}
	}
}

bool Gu::getConvexData(const Gu::GeometryUnion& shape, Cm::FastVertex2ShapeScaling& scaling, PxBounds3& bounds, PolygonalData& polyData)
{
	const PxConvexMeshGeometryLL& shapeConvex = shape.get<const PxConvexMeshGeometryLL>();

	const bool idtScale = shapeConvex.scale.isIdentity();
	if(!idtScale)
		scaling.init(shapeConvex.scale);

	// PT: this version removes all the FCMPs and almost all LHS. This is temporary until
	// the legacy 3x3 matrix totally vanishes but meanwhile do NOT do useless matrix conversions,
	// it's a perfect recipe for LHS.
	PX_ASSERT(!shapeConvex.hullData->mAABB.isEmpty());
	bounds = PxBounds3::transformFast(scaling.getVertex2ShapeSkew(), shapeConvex.hullData->mAABB);

	getPolygonalData_Convex(&polyData, shapeConvex.hullData, scaling);

	// PT: non-uniform scaling invalidates the "internal objects" optimization, since our internal sphere
	// might become an ellipsoid or something. Just disable the optimization if scaling is used...
	if(!idtScale)
		polyData.mInternal.reset();

	return idtScale;
}

PxU32 Gu::findUniqueConvexEdges(PxU32 maxNbEdges, ConvexEdge* PX_RESTRICT edges, PxU32 numPolygons, const Gu::HullPolygonData* PX_RESTRICT polygons, const PxU8* PX_RESTRICT vertexData)
{
	PxU32 nbEdges = 0;

	while(numPolygons--)
	{
		const HullPolygonData& polygon = *polygons++;
		const PxU8* vRefBase = vertexData + polygon.mVRef8;
		PxU32 numEdges = polygon.mNbVerts;

		PxU32 a = numEdges - 1;
		PxU32 b = 0;
		while(numEdges--)
		{
			PxU8 vi0 =  vRefBase[a];
			PxU8 vi1 =	vRefBase[b];

			if(vi1 < vi0)
			{
				PxU8 tmp = vi0;
				vi0 = vi1;
				vi1 = tmp;
			}

			bool found=false;
			for(PxU32 i=0;i<nbEdges;i++)
			{
				if(edges[i].vref0==vi0 && edges[i].vref1==vi1)
				{
					found = true;
					edges[i].normal += polygon.mPlane.n;
					break;
				}
			}
			if(!found)
			{
				if(nbEdges==maxNbEdges)
				{
					PX_ALWAYS_ASSERT_MESSAGE("Internal error: max nb edges reached. This shouldn't be possible...");
					return nbEdges;
				}

				edges[nbEdges].vref0	= vi0;
				edges[nbEdges].vref1	= vi1;
				edges[nbEdges].normal	= polygon.mPlane.n;
				nbEdges++;
			}

			a = b;
			b++;
		}
	}
	return nbEdges;
}
