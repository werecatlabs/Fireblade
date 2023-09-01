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

#include "CuClothData.h"
#include "CuCloth.h"
#include "CuFabric.h"
#include "CuCheckSuccess.h"
#include "CuContextLock.h"
#include "IterationState.h"

using namespace physx;

cloth::CuClothData::CuClothData(CuCloth& cloth)
{
	mNumParticles = cloth.mNumParticles; 
	mParticles = array(*cloth.mParticles.begin().get()); 

	mParticlesHostCopy = array(*getDevicePointer(cloth.mParticlesHostCopy));
	
	mNumPhases = uint32_t(cloth.mPhaseConfigs.size());
	mPhaseConfigs = cloth.mPhaseConfigs.begin().get();

	mTethers = cloth.mFabric.mTethers.begin().get();
	mNumTethers = uint32_t(cloth.mFabric.mTethers.size());
	mTetherConstraintScale = cloth.mTetherConstraintScale 
		* cloth.mFabric.mTetherLengthScale;

	mMotionConstraintScale = cloth.mMotionConstraintScale;
	mMotionConstraintBias = cloth.mMotionConstraintBias;

	mNumSpheres = uint32_t(cloth.mStartCollisionSpheres.size());
	mNumCapsules = uint32_t(cloth.mCapsuleIndices.size());
	mCapsuleIndices = getDevicePointer(cloth.mCapsuleIndices);

	mNumPlanes = uint32_t(cloth.mStartCollisionPlanes.size());
	mNumConvexes = uint32_t(cloth.mConvexMasks.size());
	mConvexMasks = getDevicePointer(cloth.mConvexMasks);

	mNumTriangles = uint32_t(cloth.mStartCollisionTriangles.size()) / 3;

	mVirtualParticleSetSizesBegin = cloth.mVirtualParticleSetSizes.begin().get();
	mVirtualParticleSetSizesEnd = mVirtualParticleSetSizesBegin 
		+ cloth.mVirtualParticleSetSizes.size();
	mVirtualParticleIndices = array(*cloth.mVirtualParticleIndices.begin().get());
	mVirtualParticleWeights = array(*cloth.mVirtualParticleWeights.begin().get());

	mEnableContinuousCollision = cloth.mEnableContinuousCollision;
	mCollisionMassScale = cloth.mCollisionMassScale;
	mFrictionScale = cloth.mFriction;

	mSelfCollisionDistance = cloth.mSelfCollisionDistance;
	mSelfCollisionIndices = cloth.mSelfCollisionIndices.empty() ?
		0 : cloth.mSelfCollisionIndices.begin().get();
	mNumSelfCollisionIndices = mSelfCollisionIndices ? 
		uint32_t(cloth.mSelfCollisionIndices.size()) : mNumParticles;

	if(!cloth.mSelfCollisionData.empty())
	{
		uint32_t keySize = 2 * mNumSelfCollisionIndices;
		uint32_t particleSize = 4 * mNumParticles;

		mSelfCollisionParticles = cloth.mSelfCollisionData.begin().get();
		mSelfCollisionKeys = (uint32_t*)(mSelfCollisionParticles + particleSize);
		mSelfCollisionCellStart = (uint16_t*)(mSelfCollisionKeys + keySize);
	} else {
		mSelfCollisionParticles = 0;
		mSelfCollisionKeys = 0;
		mSelfCollisionCellStart = 0;
	}

	mSleepTestInterval = cloth.mSleepTestInterval;
	mSleepAfterCount = cloth.mSleepAfterCount;
	mSleepThreshold = cloth.mSleepThreshold;
}

