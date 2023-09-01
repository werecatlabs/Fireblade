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


#include "ScArticulationJointCore.h"

#include "ScArticulationJointSim.h"
#include "ScBodyCore.h"

using namespace physx;

Sc::ArticulationJointCore::ArticulationJointCore(const PxTransform& parentFrame,
												 const PxTransform& childFrame) :
	mSim	(NULL)
{
	PX_ASSERT(parentFrame.isValid());
	PX_ASSERT(childFrame.isValid());

	mCore.parentPose			= parentFrame;
	mCore.childPose				= childFrame;

	mCore.targetPosition		= PxQuat(PxIdentity);
	mCore.targetVelocity		= PxVec3(0);

	mCore.driveType				= PxArticulationJointDriveType::eTARGET;

	mCore.spring				= 0.0f;
	mCore.damping				= 0.0f;

	mCore.internalCompliance	= 1.0f;
	mCore.externalCompliance	= 1.0f;

	mCore.swingYLimit			= PxPi/4;
	mCore.swingZLimit			= PxPi/4;
	mCore.swingLimitContactDistance			= 0.05f;
	mCore.swingLimited			= false;
	mCore.tangentialStiffness	= 0.0f;
	mCore.tangentialDamping		= 0.0f;

	mCore.twistLimitLow			= -PxPi/4;
	mCore.twistLimitHigh		= PxPi/4;
	mCore.twistLimitContactDistance			= 0.05f;
	mCore.twistLimited			= false;
	

	mCore.tanQSwingY			= PxTan(mCore.swingYLimit/4);
	mCore.tanQSwingZ			= PxTan(mCore.swingZLimit/4);
	mCore.tanQSwingPad			= PxTan(mCore.swingLimitContactDistance/4);

	mCore.tanQTwistHigh			= PxTan(mCore.twistLimitHigh/4);
	mCore.tanQTwistLow			= PxTan(mCore.twistLimitLow/4);
	mCore.tanQTwistPad			= PxTan(mCore.twistLimitContactDistance/4);

}


Sc::ArticulationJointCore::~ArticulationJointCore()
{
	PX_ASSERT(getSim() == 0);
}


//--------------------------------------------------------------
//
// ArticulationJointCore interface implementation
//
//--------------------------------------------------------------


void Sc::ArticulationJointCore::setParentPose(const PxTransform& t)
{
	mCore.parentPose = t;
}


void Sc::ArticulationJointCore::setChildPose(const PxTransform& t)
{
	mCore.childPose = t;
}


void Sc::ArticulationJointCore::setTargetOrientation(const PxQuat& p)
{
	mCore.targetPosition = p;
}


void Sc::ArticulationJointCore::setTargetVelocity(const PxVec3& v)
{
	mCore.targetVelocity = v;
}

void Sc::ArticulationJointCore::setDriveType(PxArticulationJointDriveType::Enum type)
{
	mCore.driveType = PxU8(type);
}


void Sc::ArticulationJointCore::setStiffness(PxReal s)
{
	mCore.spring = s;
}


void Sc::ArticulationJointCore::setDamping(PxReal d)
{
	mCore.damping = d;
}


void Sc::ArticulationJointCore::setInternalCompliance(PxReal r)
{
	mCore.internalCompliance = r;
}

void Sc::ArticulationJointCore::setExternalCompliance(PxReal r)
{
	mCore.externalCompliance = r;
}


void Sc::ArticulationJointCore::setSwingLimit(PxReal yLimit, PxReal zLimit)
{
	mCore.swingYLimit = yLimit;
	mCore.swingZLimit = zLimit;

	mCore.tanQSwingY			= PxTan(yLimit/4);
	mCore.tanQSwingZ			= PxTan(zLimit/4);
}


void Sc::ArticulationJointCore::setTangentialStiffness(PxReal s)
{
	mCore.tangentialStiffness = s;
}


void Sc::ArticulationJointCore::setTangentialDamping(PxReal d)
{
	mCore.tangentialDamping = d;
}


void Sc::ArticulationJointCore::setSwingLimitEnabled(bool e)
{
	mCore.swingLimited = e;
}

void Sc::ArticulationJointCore::setSwingLimitContactDistance(PxReal e)
{
	mCore.swingLimitContactDistance = e;
	mCore.tanQSwingPad = PxTan(e/4);
}


void Sc::ArticulationJointCore::setTwistLimit(PxReal lower, PxReal upper)
{
	mCore.twistLimitLow = lower;
	mCore.twistLimitHigh = upper;

	mCore.tanQTwistHigh			= PxTan(upper/4);
	mCore.tanQTwistLow			= PxTan(lower/4);
}


void Sc::ArticulationJointCore::setTwistLimitEnabled(bool e)
{
	mCore.twistLimited = e;
}

void Sc::ArticulationJointCore::setTwistLimitContactDistance(PxReal e)
{
	mCore.twistLimitContactDistance = e;
	mCore.tanQTwistPad = PxTan(e/4);
}
