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

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "PxPhysics.h"
#include "PxConstraint.h"
#include "PxJoint.h"
#include "ExtJoint.h"
#include "ExtDistanceJoint.h"
#include "ExtD6Joint.h"
#include "ExtFixedJoint.h"
#include "ExtPrismaticJoint.h"
#include "ExtRevoluteJoint.h"
#include "ExtSphericalJoint.h"

using namespace physx;
using namespace Ext;

// PX_SERIALIZATION
using namespace Ext;

PxConstraint* physx::resolveConstraintPtr(PxDeserializationContext& v,
										  PxConstraint* old,
										  PxConstraintConnector* connector,
										  PxConstraintShaderTable &shaders)
{
	v.translatePxBase(old);
	PxConstraint* new_nx = static_cast<PxConstraint*>(old);
	new_nx->setConstraintFunctions(*connector, shaders);
	return new_nx;
}
//~PX_SERIALIZATION


static void normalToTangents(const PxVec3& n, PxVec3& t1, PxVec3& t2)
{
	const PxReal m_sqrt1_2 = PxReal(0.7071067811865475244008443621048490);
	if(fabsf(n.z) > m_sqrt1_2)
	{
		const PxReal a = n.y*n.y + n.z*n.z;
		const PxReal k = PxReal(1.0)/PxSqrt(a);
		t1 = PxVec3(0,-n.z*k,n.y*k);
		t2 = PxVec3(a*k,-n.x*t1.z,n.x*t1.y);
	}
	else 
	{
		const PxReal a = n.x*n.x + n.y*n.y;
		const PxReal k = PxReal(1.0)/PxSqrt(a);
		t1 = PxVec3(-n.y*k,n.x*k,0);
		t2 = PxVec3(-n.z*t1.y,n.z*t1.x,a*k);
	}
	t1.normalize();
	t2.normalize();
}

#include "PxMat33.h"
void PxSetJointGlobalFrame(PxJoint& joint, const PxVec3* wsAnchor, const PxVec3* axisIn)
{
	PxRigidActor* actors[2];
	joint.getActors(actors[0], actors[1]);

	PxTransform localPose[2];
	for(PxU32 i=0; i<2; i++)
		localPose[i] = PxTransform(PxIdentity);

	// 1) global anchor
	if(wsAnchor)
	{
		//transform anchorPoint to local space
		for(PxU32 i=0; i<2; i++)
			localPose[i].p = actors[i] ? actors[i]->getGlobalPose().transformInv(*wsAnchor) : *wsAnchor;
	}

	// 2) global axis
	if(axisIn)
	{
		PxVec3 localAxis[2], localNormal[2];

		//find 2 orthogonal vectors.
		//gotta do this in world space, if we choose them
		//separately in local space they won't match up in worldspace.
		PxVec3 axisw = *axisIn;
		axisw.normalize();

		PxVec3 normalw, binormalw;
		::normalToTangents(axisw, binormalw, normalw);
		//because axis is supposed to be the Z axis of a frame with the other two being X and Y, we need to negate
		//Y to make the frame right handed. Note that the above call makes a right handed frame if we pass X --> Y,Z, so 
		//it need not be changed.

		for(PxU32 i=0; i<2; i++)
		{
			if(actors[i])
			{
				const PxTransform& m = actors[i]->getGlobalPose();
				PxMat33 mM(m.q);
				localAxis[i]   = mM.transformTranspose(axisw);
				localNormal[i] = mM.transformTranspose(normalw);
			}
			else
			{
				localAxis[i] = axisw;
				localNormal[i] = normalw;
			}

			PxMat33 rot(localAxis[i], localNormal[i], localAxis[i].cross(localNormal[i]));
			
			localPose[i].q = PxQuat(rot);
			localPose[i].q.normalize();
		}
	}

	for(PxU32 i=0; i<2; i++)
		joint.setLocalPose(static_cast<PxJointActorIndex::Enum>( i ), localPose[i]);
}
