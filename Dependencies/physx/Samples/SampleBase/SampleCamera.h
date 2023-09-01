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

#ifndef SAMPLE_CAMERA_H
#define SAMPLE_CAMERA_H

#include "SampleAllocator.h"
#include "RendererProjection.h"
#include "foundation/PxPlane.h"

	class RenderPhysX3Debug;

	struct Viewport : public SampleAllocateable
	{
				Viewport() : mClientWidth(0), mClientHeight(0), mWindowWidth(0), mWindowHeight(0)	{}

		PxU32	mClientWidth;
		PxU32	mClientHeight;
		PxU32	mWindowWidth;
		PxU32	mWindowHeight;

		PX_FORCE_INLINE	PxReal	computeRatio()	const	{ return PxReal(mWindowWidth)/PxReal(mWindowHeight);	}
	};

	enum PlaneAABBCode
	{
		PLANEAABB_EXCLUSION,
		PLANEAABB_INTERSECT,
		PLANEAABB_INCLUSION
	};

	class Camera : public SampleAllocateable
	{
		public:
													Camera();
													~Camera();

		///////////////////////////////////////////////////////////////////////////////

		// Projection part

		PX_FORCE_INLINE	PxReal						getFOV()			const	{ return mFOV;						}
		PX_FORCE_INLINE	PxReal						getNearPlane()		const	{ return mNearPlane;				}
		PX_FORCE_INLINE	PxReal						getFarPlane()		const	{ return mFarPlane;					}
		PX_FORCE_INLINE	PxU32						getScreenWidth()	const	{ return mViewport.mClientWidth;	}
		PX_FORCE_INLINE	PxU32						getScreenHeight()	const	{ return mViewport.mClientHeight;	}

		PX_FORCE_INLINE	void						setFOV(PxReal fov)			{ mFOV			= fov;	mDirtyProj = true;	}
		PX_FORCE_INLINE	void						setNearPlane(PxReal d)		{ mNearPlane	= d;	mDirtyProj = true;	}
		PX_FORCE_INLINE	void						setFarPlane(PxReal d)		{ mFarPlane		= d;	mDirtyProj = true;	}
		PX_FORCE_INLINE	void						setScreenSize(PxU32 clientWidth, PxU32 clientHeight, PxU32 windowWidth, PxU32 windowHeight)
													{
														mViewport.mClientWidth	= clientWidth;
														mViewport.mClientHeight	= clientHeight;
														mViewport.mWindowWidth	= windowWidth;
														mViewport.mWindowHeight	= windowHeight;
														mDirtyProj				= true;
													}

		PX_FORCE_INLINE	const SampleRenderer::RendererProjection&
													getProjMatrix()		const
													{
														if(mDirtyProj)
															const_cast<Camera*>(this)->updateInternals();

														return mProjMatrix;
													}

		///////////////////////////////////////////////////////////////////////////////

		// View part

		PX_FORCE_INLINE	const PxVec3&				getPos()					const	{ return mPos;								}
		PX_FORCE_INLINE	const PxVec3&				getRot()					const	{ return mRot;								}

		PX_FORCE_INLINE	void						setPos(const PxVec3& pos)			{ mPos = pos;			mDirtyView = true;	}
		PX_FORCE_INLINE	void						setRot(const PxVec3& rot)			{ mRot = rot;			mDirtyView = true;	}
		PX_FORCE_INLINE	void						setView(const PxTransform& view)	{ mViewMatrix = view;	mPos = view.p; mDirtyView = false;	}

		PX_FORCE_INLINE	const PxTransform&			getViewMatrix()				const
													{
														if(mDirtyView)
															const_cast<Camera*>(this)->updateInternals();

														return mViewMatrix;
													}

						PxVec3						getViewDir()				const;
						void						lookAt(const PxVec3& position, const PxVec3& target);

		///////////////////////////////////////////////////////////////////////////////

		PX_FORCE_INLINE	const PxVec3*				getFrustumVerts()	const	{ return mFrustum;	}

		///////////////////////////////////////////////////////////////////////////////

		// Culling

						PlaneAABBCode				cull(const PxBounds3& aabb)	const;
						bool						mDrawDebugData;
						bool						mFreezeFrustum;
						bool						mPerformVFC;

		///////////////////////////////////////////////////////////////////////////////

						void						drawDebug(RenderPhysX3Debug*);
		private:
		mutable			SampleRenderer::RendererProjection
													mProjMatrix;
		mutable			PxTransform					mViewMatrix;

						PxVec3						mPos;
						PxVec3						mRot;

						Viewport					mViewport;
						PxReal						mFOV;
						PxReal						mNearPlane;
						PxReal						mFarPlane;

						PxVec3						mFrustum[8];	//!< Frustum's vertices
						PxPlane						mPlanes[6];		//!< Frustum's planes

						bool						mDirtyProj;
						bool						mDirtyView;

						void						updateInternals();
	public:
						void						BuildFrustum();
	};

#endif
