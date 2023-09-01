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


#include "PxsContactManager.h"
#include "PxsRigidBody.h"
#include "PxcContactMethodImpl.h"
#include "PxsThreadContext.h"
#include "PxcSolverConstraintTypes.h"
#include "PxcSolverContact.h"
#include "PxcSolverContactPF.h"
#include "PxsDynamics.h"

using namespace physx;

PxsContactManager::PxsContactManager(PxsContext*, PxU32 index) :
	//mBodyShape0 (0), 
	//mBodyShape1 (0),
	mUserData	(NULL)
{
	mFlags = 0;

	mNpUnit.index				= index;
	mNpUnit.rigidCore0			= 0;
	mNpUnit.rigidCore1			= 0;
	//mNpUnit.dynamicFriction		= 0;
	//mNpUnit.staticFriction		= 0;
	//mNpUnit.restitution			= 0;
	mNpUnit.restDistance		= 0;
	mNpUnit.dominance0			= 1.0f;
	mNpUnit.dominance1			= 1.0f;
	//mNpUnit.contactPoints		= NULL;
	mNpUnit.frictionDataPtr		= NULL;
	mNpUnit.frictionPatchCount	= 0;
}

PxsContactManager::~PxsContactManager()
{
}

void PxsContactManager::init(const PxvManagerDescRigidRigid& desc, const PxsMaterialManager* materialManager)
{
	//mBodyShape0					= desc.bodyShape0;
	//mBodyShape1					= desc.bodyShape1;

	mRigidBody0					= desc.rigidBody0;
	mRigidBody1					= desc.rigidBody1;

	mNpUnit.shapeCore0			= desc.shapeCore0;
	mNpUnit.shapeCore1			= desc.shapeCore1;

	PX_ASSERT(mNpUnit.shapeCore0->transform.isValid() && mNpUnit.shapeCore1->transform.isValid());

	mNpUnit.rigidCore0			= desc.rigidCore0;
	mNpUnit.rigidCore1			= desc.rigidCore1;

	//mNpUnit.dynamicFriction		= desc.dynamicFriction;
	//mNpUnit.staticFriction		= desc.staticFriction;
	//mNpUnit.restitution			= desc.restitution;
	mNpUnit.restDistance		= desc.restDistance;
	mNpUnit.dominance0			= desc.dominance0;
	mNpUnit.dominance1			= desc.dominance1;
	mNpUnit.geomType0			= PxU8(mNpUnit.shapeCore0->geometry.getType());
	mNpUnit.geomType1			= PxU8(mNpUnit.shapeCore1->geometry.getType());
	mNpUnit.materialManager     = materialManager;
	mNpUnit.mTransformCache0	= desc.transformCache0;
	mNpUnit.mTransformCache1	= desc.transformCache1;

	PxcNpWorkUnitClearContactState(mNpUnit);
	PxcNpWorkUnitClearCachedState(mNpUnit);
	
	PxU16 wuflags = 0;

	if(desc.hasArticulations & 1)
		wuflags |= PxcNpWorkUnitFlag::eARTICULATION_BODY0;

	if(desc.hasArticulations & 2)
		wuflags |= PxcNpWorkUnitFlag::eARTICULATION_BODY1;

	if(desc.hasDynamics & 1)
		wuflags |= PxcNpWorkUnitFlag::eDYNAMIC_BODY0;

	if(desc.hasDynamics & 2)
		wuflags |= PxcNpWorkUnitFlag::eDYNAMIC_BODY1;

	if(!desc.disableResponse && !desc.contactChangeable)
		wuflags |= PxcNpWorkUnitFlag::eOUTPUT_CONSTRAINTS;

	if(!desc.disableDiscreteContact)
		wuflags |= PxcNpWorkUnitFlag::eDETECT_DISCRETE_CONTACT;

	mNpUnit.disableResponse = desc.disableResponse != 0;

	mNpUnit.disableCCDContact = desc.disableCCDContact != 0;

	// this is just the user req: contact reports can also be generated by body thresholding
	if(desc.reportContactInfo || desc.contactChangeable)
		wuflags |= PxcNpWorkUnitFlag::eOUTPUT_CONTACTS;

	if(desc.hasForceThreshold)
		wuflags |= PxcNpWorkUnitFlag::eFORCE_THRESHOLD;

	if(desc.contactChangeable)
		wuflags |= PxcNpWorkUnitFlag::eMODIFIABLE_CONTACT;

	mNpUnit.flags = wuflags;

	mFlags	=  PxU32(desc.contactChangeable ? PXS_CM_CHANGEABLE : 0);

	mUserData				= desc.userData;

	PxcNpWorkUnitClearContactState(mNpUnit);

	if (desc.hasTouch > 0)
		mNpUnit.statusFlags |= PxcNpWorkUnitStatusFlag::eHAS_TOUCH;
	else if (desc.hasTouch < 0)
		mNpUnit.statusFlags |= PxcNpWorkUnitStatusFlag::eHAS_NO_TOUCH;
}


void PxsContactManager::setCCD(bool enable)
{
	PxU32 flags = mFlags & (~PXS_CM_CCD_CONTACT);
	if (enable)
		flags |= PXS_CM_CCD_LINEAR;
	else
		flags &= ~PXS_CM_CCD_LINEAR;

	mFlags = flags;
}



void PxsContactManager::resetCachedState()
{ 
	// happens when the body transform or shape relative transform changes.

	PxcNpWorkUnitClearCachedState(mNpUnit);
}

