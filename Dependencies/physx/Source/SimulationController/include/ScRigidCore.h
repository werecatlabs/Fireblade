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


#ifndef PX_PHYSICS_SCP_RB_CORE
#define PX_PHYSICS_SCP_RB_CORE

#include "ScActorCore.h"
#include "PxvDynamics.h"
#include "PxShape.h"

namespace physx
{

namespace Sc
{

	class RigidSim;

	struct ShapeChangeNotifyFlag
	{
		enum Enum
		{
			eGEOMETRY			= 1<<0,
			eMATERIAL			= 1<<1,
			eSHAPE2BODY			= 1<<2,
			eFILTERDATA			= 1<<3,
			eCONTACTOFFSET		= 1<<4,
			eRESTOFFSET			= 1<<5,
			eFLAGS				= 1<<6,
			eRESET_FILTERING	= 1<<7

		};
	};
	typedef PxFlags<ShapeChangeNotifyFlag::Enum, PxU32> ShapeChangeNotifyFlags;
	PX_FLAGS_OPERATORS(ShapeChangeNotifyFlag::Enum,PxU32)


	class ShapeCore;

	class RigidCore : public ActorCore
	{
	//= ATTENTION! =====================================================================================
	// Changing the data layout of this class breaks the binary serialization format.  See comments for 
	// PX_BINARY_SERIAL_VERSION.  If a modification is required, please adjust the getBinaryMetaData 
	// function.  If the modification is made on a custom branch, please change PX_BINARY_SERIAL_VERSION
	// accordingly.
	//==================================================================================================
	public:
				PxActor*	getPxActor() const;
				void		addShapeToScene(ShapeCore& shape);
				void		removeShapeFromScene(ShapeCore& shape, bool wakeOnLostTouch);
				void		onShapeChange(ShapeCore& shape, ShapeChangeNotifyFlags notifyFlags, PxShapeFlags newShapeFlags = PxShapeFlags());

				RigidSim*	getSim() const;
		static	void		getBinaryMetaData(PxOutputStream& stream);
	protected:
							RigidCore(const PxEMPTY&) :	ActorCore(PxEmpty)	{}
							RigidCore(PxActorType::Enum type);
							~RigidCore();
	};

} // namespace Sc

}

#endif