cloth::CuFrameData::CuFrameData( CuCloth& cloth, uint32_t numSharedPositions,
	const IterationState<Simd4f>& state, const CuIterationData* iterationData )
{
	mDeviceParticlesDirty = cloth.mDeviceParticlesDirty;

	mNumSharedPositions = numSharedPositions;

	mIterDt = state.mIterDt;
	mNumIterations = state.mRemainingIterations;
	mIterationData = iterationData;

	Simd4f logStiffness = simd4f(0.0f, cloth.mSelfCollisionLogStiffness, 
		cloth.mMotionConstraintLogStiffness, cloth.mTetherConstraintLogStiffness);
	Simd4f stiffnessExponent = simd4f(cloth.mStiffnessFrequency * mIterDt);
	Simd4f stiffness = simd4f(_1) - exp2(logStiffness * stiffnessExponent);

	mTetherConstraintStiffness = array(stiffness)[3];
	mMotionConstraintStiffness = array(stiffness)[2];
	mSelfCollisionStiffness = array(stiffness)[1];

	mTargetMotionConstraints = 0;
	if(!cloth.mMotionConstraints.mStart.empty()) 
	{
		mTargetMotionConstraints = array(
			*cloth.mMotionConstraints.mStart.begin().get());
	}

	mStartMotionConstraints = mTargetMotionConstraints;
	if(!cloth.mMotionConstraints.mTarget.empty())
	{
		mTargetMotionConstraints = array(
			*cloth.mMotionConstraints.mTarget.begin().get());
	}

	mHostMotionConstraints = array(*getDevicePointer(cloth.mMotionConstraints.mHostCopy));

	mTargetSeparationConstraints = 0;
	if(!cloth.mSeparationConstraints.mStart.empty()) 
	{
		mTargetSeparationConstraints = array(
			*cloth.mSeparationConstraints.mStart.begin().get());
	}

	mStartSeparationConstraints = mTargetSeparationConstraints;
	if(!cloth.mSeparationConstraints.mTarget.empty())
	{
		mTargetSeparationConstraints = array(
			*cloth.mSeparationConstraints.mTarget.begin().get());
	}

	mHostSeparationConstraints = array(*getDevicePointer(cloth.mSeparationConstraints.mHostCopy));

	mParticleAccelerations = 0;
	if(!cloth.mParticleAccelerations.empty()) 
	{
		mParticleAccelerations = array(
			*cloth.mParticleAccelerations.begin().get());
	}

	mHostParticleAccelerations = array(*getDevicePointer(cloth.mParticleAccelerationsHostCopy));

	mRestPositions = 0;
	if(!cloth.mRestPositions.empty()) 
	{
		mRestPositions = array(
			*cloth.mRestPositions.begin().get());
	}

	mStartCollisionSpheres = array(*getDevicePointer(cloth.mStartCollisionSpheres));
	mTargetCollisionSpheres = array(*getDevicePointer(cloth.mTargetCollisionSpheres));

	if(!mTargetCollisionSpheres)
		mTargetCollisionSpheres = mStartCollisionSpheres;

	mStartCollisionPlanes = array(*getDevicePointer(cloth.mStartCollisionPlanes));
	mTargetCollisionPlanes = array(*getDevicePointer(cloth.mTargetCollisionPlanes));

	if(!mTargetCollisionPlanes)
		mTargetCollisionPlanes = mStartCollisionPlanes;

	mStartCollisionTriangles = array(*getDevicePointer(cloth.mStartCollisionTriangles));
	mTargetCollisionTriangles = array(*getDevicePointer(cloth.mTargetCollisionTriangles));

	if(!mTargetCollisionTriangles)
		mTargetCollisionTriangles = mStartCollisionTriangles;

	for(uint32_t i=0; i<3; ++i)
	{
		float c = cloth.mParticleBoundsCenter[i];
		float r = cloth.mParticleBoundsHalfExtent[i];
		mParticleBounds[i*2+0] = r + c;
		mParticleBounds[i*2+1] = r - c;
	}

	mSleepPassCounter = cloth.mSleepPassCounter;
	mSleepTestCounter = cloth.mSleepTestCounter;

	mStiffnessExponent = cloth.mStiffnessFrequency*mIterDt; 
}

namespace
{
	void copySquareTransposed(float* dst, const float* src)
	{
		dst[0] = src[ 0];
		dst[1] = src[ 4];
		dst[2] = src[ 8];
		dst[3] = src[ 1];
		dst[4] = src[ 5];
		dst[5] = src[ 9];
		dst[6] = src[ 2];
		dst[7] = src[ 6];
		dst[8] = src[10];
	}
}

cloth::CuIterationData::CuIterationData( const IterationState<Simd4f>& state)
{
	mIntegrationTrafo[0] = array(state.mPrevBias)[0];
	mIntegrationTrafo[1] = array(state.mPrevBias)[1];
	mIntegrationTrafo[2] = array(state.mPrevBias)[2];

	mIntegrationTrafo[3] = array(state.mCurBias)[0];
	mIntegrationTrafo[4] = array(state.mCurBias)[1];
	mIntegrationTrafo[5] = array(state.mCurBias)[2];

	copySquareTransposed(mIntegrationTrafo + 6, array(*state.mPrevMatrix));
	copySquareTransposed(mIntegrationTrafo + 15, array(*state.mCurMatrix));

	mIsTurning = uint32_t(state.mIsTurning);
}
