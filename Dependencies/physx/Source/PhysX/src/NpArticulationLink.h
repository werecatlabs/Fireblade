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


#ifndef PX_PHYSICS_NP_ARTICULATION_LINK
#define PX_PHYSICS_NP_ARTICULATION_LINK

#include "NpRigidBodyTemplate.h"
#include "PxArticulationLink.h"
#include "NpArticulation.h"

#if PX_ENABLE_DEBUG_VISUALIZATION
#include "CmRenderOutput.h"
#endif

namespace physx
{

class NpArticulationLink;
class NpArticulationJoint;
class PxConstraintVisualizer;

class NpArticulationLink;
typedef NpRigidBodyTemplate<PxArticulationLink> NpArticulationLinkT;

class NpArticulationLink : public NpArticulationLinkT
{
//= ATTENTION! =====================================================================================
// Changing the data layout of this class breaks the binary serialization format.  See comments for 
// PX_BINARY_SERIAL_VERSION.  If a modification is required, please adjust the getBinaryMetaData 
// function.  If the modification is made on a custom branch, please change PX_BINARY_SERIAL_VERSION
// accordingly.
//==================================================================================================
public:
// PX_SERIALIZATION
									NpArticulationLink(PxBaseFlags baseFlags) : NpArticulationLinkT(baseFlags), mChildLinks(PxEmpty)	{}
	virtual		void				exportExtraData(PxSerializationContext& stream);
				void				importExtraData(PxDeserializationContext& context);
				void				registerReferences(PxSerializationContext& stream);
				void				resolveReferences(PxDeserializationContext& context);
	virtual		void				requires(PxProcessPxBaseCallback& c);
	virtual		bool			    isSubordinate()  const	 { return true; } 
	static		NpArticulationLink*	createObject(PxU8*& address, PxDeserializationContext& context);
	static		void				getBinaryMetaData(PxOutputStream& stream);		
//~PX_SERIALIZATION
	virtual							~NpArticulationLink();

	//---------------------------------------------------------------------------------
	// PxArticulationLink implementation
	//---------------------------------------------------------------------------------
	virtual		void				release();


	virtual		PxActorType::Enum	getType() const { return PxActorType::eARTICULATION_LINK; }

	// Pose
	virtual		void				setGlobalPose(const PxTransform& pose);
	virtual		void 				setGlobalPose(const PxTransform& pose, bool autowake);
	virtual		PxTransform			getGlobalPose() const;

	// Velocity
	virtual		void				setLinearVelocity(const PxVec3&, bool autowake = true);
	virtual		void				setAngularVelocity(const PxVec3&, bool autowake = true);

	virtual		PxArticulation&		getArticulation() const;
	
	virtual		PxArticulationJoint* getInboundJoint() const;

	virtual		PxU32				getNbChildren() const;
	virtual		PxU32				getChildren(PxArticulationLink** userBuffer, PxU32 bufferSize) const;

	virtual		void				setCMassLocalPose(const PxTransform& pose);

	virtual		void				addForce(const PxVec3& force, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autowake = true);
	virtual		void				addTorque(const PxVec3& torque, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autowake = true);
	virtual		void				clearForce(PxForceMode::Enum mode = PxForceMode::eFORCE);
	virtual		void				clearTorque(PxForceMode::Enum mode = PxForceMode::eFORCE);

	//---------------------------------------------------------------------------------
	// Miscellaneous
	//---------------------------------------------------------------------------------
									NpArticulationLink(const PxTransform& bodyPose, NpArticulation& root, NpArticulationLink* parent);

				void				releaseInternal();

	PX_INLINE	NpArticulation&		getRoot()	{ return *mRoot; }
	PX_INLINE	NpArticulationLink*	getParent()	{ return mParent; }

	PX_INLINE	void				setInboundJoint(NpArticulationJoint& joint) { mInboundJoint = &joint; }

private:
	PX_INLINE	void				addToChildList(NpArticulationLink& link) { mChildLinks.pushBack(&link); }
	PX_INLINE	void				removeFromChildList(NpArticulationLink& link) { PX_ASSERT(mChildLinks.find(&link) != mChildLinks.end()); mChildLinks.findAndReplaceWithLast(&link); }

public:
	PX_INLINE	NpArticulationLink* const*	getChildren() { return mChildLinks.empty() ? NULL : &mChildLinks.front(); }

#if PX_ENABLE_DEBUG_VISUALIZATION
public:
				void				visualize(Cm::RenderOutput& out, NpScene* scene);
				void				visualizeJoint(PxConstraintVisualizer& jointViz);
#endif


private:
				NpArticulation*					mRoot;  //!!!AL TODO: Revisit: Could probably be avoided if registration and deregistration in root is handled differently
				NpArticulationJoint*			mInboundJoint;
				NpArticulationLink*				mParent;  //!!!AL TODO: Revisit: Some memory waste but makes things faster
				NpArticulationLinkArray			mChildLinks;
};

}

#endif
