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


#ifndef PX_PARTICLE_DEVICE_EXCLUSIVE_H
#define PX_PARTICLE_DEVICE_EXCLUSIVE_H

#include "PxPhysXCommonConfig.h"
#include "PxFlags.h"

typedef struct CUstream_st *CUstream; 

namespace physx
{

/**
\brief Internal per particle flags to mark changes in device particle data.
@see PxParticleDeviceExclusive.getReadWriteCudaBuffers
*/
struct PxInternalParticleFlagGpu
{
	enum Enum
	{
		//reserved	(1<<0),
		//reserved	(1<<1),
		//reserved	(1<<2),
		//reserved	(1<<3),
		//reserved	(1<<4),
		//reserved	(1<<5),
		eCUDA_NOTIFY_CREATE								= (1<<6),
		eCUDA_NOTIFY_SET_POSITION						= (1<<7)
	};
};

/**
\brief Combined api and internal flags for device particle data.
@see PxInternalParticleFlagGpu, PxParticleDeviceExclusive.getReadWriteCudaBuffers
*/
struct PxParticleFlagGpu
{
	PxParticleFlagGpu(PxU16 apiFlags, PxU16 lowFlags)
	: api(apiFlags)
	, low(lowFlags)
	{}

	PxParticleFlagGpu() {}

	PxU16 api;	// PxParticleFlag
	PxU16 low;	// PxInternalParticleFlagGpu
};

/**
\brief Device particle buffer descriptor.
*/
struct PxCudaReadWriteParticleBuffers
{
	PxVec4*				positions;
	PxVec4*				velocities;
	PxVec4*				collisionNormals;
	PxVec4*				collisionVelocities;
	PxF32*				densities;
	PxF32*				restOffset;
	PxParticleFlagGpu*	flags;
	PxU32				maxParticles;
};

/**
\brief Flags to control whether the particle simulation updates positions for new and externally moved particles.
When updating particle positions through the device (by adding particles or setting the position of particles)
collisions with the environment can't be resolved for the corresponding particles. Sometimes it's better
to avoid particle positions being changed by the simulation in these cases, to void missing collisions 
against static actors.
@see PxParticleDeviceExclusive.setFlags
*/
struct PxParticleDeviceExclusiveFlag
{
	enum Enum
	{
		eDISABLE_POSITION_UPDATE_ON_CREATE	= (1 << 0),
		eDISABLE_POSITION_UPDATE_ON_SETPOS	= (1 << 1)
	};
};

typedef PxFlags<PxParticleDeviceExclusiveFlag::Enum, PxU32> PxParticleDeviceExclusiveFlags;
PX_FLAGS_OPERATORS(PxParticleDeviceExclusiveFlag::Enum, PxU32)

/**
\brief API for gpu specific particle functionality.
*/
class PxParticleDeviceExclusive
{
public:

	/**
	\brief Experimental method for particle data read/write access from cuda device memory.

	By calling this method, the cuda particle system enters a "device exclusive" mode. This 
	means that particles can't be read nor updated through the host side PxParticleBase 
	interface. Additionally, it's not possible to enter the device exclusive mode after particles were 
	already added through the host side interface.

	Particle systems need to be part of a scene and need to have the PxParticleBaseFlag::eGPU raised in order
	to enter the device exclusive mode successfully. Particle systems that are removed from a scene leave the 
	device exclusive mode.

	Note: particle systems in device exclusive mode are not displayed with the PhysX Visual Debugger.
	@see PxParticleDeviceExclusive.isEnabled
	*/
	PX_PHYSX_CORE_API static void enable(class PxParticleBase& particleBase);

	/**
	\brief Returns whether particle system is in device exclusive mode.

	@see PxParticleDeviceExclusive.enable
	*/
	PX_PHYSX_CORE_API static bool isEnabled(class PxParticleBase& particleBase);

	/**
	\brief Experimental method for getting particle data read/write access through cuda device memory.
	
	Protocol for creating, releasing and updating particles:

	Initial particle state:
	- all fields uninitialized but flags set to 0
	
	Creating:
	- set position
	- set velocity
	- set flags: PxParticleFlagGpu.api = PxParticleFlag::eVALID, PxParticleFlagGpu.low = PxInternalParticleFlagGpu::eCUDA_NOTIFY_CREATE
	
	Releasing:
	- set flags to 0
	
	Updating positions:
	- set position
	- set flags: PxParticleFlagGpu.low |= PxInternalParticleFlagGpu::eCUDA_NOTIFY_SET_POSITION
	
	Updating velocities:
	- set velocity
	
	Updating with forces: 
	- not supported 
	
	Updating restOffsets:
	- not supported yet

	\param[in] particleBase PxParticleBase instance to fetch cuda buffers from.
	\param[out] buffers Descriptor for cuda device particle data.
	*/
	PX_PHYSX_CORE_API static void getReadWriteCudaBuffers(class PxParticleBase& particleBase, PxCudaReadWriteParticleBuffers& buffers);

	/**
	\brief Experimental method for optionally setting the valid particle range in device exclusive mode.
	
	By default PxParticleBase::getMaxParticles is used to define the range at which particle are processed in "device exclusive" mode.
	Optionally this can be optimized by the application using this method. This method should be called for every following 
	call to PxScene::simulate(), otherwise PxParticleBase::getMaxParticles is assumed.
	
	\param[in] particleBase PxParticleBase instance to fetch cuda buffers from.
	\param[in] validParticleRange valid particle range (index+1, with index being the highest particle index with (PxCudaReadWriteParticleBuffers::flags[index].api & PxParticleFlag::eVALID != 0) 

	@see PxParticleDeviceExclusive.getReadWriteCudaBuffers
	*/
	PX_PHYSX_CORE_API static void setValidParticleRange(class PxParticleBase& particleBase, PxU32 validParticleRange);

	/**
	\brief Set flags to control position update for added particles and particles for which position has been set. 
	@see PxParticleDeviceExclusiveFlags
	*/
	PX_PHYSX_CORE_API static void setFlags(class PxParticleBase& particleBase, PxParticleDeviceExclusiveFlags flags);

	/**
	\brief Query task that launches PhysX particle kernels.
	*/
	PX_PHYSX_CORE_API static class PxBaseTask* getLaunchTask(class PxParticleBase& particleBase);

	/**
	\brief Adds a dependency to the PhysX particle kernel launch task.

	Similarly to PxGpuDispatcher.addPreLaunchDependent and PxGpuDispatcher.addPostLaunchDependent
	this adds a reference to the launch task.
	*/
	PX_PHYSX_CORE_API static void addLaunchTaskDependent(class PxParticleBase& particleBase, class PxBaseTask& dependent);

	/**
	\brief Query the cuda stream that is used for the PhysX particle kernels.
	\note The stream is not valid if getLaunchTask returns NULL.
	\return the cuda stream.
	*/
	PX_PHYSX_CORE_API static CUstream getCudaStream(class PxParticleBase& particleBase);
};

/**
Internal class providing device exclusive mode functionality.
*/
class PxParticleDeviceExclusiveAccess
{
public:
	virtual void getReadWriteCudaBuffers(PxCudaReadWriteParticleBuffers& buffers) const = 0;
	virtual void setValidParticleRange(PxU32 validParticleRange) = 0;
	virtual void setFlags(PxU32 flags) = 0;
	virtual class PxBaseTask* getLaunchTask() const = 0;
	virtual void addLaunchTaskDependent(class PxBaseTask& dependent) = 0;
	virtual size_t getCudaStream() const = 0;
};

}

#endif // PX_SUPPORT_GPU_PHYSX
